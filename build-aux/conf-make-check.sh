#!/bin/sh
test -f configure || build-aux/autogen.sh
./configure && make && make check-log
