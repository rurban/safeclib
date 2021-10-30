#!/bin/sh
autoreconf
make=make

rm -rf src/*/.deps src/.deps tests/.deps 2>/dev/null

case `uname` in
Darwin) # macports compilers
    make=gmake

gmake -s -j4 clean
echo clang-mp-5.0 -fsanitize=address,undefined -fno-omit-frame-pointer --enable-debug --enable-unsafe --enable-norm-compat
CC="clang-mp-5.0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
# full optim: failed -O2 in wcsnset_s WCHECK_SLACK, because the word after static str1
#             was reused. off-by-one
echo clang-mp-5.0 -march=native --disable-constraint-handler --enable-unsafe --enable-norm-compat
CC="clang-mp-5.0 -march=native" \
    ./configure --disable-constraint-handler --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
# disable -DFORTIFY_SOURCE=2, asan set it to 0 already
echo clang-mp-5.0 -g -O2 -fsanitize=address,undefined -fno-omit-frame-pointer --disable-shared --enable-unsafe --enable-norm-compat
CC="clang-mp-5.0" \
    CFLAGS="-g -O2 -fsanitize=address,undefined -fno-omit-frame-pointer" \
    ./configure --disable-shared --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo clang-mp-8.0 -fsanitize=address -fno-omit-frame-pointer --enable-debug --enable-unsafe --enable-norm-compat
CC="clang-mp-8.0 -fsanitize=address -fno-omit-frame-pointer" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
# since clang 5 with diagnose_if BOS compile-time checks
echo clang-mp-5.0 -std=c11 --enable-unsafe --enable-norm-compat
CC="clang-mp-5.0 -std=c11" \
    ./configure --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
    gmake -s -j4 -C tests tests-bos
    # many darwin kernel and libc leaks, esp. with locale and time.
    gmake -s -j4 check-valgrind
gmake -s -j4 clean
# relax compile-time errors to warnings
echo clang-mp-devel -DTEST_BOS --enable-debug --enable-warn-dmax --enable-unsafe --enable-norm-compat
CC="clang-mp-devel -DTEST_BOS" \
    ./configure --enable-debug --enable-warn-dmax --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
# also check against BOS compile-time errors
echo clang-mp-devel --enable-debug --enable-unsafe --enable-norm-compat
CC="clang-mp-devel" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log && make -s -j4 -C tests tests-bos || exit
gmake -s -j4 clean
CC="clang-mp-devel" ./configure --enable-error-dmax && \
    $make -s -j4 check-log && exit
gmake -s -j4 clean
echo clang-mp-4.0 -std=c99 --enable-debug --enable-unsafe --enable-norm-compat
CC="clang-mp-4.0 -std=c99" \
    ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
    gmake -s -j4 check-valgrind
gmake -s -j4 clean
echo gcc-mp-4.3 -ansi
CC="gcc-mp-4.3 -ansi" ./configure && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo gcc-mp-4.3 -std=iso9899:199409
CC="gcc-mp-4.3 -std=iso9899:199409" ./configure && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "gcc-mp-6"
CC="gcc-mp-6" ./configure && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "gcc-mp-7  --enable-unsafe"
CC="gcc-mp-7" ./configure --enable-unsafe && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "gcc-mp-8 -march=native --enable-unsafe"
CC="gcc-mp-8 -march=native" ./configure --enable-unsafe && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "gcc-mp-9  --enable-unsafe"
CC="gcc-mp-9 -march=native" ./configure --enable-unsafe && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "gcc-mp-7 -march=native -Wa,-q --enable-unsafe"
CC="gcc-mp-7 -march=native -Wa,-q" ./configure --enable-unsafe && \
    gmake -s -j4 check-log || exit
gmake -s -j4 clean
echo "g++-mp-6 -std=c++11 --enable-unsafe --enable-norm-compat"
CC="g++-mp-6 -std=c++11" ./configure --enable-unsafe --enable-norm-compat && \
    gmake -s -j4 check-log || exit
echo gcc-mp-6 gcov
CC=gcc-mp-6 \
    ./configure --enable-gcov=gcov-mp-6 --disable-shared --enable-unsafe \
                --enable-norm-compat && \
    gmake -s -j4 gcov
gmake -s -j4 clean
#clang++ not
#CC="c++ -std=c++98" ./configure && \
    #    make -s -j4 check-log || exit

# port install arm-elf-gcc (with newlib, not glibc)
if [ -e /opt/local/bin/arm-elf-gcc-4.7 ]; then
    echo arm-elf-gcc-4.7 --enable-unsafe --host=arm-elf --disable-shared
    CC=arm-elf-gcc-4.7 ./configure --enable-unsafe --host=arm-elf --disable-shared && \
        gmake -s -j4 || exit;
    # $make -s -j4 check-log
    m -C tests tests
    for t in tests/t*_s; do
        b=$(basename $t)
        qemu-arm -L /opt/local/arm-elf $t | tee tests/$b.log
    done
    gmake -s -j4 clean
fi
if [ -e /opt/pgi/osx86-64/2019/bin/pgcc ]; then
    echo /opt/pgi/osx86-64/2019/bin/pgcc --enable-unsafe --enable-debug
    CC=/opt/pgi/osx86-64/2019/bin/pgcc ./configure --enable-unsafe --enable-debug && \
        gmake -s j4 && gmake check-log
        # fails on several not null slack with >RMAX
    gmake -s -j4 clean
fi

;;

Linux)
    make -s clean
    if test -n "`which clang`"; then
        echo clang -fsanitize=address -fno-omit-frame-pointer --enable-debug --enable-unsafe --enable-norm-compat
        CC="clang -fsanitize=address -fno-omit-frame-pointer" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat && \
            make -s -j4 check-log || exit
        make -s clean
    fi
    for clang in clang clang-{11,10,7,5.0}
    do
        if test -n `which $clang`; then
            echo $clang -march=native --disable-constraint-handler --enable-unsafe --enable-norm-compat
            CC="$clang -march=native" \
              ./configure --disable-constraint-handler --enable-unsafe --enable-norm-compat && \
                make -s -j4 check-log && make -s -j4 -C tests tests-bos
        fi
    done
    for clang in clang-{3.7,3.6,3.5,3.4}
    do
        if test -n "`which $clang`"; then
            echo $clang -std=c99 --enable-debug --enable-unsafe --enable-norm-compat
            if CC="#clang -std=c99" \
                 ./configure --enable-debug --enable-unsafe --enable-norm-compat; then
                make -s -j4 check-log || exit
            fi
            #    #TODO: valgrind broken with kpti
            #    #make -s -j4 check-valgrind
        fi
    done
    if test -n "`which gcc-4.4`"; then
        echo gcc-4.4 -ansi
        if CC="gcc-4.4 -ansi" ./configure; then
            make -s -j4 check-log || exit
        fi
        echo gcc-4.4 -std=iso9899:199409
        if CC="gcc-4.4 -std=iso9899:199409" ./configure; then
            make -s -j4 check-log || exit
        fi
    fi
    #CC="g++-6 -std=c++11" ./configure && \
        #    make -s -j4 check-log || exit
    for gcc in gcc-{12,11,10,9,8,7,6,5}
    do
        if test -n "`which $gcc`"; then
            if CC="$gcc" ./configure; then
                make -s -j4 check-log || exit
            fi
        fi
    done
    if test -n "`which clang-5.0`"; then
        # since clang 5 with diagnose_if BOS compile-time checks, but on linux it is flappy
        if CC="clang-5.0" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat; then
            make -s -j4 check-log && make -s -j4 -C tests tests-bos
        fi
    fi
    if test -n "`which clang`"; then
        if CC="clang -fsanitize=address,undefined -fno-omit-frame-pointer" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat; then
            make -s -j4 check-log || exit
        fi
        # retpoline and diagnose_if, skip compile-time errors
        CC="clang" LDFLAGS="-fuse-ld=lld" ./configure && \
            make -s -j4 check-log
        make -s clean
        # warn on compile-time errors and checks, but on linux it is flappy
        CC="clang" LDFLAGS="-fuse-ld=lld-7" ./configure --enable-warn-dmax && \
            make -s -j4 check-log && make -s -j4 -C tests tests-bos
        make -s clean
        # must error
        echo "clang -DTEST_BOS --enable-error-dmax. MUST error, ignore"
        CC="clang -DTEST_BOS" ./configure --enable-error-dmax && \
            make -s -j4 check-log && exit
        make -s clean
    fi
git clean -dxf src tests
autoreconf
echo "--disable-wchar -f Makefile.kernel"
./configure --disable-wchar && \
    make -s -j4 -f Makefile.kernel || exit
make -s clean
git clean -dxf src tests
autoreconf
echo gcc gcov
if ./configure --enable-gcov --disable-shared --enable-unsafe --enable-norm-compat; then
    $make -s -j4 gcov
    #    perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
    #    gcov2perl src/*/*.gcov src/*.gcov && \
    #    cover -no-gcov
fi
make -s clean
echo c++ -std=c++11 --enable-unsafe --enable-norm-compat
if CC="c++ -std=c++11" ./configure --enable-unsafe --enable-norm-compat; then
    $make -s -j4 check-log || exit
fi
#CC="c++ -std=c++98" ./configure && \
#    make -s -j4 check-log || exit

# apt install gcc-arm-linux-gnueabihf gcc-7-multilib (with glibc, not newlib)
# and either:
#   apt-install linux-libc-dev (for asm/errno.h)
#   cd /usr/include/i386-linux-gnu; ln -s /usr/include/x86_64-linux-gnu/asm; cd -
# or:
#   cd /usr/include/i386-linux-gnu; ln -s /usr/include/asm-generic asm; cd -
if [ -e /usr/bin/arm-linux-gnueabihf-gcc ]; then
    echo "--enable-unsafe --enable-debug --host=arm-linux-gnueabihf"
    ./configure --enable-unsafe --enable-debug --host=arm-linux-gnueabihf && \
        make -s -j4 || exit;
    # $make -s -j4 check-log
    if [ ! -e /usr/arm-linux-gnueabihf/lib/libsafec-3.5.so.3 ]; then
        cd /usr/arm-linux-gnueabihf/lib/;
        sudo ln -s $OLDPWD/src/.libs/libsafec-3.5.so.3;
        cd -
    fi
    make -s -j4 -C tests tests;
    for t in tests/.libs/t*_s; do
        b=$(basename $t)
        qemu-arm -L /usr/arm-linux-gnueabihf $t | tee tests/$b.log
    done
fi

if [ -e /opt/pgi/linux86-64/2019/pgcc ]; then
    echo /opt/pgi/linux86-64/2019/bin/pgcc --enable-unsafe --enable-debug
    CC=/opt/pgi/linux86-64/2019/bin/pgcc ./configure --enable-unsafe --enable-debug && \
        make -s j4 && make check-log
        # fails on several not null slack wirh >RMAX
    make clean
fi

;;

MSYS_NT*)
# static, usually ours
echo "--disable-shared --enable-debug --enable-unsafe --enable-norm-compat"
./configure --disable-shared --enable-debug --enable-unsafe --enable-norm-compat && \
    make check-log || exit
# shared, might be the msvcrt overriding ours
echo "--enable-shared --enable-debug --enable-unsafe --enable-norm-compat"
./configure --enable-shared --enable-debug --enable-unsafe --enable-norm-compat && \
    make check-log || exit
# ensure we use the windows msvcrt sec_api
echo "-g -DTEST_MSVCRT --enable-shared --enable-debug --enable-unsafe"
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
./configure --disable-constraint-handler && \
    $make -s -j4 check-log || exit
./configure --disable-extensions && \
    $make -s -j4 check-log || exit
./configure --disable-wchar && \
    $make -s -j4 check-log || exit
./configure --enable-warn-dmax && \
    $make -s -j4 check-log || exit
echo configure --enable-error-dmax must fail
./configure --enable-error-dmax && \
    $make -s -j4 check-log && exit

# different .deps format
git clean -dxf src tests
autoreconf
if test -n "`which x86_64-w64-mingw32-gcc`"; then
    #CC="x86_64-w64-mingw32-gcc"
    test -f libssp-0.dll.m64 && cp tests/libssp-0.dll
    ./configure --enable-unsafe --host=x86_64-w64-mingw32 && \
    $make -s -j4 && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
            b=$(basename $t); wine $t | tee tests/$b.log; done
        rm *.dll
    fi
    git clean -dxf src tests
    autoreconf
fi
if test -n "`which i686-w64-mingw32-gcc`"; then
    test -f libssp-0.dll.m32 && cp tests/libssp-0.dll
    ./configure --enable-unsafe --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/t_*.exe; do
            b=$(basename $t .exe); wine $t | tee tests/$b.log;
        done
        rm *.dll
    fi
    $make clean
    CFLAGS="-g -gdwarf-2 -DTEST_MSVCRT" \
    ./configure --enable-unsafe --enable-debug --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    cp src/.libs/*.dll . && \
    for t in tests/t_*.exe; do
        b=$(basename $t .exe); wine $t | tee tests/$b.log;
    done
    $make clean
    git clean -dxf src tests
    autoreconf
fi
if test -n "`i386-mingw32-gcc`"; then
    #CC="i386-mingw32-gcc"
    test -f libssp-0.dll.m32 && cp tests/libssp-0.dll
    ./configure --enable-unsafe --host=i386-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ `uname` = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
          b=$(basename $t); wine $t | tee tests/$b.log; done
        rm *.dll
    fi
    $make clean
fi
git clean -dxf src tests
# if all clean, try out-of-tree build and distcheck
if [ -z "`git status --porcelain`" ]; then
    echo build from outside
    build-aux/autogen.sh
    mkdir .build && cd .build && \
        ../configure && $make check-log || exit
    cd ..
    rm -rf .build
    
    echo make distcheck
    build-aux/autogen.sh && \
        ./configure && $make distcheck
else
    echo "not clean srcdir, out-of-tree + make distcheck skipped"
    echo `git status --short`
fi
rm .slkm.ko.cmd .testslkm.ko.cmd  CaseFolding.txt test-upr.pl \
   tmpfopen tmpvwscanf tmpwscanf
rm -rf .tmp_versions/

autoreconf    
./configure --enable-unsafe --enable-debug && \
    $make -s -j4 check-log || exit
