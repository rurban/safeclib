#!/bin/sh
# release-build.sh -- build, tag and publish a safeclib release.
#
# Run by `make release`, after build-aux/release-checks.sh has already
# passed. Extracted out of Makefile.am's `release:` recipe, rather
# than inline shell, to keep the target's body within checkmake's
# maxbodylength. Runs, in order, exactly what the inline recipe did
# before this refactor:
#
#   1. tag vVERSION (force; lightweight tag, matching the existing
#      convention -- see build-aux/mk-release.sh).
#   2. full default build + check + docs + dist, producing the
#      archives build-aux/mk-release.sh signs and uploads.
#   3. (smoke.sh is not run automatically yet -- echoed as a
#      reminder, same as before this refactor.)
#   4. build-aux/mk-release.sh: tag push, CI wait, sign, GitHub release.
#   5. rebuild with --enable-unsafe --enable-wchar and regenerate
#      docs, for the gh-pages publish step.
#   6. make gh-pages, then commit and push the gh-pages branch.
set -eu

version="${1:?usage: $0 VERSION}"

git tag -f "v${version}" || true

build-aux/autogen.sh
./configure
make
make check
make docs
make dist

echo build-aux/smoke.sh

build-aux/mk-release.sh "$version"

./configure --enable-unsafe --enable-wchar
make
make docs
make gh-pages

git branch | grep '^\* gh-pages'
git commit -m "Update to v${version}"
git push
git checkout master
