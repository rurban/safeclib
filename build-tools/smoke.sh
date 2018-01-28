#!/bin/sh
autoreconf
make=make

case `uname` in
Darwin) # macports compilers
    make=gmake

$make -s clean
CC="clang-mp-5.0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
$make -s clean
CC="clang-mp-3.9 -fsanitize=address -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
$make -s clean
CC="clang-mp-5.0 -std=c11" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
    # too many darwin kernel and libc leaks, esp. with locale and time.
    # not getting better, getting worse
    gmake -s -j4 check-valgrind
# try the BSD/darwin memset_s
CFLAGS="-g -DTEST_MSVCRT" ./configure --enable-shared --enable-debug --enable-unsafe && \
    make check-log || exit
CC="clang-mp-4.0 -std=c99" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
    gmake -s -j4 check-valgrind
CC="gcc-mp-4.3 -ansi" ./configure && \
    gmake -s -j4 check-log || exit
CC="gcc-mp-4.3 -std=iso9899:199409" ./configure && \
    gmake -s -j4 check-log || exit
CC="gcc-mp-6" ./configure && \
    gmake -s -j4 check-log || exit
CC="g++-mp-6 -std=c++11" ./configure --enable-unsafe --enable-norm-compat && \
    $make -s -j4 check-log || exit
CC=gcc-mp-6 \
    ./configure --enable-gcov=gcov-mp-6 --disable-shared --enable-unsafe \
                --enable-norm-compat && \
    $make -s -j4 gcov
$make clean
#clang++ not
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check-log || exit
;;

Linux)
make -s clean
CC="clang-3.9 -fsanitize=address -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    make -s -j4 check-log || exit
$make -s clean
CC="clang-4.0 -std=c99" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    make -s -j4 check-log || exit
    #TODO: valgrind broken with kpti
    #make -s -j4 check-valgrind
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
CC="clang-5.0" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    make -s -j4 check-log || exit
CC="clang-6.0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    make -s -j4 check-log || exit
$make -s clean
./configure --disable-wchar && \
    $make -s -j4 -f Makefile.kernel || exit
$make -s clean
./configure --enable-gcov --disable-shared --enable-unsafe --enable-norm-compat && \
    $make -s -j4 gcov
#    perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
#    gcov2perl src/*/*.gcov src/*.gcov && \
#    cover -no-gcov
$make clean
CC="c++ -std=c++11" ./configure --enable-unsafe --enable-norm-compat && \
    $make -s -j4 check-log || exit
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check-log || exit
;;

MSYS_NT*)
# static, usually ours
./configure --disable-shared --enable-debug --enable-unsafe --enable-norm-compat && \
    make check-log || exit
# shared, might be the msvcrt overriding ours
./configure --enable-shared --enable-debug --enable-unsafe --enable-norm-compat && \
    make check-log || exit
# ensure we use the windows msvcrt sec_api
CFLAGS="-g -DTEST_MSVCRT" ./configure --enable-shared --enable-debug --enable-unsafe && \
    make check-log || exit
exit
;;

CYGWIN_NT*)
./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    make check-log || exit
./configure --enable-debug --enable-unsafe --enable-norm-compat --host=x86_64-w64-mingw32 && \
    make check-log || exit
exit
;;

esac

# platform independent (i.e. darwin, linux, bsd's with the 3 mingw cross compilers)
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

# different .deps format
git clean -dxf src tests
autoreconf    
#CC="x86_64-w64-mingw32-gcc"
./configure --enable-unsafe --host=x86_64-w64-mingw32 && \
    $make -s -j4 && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
          b=$(basename $t); wine $t | tee tests/$b.log; done
        rm *.dll
    fi
    $make clean
git clean -dxf src tests
autoreconf
./configure --enable-unsafe --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
          b=$(basename $t); wine $t | tee tests/$b.log; done
        rm *.dll
    fi
    $make clean
git clean -dxf src tests
autoreconf    
#CC="i386-mingw32-gcc"
./configure --enable-unsafe --host=i386-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
          b=$(basename $t); wine $t | tee tests/$b.log; done
        rm *.dll
    fi
    $make clean
git clean -dxf src tests
autoreconf    
./configure --enable-unsafe --enable-debug && \
    $make -s -j4 check-log || exit
