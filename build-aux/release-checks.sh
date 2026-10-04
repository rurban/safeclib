#!/bin/sh
# release-checks.sh -- release-readiness gate for `make release`.
#
# Checks that HEAD is on the master branch and GPG-signed, then
# delegates to changelog-check.sh for the ChangeLog heading check.
# Extracted out of Makefile.am's `release:` recipe, rather than inline
# shell, to keep the target's body within checkmake's maxbodylength.
set -eu

version="${1:?usage: $0 VERSION}"

branch="$(git rev-parse --abbrev-ref HEAD)"
if [ "$branch" != "master" ]; then
    echo "You are not on the master branch (on $branch), aborting." >&2
    exit 1
fi

sig_status="$(git log -1 --pretty=%G?)"
case "$sig_status" in
    G | U) : ;;
    *)
        echo "HEAD commit is not GPG-signed (git commit -S), aborting." >&2
        exit 1
        ;;
esac

build-aux/changelog-check.sh "$version"
