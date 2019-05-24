#!/bin/sh

rm -rf src/*/.deps src/.deps tests/.deps

case `uname` in
Darwin) # darwin is preferred

    CC=gcc-mp-6 ./configure --enable-gcov=gcov-mp-6 --disable-shared --enable-unsafe \
                            --enable-norm-compat && \
        gmake -s -j4 all gcov ;;

Linux)

    echo "The repo has the stats from darwin" # expect minor differences

    ./configure --enable-gcov --disable-shared --enable-unsafe --enable-norm-compat && \
        make -s -j4 gcov ;;

esac

# now all done via make gcov
# fixup and post-process .gcov
#perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov && \
#    gcov2perl src/*/*.gcov && \
#    cover -no-gcov
