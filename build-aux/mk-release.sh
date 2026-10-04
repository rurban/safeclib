#!/bin/sh
# mk-release.sh -- tag, publish and sign a safeclib release on GitHub.
#
# Adapted from rcc's tools/mk-release.sh.
#
# Usage: build-aux/mk-release.sh VERSION
#   VERSION without the "v" prefix, e.g. "3.13".
#
# Run by `make release` after the dist archives are built and the
# release-readiness checks (clean tree, correct branch, GPG-signed
# HEAD, ChangeLog updated -- see build-aux/changelog-check.sh and the
# Makefile.am `release` target) have already passed. This script does
# not repeat those; it:
#
#   1. Re-verifies HEAD is GPG-signed (defensive).
#   2. Creates the lightweight tag vVERSION (if not already tagged).
#      safeclib tags are plain refs, not signed tag objects -- the
#      signature lives on the commit (see e.g. v3.12) -- so, unlike
#      rcc, this does not use `git tag -s`.
#   3. Pushes the branch and the tag.
#   4. Waits for that push's CI run to go green. The mingw job in
#      .github/workflows/main.yml creates the GitHub release itself
#      (via softprops/action-gh-release) as soon as the tag appears,
#      uploading its *.zip/*.sha256 assets -- this script waits for
#      that to finish before touching the release.
#   5. GPG-detach-signs every safeclib-VERSION.tar.{gz,bz2,xz} dist
#      archive.
#   6. Builds release notes in the same shape as
#      https://github.com/rurban/rcc/releases/tag/v1.2.0 : a
#      "## What's Changed" section (from ChangeLog), a Full Changelog
#      compare link, a "## Files" section, and a "## Verification"
#      section with sha256sums and gpg-verify instructions.
#   7. Uploads the signed source archives to the release CI already
#      created (falling back to creating it, if the mingw job was
#      skipped or failed to), then edits the notes to add the "##
#      What's Changed"/"## Files"/"## Verification" sections ahead of
#      whatever CI already appended (the permanent asset URLs only
#      exist after upload).
set -eu

version="${1:?usage: $0 VERSION}"
version="${version#v}"
tag="v${version}"
repo="${GH_REPO:-rurban/safeclib}"
gpg_fpr="${SAFECLIB_RELEASE_GPG_FPR:-38A4167B0DB69E49C5F7216CB8C28866AB27A7A2}"
branch="$(git rev-parse --abbrev-ref HEAD)"

echo "==> Verifying HEAD is GPG-signed..."
sig_status="$(git log -1 --pretty=%G?)"
case "$sig_status" in
    G | U) : ;;
    *)
        echo "HEAD commit is not GPG-signed (status: $sig_status)." >&2
        echo "Amend it first: git commit --amend -S --no-edit" >&2
        exit 1
        ;;
esac

if git rev-parse "$tag" >/dev/null 2>&1; then
    echo "==> Tag $tag already exists, reusing it."
else
    echo "==> Tagging $tag..."
    git tag -f "$tag"
fi

echo "==> Pushing $branch and $tag..."
git push origin "$branch"
git push -f origin "$tag"

echo "==> Waiting for CI run to appear..."
sleep 5
run_id=$(gh run list --repo "$repo" --branch "$branch" --limit 1 --json databaseId --jq '.[0].databaseId')
if [ -z "$run_id" ]; then
    echo "Could not find a CI run for branch $branch." >&2
    exit 1
fi
echo "==> Watching CI run $run_id..."
gh run watch --repo "$repo" --exit-status "$run_id"

echo "==> Signing dist archives..."
set -- safeclib-"${version}".tar.gz safeclib-"${version}".tar.bz2 safeclib-"${version}".tar.xz
archives=""
sigs=""
for f in "$@"; do
    [ -e "$f" ] || continue
    case "$f" in *.sig.txt) continue ;; esac
    gpg --detach-sign --armor -o "${f}.sig.txt" "$f"
    archives="$archives $f"
    sigs="$sigs ${f}.sig.txt"
done
if [ -z "$archives" ]; then
    echo "No dist archives found matching safeclib-${version}.tar.{gz,bz2,xz}." >&2
    echo "Run 'make dist' first." >&2
    exit 1
fi

echo "==> Computing checksums..."
# shellcheck disable=SC2086
checksums=$(sha256sum $archives)

echo "==> Extracting ChangeLog section for $version..."
changed=$(mktemp)
notes=$(mktemp)
trap 'rm -f "$changed" "$notes" "$notes.final"' EXIT
awk -v ver="$version" '
    /^Changes in / {
        if (found) exit
        if ($0 ~ "^Changes in v?" ver "$") { found = 1; next }
        next
    }
    found { print }
' ChangeLog | sed -e '/./,$!d' -e 's/^\t//' >"$changed"
if [ ! -s "$changed" ]; then
    echo "Could not extract a ChangeLog section for $version." >&2
    exit 1
fi

prev_tag=$(git tag --sort=-v:refname | grep -A1 -x "$tag" | tail -n1)

{
    echo "## What's Changed"
    echo
    cat "$changed"
    echo
    if [ -n "$prev_tag" ] && [ "$prev_tag" != "$tag" ]; then
        echo "**Full Changelog**: [${prev_tag}...${tag}](https://github.com/${repo}/compare/${prev_tag}...${tag})"
        echo
    fi
} >"$notes"

ci_body=""
if gh release view "$tag" --repo "$repo" >/dev/null 2>&1; then
    echo "==> Release $tag already exists (created by CI), uploading archives..."
    # shellcheck disable=SC2086
    gh release upload "$tag" $archives $sigs --repo "$repo" --clobber
    ci_body=$(gh release view "$tag" --repo "$repo" --json body --jq .body)
else
    echo "==> Creating GitHub release $tag..."
    # shellcheck disable=SC2086
    gh release create "$tag" $archives $sigs \
        --repo "$repo" \
        --title "safeclib release $tag" \
        --notes-file "$notes"
fi

echo "==> Adding Files/Verification sections..."
files_section=$(
    gh release view "$tag" --repo "$repo" --json assets \
        --jq '.assets[] | "[\(.name)](\(.url))  "'
)

{
    cat "$notes"
    echo "## Files"
    echo
    printf '%s\n' "$files_section"
    echo
    echo "## Verification"
    echo
    echo '```'
    printf '%s\n' "$checksums"
    echo '```'
    echo
    echo "Use the .sig.txt files to verify that the corresponding file (without the"
    echo ".sig.txt suffix) is intact. First, be sure to download both the .sig.txt file"
    echo "and the corresponding archive. Then, run a command like this:"
    echo
    echo '```'
    echo "mv archive.sig.txt archive.sig"
    echo "gpg --verify archive.sig"
    echo '```'
    echo
    echo "If that command fails because you don't have the required public key,"
    echo "then run this command to import it:"
    echo
    echo '```'
    echo "gpg --keyserver keys.gnupg.net --recv-keys ${gpg_fpr}"
    echo '```'
    echo
    echo "and rerun the 'gpg --verify' command."
    if [ -n "$ci_body" ]; then
        echo
        echo "## mingw CI binaries"
        echo
        printf '%s\n' "$ci_body"
    fi
} >"$notes.final"

gh release edit "$tag" --repo "$repo" --notes-file "$notes.final"

echo "Done: https://github.com/${repo}/releases/tag/${tag}"
