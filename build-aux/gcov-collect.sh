#!/bin/sh
# gcov-collect.sh -- collect and run gcov over libsafec's gcno/gcda.
#
# Run by `make -C src gcov` (see src/Makefile.am `gcov:` target, under
# ENABLE_GCOV). Moves each source's .gcno/.gcda out of the libtool
# .libs/ build dir next to the source, then runs gcov over it.
# Extracted out of src/Makefile.am's recipe, rather than inline shell,
# to keep the target's body within checkmake's maxbodylength.
set -eu

builddir="${1:?usage: $0 BUILDDIR GCOV SOURCE...}"
gcov="${2:?usage: $0 BUILDDIR GCOV SOURCE...}"
shift 2

log="$builddir/gcov.log"
rm -f "$log"

for c in "$@"; do
    dir="$(dirname "$c")"
    base="$(basename "$c" .c)"
    if [ -e "$dir/.libs/$base.gcno" ] && [ -e "$c" ]; then
        mv "$dir/.libs/$base.gcno" "$dir/"
        mv "$dir/.libs/$base.gcda" "$dir/"
    fi
    if [ -e "$dir/$base.gcno" ] && [ -e "$c" ]; then
        "$gcov" -s "$dir" "$c" | tee -a "$log"
        mv "$base.c.gcov" "$dir/"
    fi
done
