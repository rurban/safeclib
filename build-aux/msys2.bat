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

echo Updating pacman...
bash -lc "pacman -S --needed --noconfirm pacman-mirrors"
rem bash -lc "pacman -Syyu --noconfirm"

rem echo Installing git...
rem bash -lc "pacman -S --needed --noconfirm git"

echo Installing common dependencies...
rem mingw-w64-x86_64-pdcurses and mingw-w64-x86_64-ncurses are in conflict...
rem bash -lc "pacman -Rddn --noconfirm mingw-w64-%MSYS2_ARCH%-ncurses mingw-w64-%MSYS2_ARCH%-termcap mingw-w64-i686-gcc-ada mingw-w64-i686-gcc-objc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-objc"
bash -lc "pacman -Syyu --needed --noconfirm mingw-w64-%MSYS2_ARCH%-toolchain mingw-w64-%MSYS2_ARCH%-libtool mingw-w64-%MSYS2_ARCH%-libiconv"
rem autoconf-2.69 automake-wrapper
