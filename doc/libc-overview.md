# Overview of various libc's regarding the secure C11 extensions

C11 defined the optional secure extensions to be demanded by 
`__STDC_WANT_LIB_EXT1__`.

From the following tested libc implementations:

* glibc
* musl
* FreeBSD and DragonFly libc
* FreeBSD-derived darwin libc
* OpenBSD libc
* newlib (Cygwin)
* dietlibc
* uClibc
* minilibc
* Microsoft Windows under wine
* Microsoft Windows msvcrt and ulibc w/ secure API
* Open Watcom
* Android Bionic
* Huawei securec
* Embarcadero C++ libc
* slibc

only the last 6 implement the secure C11 extensions:

* Microsoft Windows
* Open Watcom since 1.5
* Android Bionic w/ stlport
* Huawei securec
* Embarcadero C++ libc
* slibc

# General quirks

Generally most libc's use a 4 byte wchar_t (which resembles UTF-32),
but windows/cygwin/solaris/aix use 2-byte (UTF-16), therefore they need
to support surrogate pairs.

The wide sprintf variants do not allow a NULL buffer argument to
return the size of the resulting buffer. If the initial buffer is too
small, you need to realloc and redo.

There's still no locale-independent utf8 support, not even in C11 with
its new `u8""` type.

Many wchar and mb conversions and searches are locale-dependent, hence
unusable for utf8.

Nobody implements a proper wchar case-conversion, foldcase and
normalization API. ICU or libunistring are way too heavy for this
simple problem. There's only utf8proc for utf8 encoded strings (now
with julia).

See also http://crashcourse.housegordon.org/coreutils-multibyte-support.html

None of the other libc's (and neither most crypto libraries) provide a secure
memory barrier for memset/memzero/memset_s/explicit_bzero/SecureZeroMemory/...,
they only provide a compiler barrier against false compiler optimizations. They
don't reliably sync memory stores with possibly re-ordered loads by modern
out-of-order CPU's. Only the linux kernel and safeclib do so.

# C11 Annex K/safec caveats

* `tmpnam_s`:

  Is considered unsafe. `tmpnam_s` and `tmpnam` are racy.

* `sprintf_s` and `vsprintf_s` retval on errors.

  They were revised by the author from Microsoft in
  [n1141](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1141.pdf)
  to return `0` on most errors (only `-1` on encoding errors), to allow
  the often used `count += sprintf(buf + count, format_string, args);`
  when no encoding errors could occur, ignoring `retval < 0`
  checks. Lateron Microsoft and all other implementors
  (e.g. Embacadero, safeclib) revised this decision to be consistent,
  to return `-1` on all errors, just the standard still contains the
  **n1141** revision.

## Microsoft Windows/MINGW_HAS_SECURE_API

* `fopen_s`, `freopen_s` deviate in the API: restrict is missing.

* `strtok_s`, `wcstok_s`,`vsnprintf_s` miss the dmax argument.

* `vsnprintf_s` adds a maxarg argument.

* `vswprintf` adds a maxarg argument on w32. (with `__STRICT_ANSI__`
  undefined)

* no `strnlen` on mingw32.

* no `errno_t` return type for `qsort_s`, only `void`.

* reversed argument order for `localtime_s` and `gmtime_s`.

* older mingw versions have `wchar.h` with only 2 functions:
  `wcscmp`, `wcslen`

* no `RSIZE_MAX`

* `memmove_s` does not clear dest with ERANGE when `count > dmax` and EINVAL when
  src is a NULL pointer.

* `vsprintf_s`, `sprintf_s` return `-1` on all errors, not just encoding errors.
  (Wrong standard)

* With `wcsrtombs` (used by `wcsrtomb_s`) the `*retval` result
  includes the terminating zero, i.e. the result is `+1` from the
  spec.

## safeclib

* safeclib does not check optional NULL parameters to the vararg
  `scanf_s` and `printf_s` functions. This would need tighter
  integration into the upstream libc.
  Similarily the 2nd size parameter for `%s`, `%c` and `%[`
  is not implemented.

* safeclib `fgets_s` permits temporary writes of `dmax+1` characters
  into dest.

* `vsprintf_s`, `sprintf_s` return `-1` on all errors, not just encoding errors.
  (Wrong standard)

## Android FORTIFY and _STLP_USE_SAFE_STRING_FUNCTIONS

