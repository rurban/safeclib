#!/bin/sh
autoreconf
case `uname` in
Darwin)

CC="clang-mp-3.9 -fsanitize=address" ./configure --enable-debug --enable-unsafe && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="clang-mp-4.0 -std=c99" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-mp-4.3 -ansi" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-mp-4.3 -std=iso9899:199409" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-mp-6" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="g++-mp-6 -std=c++11" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
#clang++ not
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --disable-nullslack && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --enable-unsafe && \
    gmake -s -j4 check || (cat tests/test-suite.log; exit)
#CC="clang-mp-3.9 -fsanitize=memory" ./configure --enable-debug && \
#    gmake -s -j4 check || (cat tests/test-suite.log; exit)
;;

Linux)

CC="clang-3.9 -fsanitize=address" ./configure --enable-debug --enable-unsafe && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="clang-4.0 -std=c99" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-4.4 -ansi" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-4.4 -std=iso9899:199409" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="g++-6 -std=c++11" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-6" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="gcc-7" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="c++ -std=c++11" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --disable-nullslack && \
    make -s -j4 check || (cat tests/test-suite.log; exit)
CC="cc" ./configure --enable-unsafe && \
    make -s -j4 check || (cat tests/test-suite.log; exit)

;;
esac
