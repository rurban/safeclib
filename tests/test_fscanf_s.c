/*------------------------------------------------------------------
 * test_fscanf_s
 * File 'io/fscanf_s.c'
 * Lines executed:86.96% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_FSCANF_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN (128)

static char str1[LEN];
static char str2[LEN];
#define TMP "tmpscanf"
static FILE *stream = NULL;
void stuff_stream(const char *dest);
int test_fscanf_s(void);

void stuff_stream(const char *dest) {
    if (!stream)
        stream = fopen(TMP, "w+");
    else
        rewind(stream);
    fprintf(stream, "%s\n", dest);
    rewind(stream);
}

int test_fscanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty stream")
    rc = fscanf_s(NULL, "");
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);

    stuff_stream("1");
    EXPECT_BOS("empty fmt")
    rc = fscanf_s(stream, NULL);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /* TODO: should error */
#if 0
    stuff_stream("1");
    rc = fscanf_s(stream, "%s", NULL);
    ERREOF_MSVC(ESNULLP,EINVAL);
#endif

    /*--------------------------------------------------*/

    stuff_stream("      24");
    rc = fscanf_s(stream, "%s %%n", str2, LEN);
#ifdef BSD_LIKE
    if (rc != -1) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    }
#else
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
#endif

    stuff_stream("      24");
    rc = fscanf_s(stream, " %d", &len1);
    ERR(1);
    ERRNO(0);
    if ((int)len1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__,
                     (int)len1);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    stuff_stream(str1);

    rc = fscanf_s(stream, "%s", str2, LEN);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1)
    len2 = strlen(str2);
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        len1 = strlen(str1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n", __FUNCTION__,
                     __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

    /*--------------------------------------------------*/

    stuff_stream("keep it simple");

    rc = fscanf_s(stream, "%s", str2, LEN);
    ERR(1);
    EXPSTR(str2, "keep");

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    stuff_stream(str1);

    rc = fscanf_s(stream, "%s", str2);
    NOERR();
    EXPSTR(str2, str1);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    strcpy(str1, "12345678901234567890");

    rc = fscanf_s(stream, "%s", &str1[7]);
    ERR(1);
    EXPSTR(str1, "123456712345678901234567890");

    strcpy(str1, "123456789");

    rc = fscanf_s(stream, "%s", &str1[8]);
    ERR(1);
    EXPSTR(str1, "12345678123456789");
    */

    /*--------------------------------------------------*/

    stuff_stream("      24");
    rc = fscanf_s(stream, "%s %n", str2, LEN, &ind);
    ERREOF(EINVAL);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    stuff_stream(str1);

    rc = fscanf_s(stream, "%s", str2, LEN);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1); /* unspecified behaviour */
    /*EXPNULL(str2) TODO. Got "eep" */

    /*--------------------------------------------------*/

    fclose(stream);

#if 0
    strcpy(str1, "qqweqq");
    stuff_stream(str1);
    /* would block reading */
    rc = fscanf_s(stream, "%s", str2, LEN);
#endif

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

int main(void) { return (test_fscanf_s()); }
