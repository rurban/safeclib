#!/bin/bash
# bash, not sh: relies on brace expansion (clang-{19,18,...}) to walk
# every installed compiler minor version below.
cd "$(dirname "$0")/.." || exit
autoreconf
# shellcheck disable=SC2209  # intentional plain string, not command output; overridden to "gmake" on Darwin below
make=make

rm -rf src/*/.deps src/.deps tests/.deps 2>/dev/null

case $(uname) in
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
        b=$(basename "$t")
        qemu-arm -L /opt/local/arm-elf "$t" | tee tests/"$b".log
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
    if test -n "$(which clang)"; then
        echo clang -fsanitize=address -fno-omit-frame-pointer --enable-debug --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8
        CC="clang -fsanitize=address -fno-omit-frame-pointer" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8 && \
            make -s -j4 check-log || exit
        make -s clean
    fi
    for clang in clang clang-{19,18,17,16,15,14,13,12,11,10,7,5.0}
    do
        if test -n "$(which "$clang")"; then
            echo "$clang" -march=native --disable-constraint-handler --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8
            CC="$clang -march=native" \
              ./configure --disable-constraint-handler --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8 && \
                make -s -j4 check-log && make -s -j4 -C tests tests-bos
        fi
    done
    for clang in clang-{3.7,3.6,3.5,3.4}
    do
        if test -n "$(which "$clang")"; then
            echo "$clang" -std=c99 --enable-debug --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8
            if CC="#clang -std=c99" \
                 ./configure --enable-debug --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8; then
                make -s -j4 check-log || exit
            fi
            #    #TODO: valgrind broken with kpti
            #    #make -s -j4 check-valgrind
        fi
    done
    if test -n "$(which gcc-4.4)"; then
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
    for gcc in gcc-{15,14,13,12,11,10,9,8,7,6,5}
    do
        if test -n "$(which "$gcc")"; then
            if CC="$gcc" ./configure; then
                make -s -j4 check-log || exit
            fi
        fi
    done
    if test -n "$(which clang-5.0)"; then
        # since clang 5 with diagnose_if BOS compile-time checks, but on linux it is flappy
        if CC="clang-5.0" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat; then
            make -s -j4 check-log && make -s -j4 -C tests tests-bos
        fi
    fi
    if test -n "$(which clang)"; then
        if CC="clang -fsanitize=address,undefined -fno-omit-frame-pointer" \
          ./configure --enable-debug --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8; then
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
echo "--disable-wchar --disable-u8 -f Makefile.kernel"
./configure --disable-wchar --disable-u8 && \
    make -s -j4 -f Makefile.kernel || exit
make -s -j4 -f Makefile.kernel clean
make -s clean
git clean -dxf src tests
autoreconf
echo gcc gcov
if ./configure --enable-gcov --disable-shared --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8; then
    $make -s -j4 gcov
    #    perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov src/*.gcov && \
    #    gcov2perl src/*/*.gcov src/*.gcov && \
    #    cover -no-gcov
fi
make -s clean
rm tests/CaseFolding.txt.1 || true
echo c++ -std=c++11 --enable-unsafe --enable-norm-compat
if CC="c++ -std=c++11" ./configure --enable-unsafe --enable-norm-compat --enable-wchar --enable-u8; then
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
    ./configure --enable-unsafe --enable-debug --enable-u8 --host=arm-linux-gnueabihf && \
        make -s -j4 || exit;
    # $make -s -j4 check-log
    if [ ! -e /usr/arm-linux-gnueabihf/lib/libsafec-3.5.so.3 ]; then
        cd /usr/arm-linux-gnueabihf/lib/ || exit
        sudo ln -s "$OLDPWD"/src/.libs/libsafec-3.5.so.3
        cd - || exit
    fi
    make -s -j4 -C tests tests;
    for t in tests/.libs/t*_s; do
        b=$(basename "$t")
        qemu-arm -L /usr/arm-linux-gnueabihf "$t" | tee tests/"$b".log
    done
fi

if [ -e /opt/pgi/linux86-64/2019/pgcc ]; then
    echo /opt/pgi/linux86-64/2019/bin/pgcc --enable-unsafe --enable-u8 --enable-debug
    CC=/opt/pgi/linux86-64/2019/bin/pgcc ./configure --enable-unsafe --enable-u8 --enable-debug && \
        make -s j4 && make check-log
        # fails on several not null slack wirh >RMAX
    make clean
fi

# autoreconf/configure runs above generate real include/safe_types.h,
# safe_lib_errno.h and safe_config.h (AC_CONFIG_FILES, chmod read-only)
# for the *host* compiler. Those are quote-#include'd by the other
# include/*.h, so the preprocessor's same-directory lookup finds them
# before any -I search path -- including a freestanding cross build's
# own ${CMAKE_CURRENT_BINARY_DIR}/include generated by configure_file()
# below. Clear them so e.g. safe_types.h's HAVE_SYS_TYPES_H reflects
# sdcc/avr-gcc, not the host.
git clean -dxf include

# matches the CI "sdcc stm8" job: cmake + sdcc, no apt-get here, see
# build-aux/sdcc-stm8.cmake's comment for the packages needed
if { test -n "$(which sdcc 2>/dev/null)" || test -n "$(which sdcc-sdcc 2>/dev/null)"; } && \
   { test -n "$(which sdcc-ucsim_stm8 2>/dev/null)" || test -n "$(which ucsim_stm8 2>/dev/null)"; }; then
    for stm8opts in "build-stm8:-DENABLE_U8=OFF -DENABLE_EXTENSIONS=OFF" \
                    "build-stm8-u8:-DENABLE_U8=ON -DENABLE_EXTENSIONS=ON"; do
        d=${stm8opts%%:*}
        echo "sdcc stm8 $d"
        rm -rf "$d"
        # shellcheck disable=SC2086  # ${stm8opts#*:}: intentional word-split cmake -D... flags
        cmake -S . -B "$d" -DCMAKE_TOOLCHAIN_FILE=build-aux/sdcc-stm8.cmake \
            -DENABLE_MINIMAL=ON -DBUILD_SHARED_LIBS=OFF -DENABLE_WCHAR=OFF \
            ${stm8opts#*:} && \
        cmake --build "$d" && \
        sh build-aux/sdcc-stm8-test.sh "$d" || exit
        rm -rf "$d"
    done
fi

git clean -dxf include
# matches the CI "avr-gcc" job: cmake + avr-gcc/avr-libc, simulated with
# simavr+avr-gdb (Debian/Ubuntu packages only; not in Fedora's repos)
if test -n "$(grep -is ubuntu /etc/os-release 2>/dev/null)" && \
   test -n "$(which avr-gcc)" && test -n "$(which simavr)" && \
   test -n "$(which avr-gdb)"; then
    for avropts in "build-avr:-DENABLE_U8=OFF -DENABLE_EXTENSIONS=OFF" \
                   "build-avr-u8:-DENABLE_U8=ON -DENABLE_EXTENSIONS=ON"; do
        d=${avropts%%:*}
        echo "avr-gcc $d"
        rm -rf "$d"
        # shellcheck disable=SC2086  # ${avropts#*:}: intentional word-split cmake -D... flags
        cmake -B "$d" -DCMAKE_TOOLCHAIN_FILE=build-aux/avr-gcc.cmake \
            -DENABLE_MINIMAL=ON -DBUILD_SHARED_LIBS=OFF -DENABLE_WCHAR=OFF \
            ${avropts#*:} && \
        cmake --build "$d" && \
        sh build-aux/avr-sim-test.sh "$d" || exit
        rm -rf "$d"
    done
fi
git clean -dxf include

;;

MSYS_NT*)
# covers all 3 CI matrix entries (MINGW64, MINGW32, UCRT64, see
# .github/workflows/main.yml's "mingw" job): run this once per MSYS2
# shell, $MSYSTEM selects the active toolchain/libc via PATH.
echo "MSYSTEM=$MSYSTEM"
# static, usually ours
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
$make clean
if CC="cc -m32" ./configure; then
    $make -s -j4 check-log || exit
    $make clean
fi
./configure && \
    $make -s -j4 check-log || exit
OPTS="disable-nullslack disable-constraint-handler disable-extensions enable-wchar disable-u8 \
     disable-float disable-float-exp disable-long-long disable-long-double disable-printf-ptrdiff \
     disable-doc disable-hardening disable-shared enable-debug enable-unsafe enable-norm-compat \
     enable-gcov enable-memmax=262144 enable-strmax=2056 enable-warn-dmax"
for opt in $OPTS
do
    ./configure --"$opt" && \
        $make -s -j4 check-log || exit
done

$make clean
if [ -d .build-cmake ]; then rm -rf .build-cmake; fi
mkdir .build-cmake
cd .build-cmake || exit
echo cmake ..
cmake ..
make -s -j4 || exit
make -s -j4 test || exit
make clean
rm -f CMakeCache.txt
for opt in $OPTS
do
    def="$(echo "$opt" | sed -e's,disable,ENABLE,' | tr 'a-z-' 'A-Z_')"
    case "$opt" in
      disable*) bool="=OFF" ;;
      enable-*=*)
          if [ "$opt" = "enable-memmax=262144" ]; then
              def=RSIZE_MAX_MEM
              bool=262144
          else
              def=RSIZE_MAX_STR
              bool=2056
          fi ;;
      *) bool="=ON" ;;
    esac
    echo cmake -D"$def"$bool ..
    cmake -D"$def"$bool ..
    make -s -j4 test || exit
    make clean
done
cd ..

# different .deps format
git clean -dxf src tests
autoreconf
if test -n "$(which x86_64-w64-mingw32-gcc)"; then
    #CC="x86_64-w64-mingw32-gcc"
    test -f libssp-0.dll.m64 && cp libssp-0.dll.m64 tests/libssp-0.dll
    ./configure --enable-unsafe --enable-wchar --host=x86_64-w64-mingw32 && \
    $make -s -j4 && $make -s -j4 -C tests tests && \
    if [ "$(uname)" = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
            b=$(basename "$t"); wine "$t" | tee tests/"$b".log; done
        rm -- *.dll
    fi
    git clean -dxf src tests
    autoreconf
fi
if test -n "$(which i686-w64-mingw32-gcc)"; then
    test -f libssp-0.dll.m32 && cp libssp-0.dll.m32 tests/libssp-0.dll
    ./configure --enable-unsafe --enable-wchar --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ "$(uname)" = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/t_*.exe; do
            b=$(basename "$t" .exe); wine "$t" | tee tests/"$b".log;
        done
        rm -- *.dll
    fi
    $make clean
    CFLAGS="-g -gdwarf-2 -DTEST_MSVCRT" \
    ./configure --enable-unsafe --enable-debug --enable-wchar --host=i686-w64-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    cp src/.libs/*.dll . && \
    for t in tests/t_*.exe; do
        b=$(basename "$t" .exe); wine "$t" | tee tests/"$b".log;
    done
    $make clean
    git clean -dxf src tests
    autoreconf
fi
# UCRT64 (Fedora: ucrt64-gcc; MSYS2 itself runs the native UCRT64 build
# through the MSYS_NT* case above). Build only, no wine run: this is a
# cross-compiler smoke check matching the CI "mingw UCRT64" matrix
# entry's toolchain, not a full test pass like the native MSYS2 job.
if test -n "$(which x86_64-w64-mingw32ucrt-gcc)"; then
    ./configure --enable-unsafe --enable-wchar --host=x86_64-w64-mingw32ucrt && \
    $make -s -j4 || exit
    $make clean
    git clean -dxf src tests
    autoreconf
fi
if test -n "$(which i386-mingw32-gcc)"; then
    #CC="i386-mingw32-gcc"
    test -f libssp-0.dll.m32 && cp libssp-0.dll.m32 tests/libssp-0.dll
    ./configure --enable-unsafe --enable-wchar --host=i386-mingw32 && \
    $make -s -j4  && $make -s -j4 -C tests tests && \
    if [ "$(uname)" = Linux ]; then
        cp src/.libs/*.dll . && \
        for t in tests/.libs/t_*.exe; do
          b=$(basename "$t"); wine "$t" | tee tests/"$b".log; done
        rm -- *.dll
    fi
    $make clean
fi
git clean -dxf src tests
# if all clean, try out-of-tree build and distcheck
if [ -z "$(git status --porcelain)" ]; then
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
    git status --short
fi
rm .slkm.ko.cmd .testslkm.ko.cmd  CaseFolding.txt* test-upr.pl \
   tmpfopen tmpvwscanf tmpwscanf
rm -rf .tmp_versions/

autoreconf
./configure --enable-unsafe --enable-debug && \
    $make -s -j4 check-log || exit
