#!/bin/sh
# gh-pages.sh -- rebuild and stage the gh-pages branch's generated API docs.
#
# Run by `make gh-pages` (see the Makefile.am `gh-pages:` target, which
# builds `docs` first). Extracted out of Makefile.am's recipe, rather
# than inline shell, to keep the target's body within checkmake's
# maxbodylength.
set -eu

api_version="${1:?usage: $0 SAFEC_API_VERSION}"

echo "ensure ./configure --enable-unsafe --enable-wchar"
ls doc/man/man3/tmpnam_s.3
cp -f doc/libc-overview.md doc/libc-overview.md.master
git checkout gh-pages
mv doc/libc-overview.md.master doc/libc-overview.md || true
rm -rf doc/man
rm -rf "doc/safec-${api_version}"
mkdir "doc/safec-${api_version}"
mv doc/html/* "doc/safec-${api_version}/"
rm -rf doc/html
sed -i -e "s,3\...,${api_version},g" index.html
git add index.html doc/libc-overview.md "doc/safec-${api_version}"
