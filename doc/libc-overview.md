# Overview of various libc's regarding the secure C11 extensions

C11 defined the optional secure extensions to be demanded by 
`__STDC_WANT_LIB_EXT1__`.

From the following tested libc implementations:

* glibc
* musl
* FreeBSD and DragonFly libc
* FreeBSD-derived darwin libc
* OpenBSD libc
* newlib
* dietlibc
* uClibc
* minilibc
* Microsoft Windows w/ secure API
* Android Bionic
* Embarcadero C++ libc

only the last 3 implement the secure C11 extensions:

* Microsoft Windows
* Android Bionic
* Embarcadero C++ libc

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

# C11 Annex K/safec caveats

* snprintf, vsnprintf, snwprintf, vsnwprintf, tmpnam_s:

  They are all considered unsafe. The 4 'n' truncating printf versions
  don't guarantee null-delimited destination buffers.

* `tmpnam_s` and `tmpnam` are racy.

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

## safeclib

* safeclib does not check optional NULL parameters to the `scanf_s`,
  `printf_s` functions. This would need tighter integration into the
  upstream libc.

* safeclib `fgets_s` permits temporary writes of `dmax+1` characters
  into dest.

* does not work fine with 2-byte wchar_t
  (i.e. cygwin/windows/solaris/aix), esp. the new case converters and
  `wcsnorm_s`. surrogate pairs are not yet supported everywhere.

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

* `vswscanf` is broken with a format string containing L"%%n"

* The following multibyte API's are missing, and can be defined like
  this:

```
    mbstate_t st;
    #define wctomb(dest, wc) wcrtomb((dest), (wc), &st)
    #define wcstombs(dest, src, len) wcsrtombs((dest), &(src), (len), &st)
    #define mbstowcs(dest, src, len) mbsrtowcs((dest), &(src), (len), &st)
```

## FreeBSD libc

* `vswscanf` is broken with a format string containing L"%%n"

* `mbstowcs` is broken with (NULL, '\0')

## musl

* `wmemcmp` returns not -1,0,1 but the full ptr diff.

* `mbtowc` and `wctomb` accept and convert illegal 4 byte characters
  in the ASCII locale to surrogate pairs, as it would be unicode.
  e.g. it converts `\xa0` to `\xdfa0`.

----

It's now 10 years after the secure libc extensions were designed, C11
adopted them, and still almost nobody implements them.  This library
was written 2008 under the MIT license, thanks Cisco.

Reini Urban 2017
