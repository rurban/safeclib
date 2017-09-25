#!/bin/sh

CC=gcc-mp-6 ./configure --enable-gcov=gcov-mp-6 --disable-shared \
                        --enable-unsafe --enable-norm-compat $@ \
    && gmake all \
    && gmake gcov

#build-tools/gcov-upd.pl cov.lst
