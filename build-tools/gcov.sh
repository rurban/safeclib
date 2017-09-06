#!/bin/sh
#./configure --enable-gcov --disable-shared --enable-unsafe && \
#    make -s -j4 gcov

# fixup and post-process .gcov

perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
    gcov2perl src/*/*.gcov src/*.gcov && \
    cover -no-gcov