Not yet tested. Hard to find as open source.  Apparently once
implemented as part of the *stlport* library, but unused and I cannot
find it in Bionic (orea), which is mostly an improved FreeBSD libc.
*stlport* had a portable rewrite of the secure Windows API, written in 1999.
Now they use just the fortified POSIX API,
e.g. for `strncpy_s` `strncpy_chk` and `__strncpy_chk2` with known src size.

See [Wikipedia: Bionic Fortify_source](https://en.wikipedia.org/wiki/Bionic_(software)#Fortify_source),
and their blog post [FORTIFY in Android](https://android-developers.googleblog.com/2017/04/fortify-in-android.html).

Basically they use a `__bos()` or `__builtin_object_size` macro which
is a better `sizeof` and expands to the size of the compile-time
pointer when the size of the buffer is known at compile-time. They
also try to use the `alloc_size` extension which looks at a malloc'ed
pointer into the previous word for its size.  So there's no secure API,
just the normal POSIX and glibc API with compile-time `_chk` checks as
in glibc with FORTIFY. Just a bit better than glibc.

## Huawei securec

This is similar to the secure Android and slibc variant, written in C
and it is a proper secure libc.
Their license is restrictive though:
Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.

Available at https://github.com/UVP-Tools/UVP-Tools/tree/master/uvp-monitor/securec
There are other variants under the GPL-2 and Apache 2.0 on github
(e.g. wangguitao/intelligent-protector)

## Open Watcom 1.5

Tested by
http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1967.htm#alternatives
It sets the `__STDC_LIB_EXT1__` macro to `200509L` and can be
considered a nearly conforming implementation.

## slibc

"Slibc is a complete, open source implementation of Annex K designed
to be used with GNU C library typically distributed with Linux. The
implementation claims to be complete and to fully conform to C11. An
inspection of the implementation reveals that it is quite inefficient
and thus unsuitable for production use without considerable
changes. It does provide a good referefence implementation of the
library. A proposal to incorporate slibc into the GNU C library was
submitted in 2012 to the GNU C library community and rejected."
- http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1967.htm#alternatives

Available at https://code.google.com/archive/p/slibc/

# Other caveats

## glibc

* SEGV with `freopen(NULL, "rb", stdin)` with asan on some systems,
  calling an invalid `strlen()` on NULL.

* quirky declaration of various standards, which conflict with each other.
  
  glibc needs the correct standard to include some extensions
  when we declare the standard by ourselves.
  e.g. clang-4.0 -std=c99 misses several reentrant versions.
  when defining `_XOPEN_SOURCE 700` to define `strnlen` and the reentrant
  time versions, we need also `__STDC_WANT_LIB_EXT1__ 1` for `errno_t`,
  and this would break several other older struct members, such as `tm_gmtoff`

## newlib

* `vswscanf` is broken with a format string containing `L"%%n"`

* The following multibyte API's are missing, and can be defined like
  this:

```
    mbstate_t st;
    #define wctomb(dest, wc) wcrtomb((dest), (wc), &st)
    #define wcstombs(dest, src, len) wcsrtombs((dest), &(src), (len), &st)
    #define mbstowcs(dest, src, len) mbsrtowcs((dest), &(src), (len), &st)
```

## FreeBSD libc

* `vswscanf` is broken with a format string containing `L"%%n"`

* `mbstowcs` is broken with `(NULL, '\0')`

## musl

* `wmemcmp` returns not `-1`, `0` or `1` but the full ptr diff.

* `mbtowc` and `wctomb` accept and convert illegal 4 byte characters
  in the ASCII locale to surrogate pairs, as it would be unicode.
  e.g. it converts `\xa0` to `\xdfa0`.

## wine

As of wine-2.0.4 its libc has several more errors than the msvcrt sec_api:

* `asctime_s` with `tm->mday=0` returns not `EINVAL` but `0`.
* `wcsncat_s(dest, dmax, src, 0)` returns not `EINVAL` but `0`.
* `wcsncat_s(NULL, 0, src, 0);` returns not `0` but `EINVAL`.
* more `wcsncat_s`: ESUNTERM and ESOVRLP do not clear dest
* `wcsrtombs_s(&ind, dest, 0, &cs, 0, &ps)`  returns not `EINVAL` but `0`,
  with `ind` kept at `0`.

----

It's now 10 years after the secure libc extensions were designed, C11
adopted them, and still almost nobody implements them. Only for
compile-time known sizes the FORTIFY `_chk` extension secures against
overflows, but not against dynamically allocated buffers. This library
was written 2008 under the MIT license, thanks Cisco.

Reini Urban 2018
