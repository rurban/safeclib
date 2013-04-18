# Kbuild - build file read by the Linux kernel build system
#
# Aug 2012, Jonathan Toppins
#
# Copyright (c) 2012 Cisco Systems
# All rights reserved.
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#

# Test module definition
obj-m     := slkm.o testslkm.o

ccflags-y := -I$(src)/include

# Functions in the kernel that don't have a 1-to-1 name correlation
# __HAVE_ARCH_STRLCPY
# __HAVE_ARCH_STRLCAT
# __HAVE_ARCH_STRNCMP
# __HAVE_ARCH_STRNICMP
# __HAVE_ARCH_STRNCASECMP
# __HAVE_ARCH_STRCHR
# __HAVE_ARCH_STRNCHR
# __HAVE_ARCH_STRRCHR
# __HAVE_ARCH_STRNSTR
# __HAVE_ARCH_STRLEN
# __HAVE_ARCH_STRSEP
# __HAVE_ARCH_MEMSCAN
# __HAVE_ARCH_MEMCHR


slkm-y     := src/slkm/slkm_init.o

# Internal Routines
slkm-y     += src/safeclib/mem_primitives_lib.o
slkm-y     += src/safeclib/safe_mem_constraint.o
slkm-y     += src/safeclib/safe_str_constraint.o
slkm-y     += src/safeclib/ignore_handler_s.o
slkm-y     += src/safeclib/abort_handler_s.o

# Memory Routines
slkm-y     += src/safeclib/memset_s.o
slkm-y     += src/safeclib/memcpy_s.o
slkm-y     += src/safeclib/memmove_s.o
slkm-y     += src/safeclib/memcmp_s.o

# String Routines
slkm-y     += src/safeclib/strcpy_s.o
slkm-y     += src/safeclib/strncpy_s.o
slkm-y     += src/safeclib/strcat_s.o
slkm-y     += src/safeclib/strncat_s.o
slkm-y     += src/safeclib/strcmp_s.o
slkm-y     += src/safeclib/strcasecmp_s.o
slkm-y     += src/safeclib/strstr_s.o
slkm-y     += src/safeclib/strnlen_s.o
slkm-y     += src/safeclib/strpbrk_s.o
slkm-y     += src/safeclib/strspn_s.o
slkm-y     += src/safeclib/strcspn_s.o

testslkm-y := tests/test_slkm.o
testslkm-y += tests/test_memset_s.o
testslkm-y += tests/test_memcpy_s.o
testslkm-y += tests/test_memmove_s.o
testslkm-y += tests/test_memcmp_s.o
testslkm-y += tests/test_strcpy_s.o
testslkm-y += tests/test_strncpy_s.o
testslkm-y += tests/test_strcat_s.o
testslkm-y += tests/test_strncat_s.o
testslkm-y += tests/test_strcmp_s.o
testslkm-y += tests/test_strcasecmp_s.o
testslkm-y += tests/test_strstr_s.o
testslkm-y += tests/test_strnlen_s.o
testslkm-y += tests/test_strpbrk_s.o
testslkm-y += tests/test_strspn_s.o
testslkm-y += tests/test_strcspn_s.o
