#!/bin/sh
autoreconf
case `uname` in
Darwin)

CC="clang-mp-3.9 -fsanitize=address" ./configure --enable-debug && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="clang-mp-4.0" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-mp-6" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="g++-mp-6 -std=c++11" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --disable-nullslack && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
#CC="clang-mp-3.9 -fsanitize=memory" ./configure --enable-debug && \
#    gmake -s -j4 check || (cat tests/test-suite.log; exit)
;;

Linux)

CC="clang-3.9 -fsanitize=address" ./configure --enable-debug && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="clang-4.0" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="g++-6 -std=c++11" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-6" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="c++" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --disable-nullslack && \
    make -s -j4 check || (cat tests/test-suite.log; exit)

;;
esac
