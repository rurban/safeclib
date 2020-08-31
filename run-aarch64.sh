#!/bin/sh
# ./configure --host=aarch64-linux-gnu --disable-shared --enable-debug
# echo to avoid libssp
# sed -i 's,-fstack-protector-strong -fstack-clash-protection,,' Makefile */Makefile
docker build --rm  -f Dockerfile-fedora-aarch64 -t aarch64 .
docker run --rm -t aarch64 $@
