/*
 * unistd.h - the small subset of POSIX used by the tests, for the MSVC UCRT,
 * which has no <unistd.h>. Only added to the include path with MSVC.
 *
 * The POSIX names (unlink, read, write, close, dup, fileno, isatty) are
 * provided by <io.h> and <stdio.h> (with _CRT_NONSTDC_NO_WARNINGS), the
 * rest is mapped here.
 */
#ifndef SAFEC_TEST_MSVC_UNISTD_H
#define SAFEC_TEST_MSVC_UNISTD_H

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef intptr_t ssize_t;
#endif

/* returns 0 on success as POSIX, but takes a long. pipe is in test_private.h */
#define ftruncate(fd, len) _chsize((fd), (long)(len))

#endif /* SAFEC_TEST_MSVC_UNISTD_H */
