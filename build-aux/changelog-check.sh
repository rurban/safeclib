#!/bin/sh
# changelog-check.sh -- verify ChangeLog documents a release.
#
# Adapted from rcc's test/news-check.sh (itself ported from gnulib's
# top/maint.mk `news-check` target, part of the GNU Coding Standards:
# NEWS/ChangeLog must be updated before every release) to safeclib's
# ChangeLog, which uses bare "Changes in X.Y[.Z]" headings with no
# release date (unlike rcc's dated NEWS headings).
#
# Two modes:
#
#   build-aux/changelog-check.sh
#     Lint mode (no VERSION given): checks every modern "Changes in
#     X.Y[.Z]" heading in ChangeLog is well-formed and that they are
#     listed newest-first. Older entries (pre-3.7) used a
#     "Changes in vDATE X.Y.Z" scheme and are left alone.
#
#   build-aux/changelog-check.sh VERSION
#     Release mode: fails unless the first "Changes in ..." heading in
#     ChangeLog reads exactly "Changes in VERSION". Run by
#     `make release` right before tagging.
set -eu

changelog="${CHANGELOG_CHECK_FILE:-ChangeLog}"

if [ ! -f "$changelog" ]; then
    echo "$changelog: not found" >&2
    exit 1
fi

heading_re='^Changes in [0-9][0-9.]*[0-9]$'

if [ $# -eq 0 ]; then
    # Lint mode: every modern heading well-formed, versions newest-first.
    headings=$(grep -nE "$heading_re" "$changelog" || true)
    if [ -z "$headings" ]; then
        echo "$changelog: no 'Changes in VERSION' headings found" >&2
        exit 1
    fi
    versions=$(printf '%s\n' "$headings" | sed -E 's/^[0-9]+:Changes in ([0-9.]+)$/\1/')
    sorted=$(printf '%s\n' "$versions" | sort -t. -k1,1nr -k2,2nr -k3,3nr)
    if [ "$versions" != "$sorted" ]; then
        echo "$changelog: version headings are not newest-first:" >&2
        printf '%s\n' "$versions" >&2
        exit 1
    fi
    echo "$changelog: OK ($(printf '%s\n' "$versions" | wc -l | tr -d ' ') release(s), newest-first)"
    exit 0
fi

# Release mode.
version="${1#v}"

first_heading=$(awk '/^Changes in /{print; exit}' "$changelog")

if [ "$first_heading" != "Changes in $version" ]; then
    echo "$changelog: expected 'Changes in $version' as the first entry, found:" >&2
    echo "  ${first_heading:-<none>}" >&2
    echo "  ChangeLog must be updated before releasing." >&2
    exit 1
fi

echo "$changelog: OK ($first_heading)"
