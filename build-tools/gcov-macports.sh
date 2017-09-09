#!/bin/sh

CC=gcc-mp-6 ./configure --enable-unsafe --enable-gcov=gcov-mp-6 --disable-shared \
    && gmake all \
    && gmake gcov 2> cov.lst

build-tools/gcov-upd.pl cov.lst
