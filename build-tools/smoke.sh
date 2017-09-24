#!/bin/sh
autoreconf

case `uname` in
Darwin) # macports compilers
    make=gmake

gmake -s clean
CC="clang-mp-3.9 -fsanitize=address -fno-omit-frame-pointer" ./configure --enable-debug --enable-unsafe && \
    gmake -s -j4 check-log || exit
CC="clang-mp-4.0 -std=c99" ./configure && \
    gmake -s -j4 check-log || exit
CC="gcc-mp-4.3 -ansi" ./configure && \
    gmake -s -j4 check-log || exit
CC="gcc-mp-4.3 -std=iso9899:199409" ./configure && \
    gmake -s -j4 check-log || exit
CC="gcc-mp-6" ./configure && \
    gmake -s -j4 check-log || exit
CC="g++-mp-6 -std=c++11" ./configure && \
    gmake -s -j4 check-log || exit
CC=gcc-mp-6 ./configure --enable-gcov=gcov-mp-6 --disable-shared --enable-unsafe && \
    gmake -s -j4 gcov && \
    perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
    gcov2perl src/*/*.gcov src/*.gcov && \
    cover -no-gcov
gmake clean
#clang++ not
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check-log || exit
;;

Linux)
    make=make

make -s clean
CC="clang-3.9 -fsanitize=address -fno-omit-frame-pointer" ./configure --enable-debug --enable-unsafe && \
    make -s -j4 check-log || exit
CC="clang-4.0 -std=c99" ./configure && \
    make -s -j4 check-log || exit
CC="gcc-4.4 -ansi" ./configure && \
    make -s -j4 check-log || exit
CC="gcc-4.4 -std=iso9899:199409" ./configure && \
    make -s -j4 check-log || exit
#CC="g++-6 -std=c++11" ./configure && \
#    make -s -j4 check-log || exit
CC="gcc-6" ./configure && \
    make -s -j4 check-log || exit
CC="gcc-7" ./configure && \
    make -s -j4 check-log || exit
./configure --enable-gcov --disable-shared --enable-unsafe && \
    $make -s -j4 gcov && \
    perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
    gcov2perl src/*/*.gcov src/*.gcov && \
    cover -no-gcov
$make clean
CC="c++ -std=c++11" ./configure && \
    $make -s -j4 check-log || exit
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check-log || exit
    ;;
esac

# platform independent
CC="cc -m32" ./configure && \
    $make -s -j4 check-log || exit
./configure && \
    $make -s -j4 check-log || exit
./configure --disable-nullslack && \
    $make -s -j4 check-log || exit
./configure --disable-extensions && \
    $make -s -j4 check-log || exit
./configure --disable-wchar && \
    $make -s -j4 check-log || exit
#CC="x86_64-w64-mingw32-gcc"
./configure --enable-unsafe --host=x86_64-w64-mingw32 && \
    $make -s -j4 && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do wine $t | tee tests/${t:12}.log; done
        rm *.dll
    fi
    $make clean
./configure --enable-unsafe --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do wine $t | tee tests/${t:12}.log; done
        rm *.dll
    fi
    $make clean
#CC="i386-mingw32-gcc"
./configure --enable-unsafe --host=i386-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do wine $t | tee tests/${t:12}.log; done
        rm *.dll
    fi
    $make clean
./configure --enable-unsafe --enable-debug && \
    $make -s -j4 check-log || exit
