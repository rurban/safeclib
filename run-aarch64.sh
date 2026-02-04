#!/bin/sh
# ./configure --host=aarch64-linux-gnu --disable-shared --enable-debug --disable-doc
# test_sprintf_s_exc.o: Relocations in generic ELF (EM: 62):
# make -sj CFLAGS="-g -O2 -fPIC"
# echo to avoid libssp:
# sed -i 's,-fstack-protector-strong -fstack-clash-protection,,' Makefile */Makefile
docker build -f Dockerfile-aarch64 -t safeclib-aarch64 .
docker run --rm -it safeclib-aarch64 $@
