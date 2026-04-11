/*------------------------------------------------------------------
 * test_uni.h - shared helpers for Unicode cross-check tests
 *
 * Provides a runtime check that perl's Unicode::UCD::UnicodeVersion()
 * matches SAFECLIB_UNICODE_VERSION (== TOWCTRANS_UNICODE_VERSION),
 * so PERL_TEST cross-checks only run when the versions align.
 *------------------------------------------------------------------
 */

#ifndef TEST_UNI_H
#define TEST_UNI_H

#include <stdio.h>
#include <stdlib.h>

/* Query perl's Unicode::UCD::UnicodeVersion() and return the major
   version as an integer, or 0 if perl is unavailable or the query
   fails. */
static inline int perl_unicode_major_version(void) {
    FILE *pipe_fp;
    int major = 0;
    char buf[32];

    pipe_fp = popen(PERL " -MUnicode::UCD -e'print "
                         "Unicode::UCD::UnicodeVersion()' 2>/dev/null",
                    "r");
    if (!pipe_fp)
        return 0;
    if (fgets(buf, sizeof(buf), pipe_fp))
        major = atoi(buf);
    pclose(pipe_fp);
    return major;
}

/* Return 1 if perl's Unicode major version matches safeclib's
   SAFECLIB_UNICODE_VERSION, else 0. On mismatch, prints a skip
   notice so the test log records why PERL_TEST was skipped. */
static inline int perl_unicode_version_matches(void) {
    int major = perl_unicode_major_version();
    if (major == SAFECLIB_UNICODE_VERSION)
        return 1;
    printf("skipping PERL_TEST: perl Unicode %d != safeclib %d\n",
           major, SAFECLIB_UNICODE_VERSION);
    return 0;
}

#endif /* TEST_UNI_H */
