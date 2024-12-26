#!/bin/sh
test -f configure || build-aux/autogen.sh
./configure --enable-debug && make && make check-log
