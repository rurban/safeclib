@echo off

cd %APPVEYOR_BUILD_FOLDER%
set MSYS2_DIR=msys64

echo Compiler: %COMPILER%
echo Architecture: %MSYS2_ARCH%
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%

echo Start updating build dependencies...

echo Installation MSYS2 build preconditions...

echo Extending path to MSYS...
SET "PATH=C:\%MSYS2_DIR%\%MSYSTEM%\bin;C:\%MSYS2_DIR%\usr\bin;%PATH%"

echo Temporary keyring mess (https://www.msys2.org/news/)
bash -lc "curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz"
bash -lc "curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig"
bash -lc "pacman-key --verify msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig"
bash -lc "pacman -U --noconfirm msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz"
bash -lc "pacman -U --config <(echo) msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz"

echo Updating pacman...
bash -lc "pacman -Syy --noconfirm pacman"
bash -lc "pacman -S --needed --noconfirm pacman-mirrors"
bash -lc "pacman -Syyu --noconfirm"
rem need it twice, really
bash -lc "pacman -Syyu --noconfirm"

rem echo Installing git...
rem bash -lc "pacman -S --needed --noconfirm git"

echo Installing dependencies...
rem mingw-w64-x86_64-pdcurses and mingw-w64-x86_64-ncurses are in conflict...
rem bash -lc "pacman -Rddn --noconfirm mingw-w64-%MSYS2_ARCH%-ncurses mingw-w64-%MSYS2_ARCH%-termcap mingw-w64-i686-gcc-ada mingw-w64-i686-gcc-objc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-objc"
bash -lc "pacman -Syyu --needed --noconfirm mingw-w64-%MSYS2_ARCH%-toolchain mingw-w64-%MSYS2_ARCH%-libtool mingw-w64-%MSYS2_ARCH%-libiconv"
rem autoconf-2.69 automake-wrapper
