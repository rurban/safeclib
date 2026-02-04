#!/bin/sh
# ./configure --disable-shared --enable-debug
# echo to avoid libssp
# sed -i 's,-fstack-protector-strong -fstack-clash-protection,,' Makefile */Makefile
docker build -f Dockerfile-centos --build-arg BASE_IMAGE=centos -t safeclib-centos .
docker run --rm -i -t safeclib-centos $@
