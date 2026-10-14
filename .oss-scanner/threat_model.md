# Threat model: safeclib

## What this project does

safeclib is a C library implementing the bounds-checking functions of ISO/IEC TR 24731-1
(Annex K: `memcpy_s`, `strcpy_s`, `strncat_s`, `sprintf_s`, `strtok_s`, ...), plus many
extensions (`wchar` variants, UTF-8 `u8` functions, `extmem`/`extstr` functions, string
search/compare helpers, optional Linux kernel module variant in `src/slkm`). It is a
drop-in hardening layer: callers pass a destination buffer, its size and the source, and the
library must **never write out of bounds, never read out of bounds, and never return
without setting the destination to a safe state on a constraint violation**.

Layout: `src/mem`, `src/str`, `src/wchar`, `src/extmem`, `src/extstr`, `src/extu8`,
`src/io`, `src/misc`, `src/os`; public headers in `include/`.

## Where untrusted input enters

The library has no I/O of its own beyond the `src/io` printf/scanf family. Treat as
attacker-controlled:

1. **Source buffers, source strings and format strings** passed to any function, in particular
   contents, embedded NULs, missing terminators, and lengths near `RSIZE_MAX_*`.
2. **Size/length arguments** (`dmax`, `smax`, `slen`, counts) that are attacker-influenced in the
   calling application, including values at or beyond `RSIZE_MAX`, `SIZE_MAX`, `0`, `INT_MAX`.
3. **Format strings and arguments** of `sprintf_s`, `snprintf_s`, `vsnprintf_s`, `sscanf_s`,
   `fscanf_s`, `fprintf_s` etc. (`%n` is rejected by design; other conversions must stay in bounds).
4. **Overlapping buffers** (`src` and `dest` overlapping must be detected where the standard
   requires it).
5. **Multibyte / UTF-8 / wide-char data** in `wchar` and `u8` functions: malformed, truncated or
   overlong sequences, unpaired surrogates, wide characters outside the valid range.

The caller's own program logic, the C library underneath (`libc`), the compiler and the
configure/build environment are trusted.

## What a real vulnerability looks like here

In scope:

- **Memory-safety bugs** in any library function given in-contract-or-violating arguments:
  out-of-bounds write (including one-byte/NUL overruns), out-of-bounds read of `dest`/`src`,
  use of uninitialised memory, integer overflow or wrap in length computations leading to a
  wrong copy size.
- **Constraint check bypass**: arguments that the standard or the function's documentation says
  must be rejected (overlap, `dmax` smaller than needed, `NULL`, size > `RSIZE_MAX`,
  unterminated source) but are accepted and processed.
- **Failure to sanitise on error**: on a runtime-constraint violation `dest` must be set to
  an empty string / zeroed as documented; leaving stale or partial secrets or unterminated
  output where the contract promises termination.
- **Secure-clearing failures**: `memset_s`, `memzero_s`, `memset16_s`, `memset32_s` etc. being
  optimised away or not clearing the full requested range.
- **Format-string handling bugs** in the `src/io` functions (`%n` acceptance, overread of
  arguments, buffer overrun on long conversions).
- **Constant-time violations** in functions documented or named as constant-time
  (e.g. `timingsafe_memcmp`, `memcmp_s` where documented) that leak content via early exit.
- **Undefined behaviour** (signed overflow, shift out of range, misaligned access, invalid
  pointer arithmetic) reachable from valid or constraint-violating arguments.
- Memory-safety bugs in the Linux-kernel-module variant (`src/slkm`, `Kbuild`, `Makefile.kernel`).

## Known design limits (do not report as vulnerabilities)

- Passing a `dmax` that is **larger than the real allocation** is the caller's bug; the library
  cannot know the true size. Not a vulnerability in safeclib.
- Passing invalid (dangling, non-`NULL` but unmapped) pointers; only `NULL` is detected.
- Behaviour on a constraint violation is determined by the installed handler
  (`set_str_constraint_handler_s`, `abort_handler_s`, `ignore_handler_s`). A handler that aborts
  is intended behaviour, not a DoS bug.
- Functions that are not part of the bounds-checked contract (e.g. the plain-C compatibility
  macros, `strcpy` replacements disabled by configuration) behave like libc.
- Non-constant-time behaviour in functions not documented as constant-time.
- Differences from the (withdrawn-from-default) C11 Annex K semantics where the documentation
  and ChangeLog describe the deviation (`--disable-*` configure options, `SAFECLIB_*`
  build-time limits).
- Issues only in `tests/`, `doc/`, `scripts/`, `build-aux/`, Dockerfiles and CI files.

## Components that matter most / least

Highest priority:

- `src/str`, `src/mem`, `src/wchar` (core Annex K surface)
- `src/io` (format-string handling)
- `src/extmem`, `src/extstr`, `src/extu8`, `src/extwchar`

Medium:

- `src/misc`, `src/os`, `src/slkm`, `include/`

Low / out of scope:

- `tests/`, `doc/`, `scripts/`, `build-aux/`, `m4/`, generated autotools files, CI configs.

## How to exercise it (inside the scanner image)

- `/src/build` – gcc `-O2 -g` build with `--enable-debug`; tests via `make -C /src/build check`.
- `/src/build-asan` – clang ASan+UBSan build. **Confirm memory-safety findings with it.** Write a
  small C reproducer and link against the static library, e.g.:
  `clang -g -fsanitize=address,undefined -I/src/include -I/src/build-asan/include repro.c /src/build-asan/src/.libs/libsafec-*.a`
  (adjust paths to the produced library names). `ASAN_OPTIONS` / `UBSAN_OPTIONS` are preset to
  abort on first error.
- Individual tests live in `/src/tests/test_*.c` and are built by `make check`.
- Remember that a violation invokes the constraint handler (aborts by default in some
  configurations): install `ignore_handler_s` via `set_str_constraint_handler_s` /
  `set_mem_constraint_handler_s` in reproducers so the post-violation state can be examined.

## How we rate severity

- **Critical**: out-of-bounds write with attacker-controlled content and length, reachable from a
  function called with a **valid, in-contract** argument set (the library corrupts memory although
  the caller did everything right).
- **High**: out-of-bounds write/read or use-after-free reachable with arguments that the library is
  documented to reject (constraint bypass); secure-clearing functions that fail to clear;
  `%n`-style write primitive in `src/io`.
- **Medium**: bounded out-of-bounds read (e.g. one byte past an unterminated source that the
  library claims to bound), failure to sanitise `dest` on error leaving secrets, non-constant-time
  leaks in functions documented as constant-time.
- **Low**: undefined behaviour without demonstrated impact, wrong return/errno code, uninitialised
  read with no visible effect, leaks on error paths.
- Bugs that need the caller to pass an incorrect true buffer size: informational.

## How we would like reports and patches

- Target the `master` branch of `github.com/rurban/safeclib`.
- Include a small standalone C reproducer, the exact compile command and the ASan/UBSan output
  from `/src/build-asan`.
- State which function(s), which argument values trigger it, and the first affected release if known.
- Patches: match the existing C style (`.clang-format`), minimal and focused, no unrelated
  cleanups; prefer fixing the constraint check at the top of the function. A regression test in
  `tests/test_<function>.c` (using the existing `ERR`, `EXPSTR`, `EXPNULL` macros) is appreciated.
- Add a ChangeLog entry for behaviour changes.

## Anything to leave alone

- Do not report the absence of Annex K in glibc, or that Annex K is deprecated/optional.
- Do not report compiler warnings, style issues or missing hardening flags in the test suite.
- Do not report issues that require the caller to pass wrong true buffer sizes.
