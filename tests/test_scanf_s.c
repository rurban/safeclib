/*------------------------------------------------------------------
 * test_scanf_s
 * File 'io/scanf_s.c'
 * Lines executed:100.00% of 19
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef HAVE_SCANF_S
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
void stuff_stdin(const char *dest);
int test_scanf_s(void);

void stuff_stdin(const char *dest) {
    stream = fopen(TMP, "w+");
    fprintf(stream, "%s\n", dest);
    fclose(stream);
    stream = freopen(TMP, "r", stdin);
}

int test_scanf_s(void) {
    errno_t rc;
    int32_t ind;
    size_t len1;
    size_t len2;
    size_t len3;
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty fmt")
    rc = scanf_s(NULL, NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERREOF_MSVC(ESNULLP, EINVAL);
#endif

    /* TODO: should error, but just hangs in both */
#if 0
    rc = scanf_s("%s", NULL);
    ERREOF_MSVC(ESNULLP,EINVAL);
#endif

    /*--------------------------------------------------*/

    stuff_stdin("      24");
    rc = scanf_s("%s %%n", str2, LEN);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fscanf(\"\",L\"%%n\"): %d\n", __FUNCTION__,
               __LINE__, (int)rc);
    } else
#endif
        if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);

    stuff_stdin("      24");
    rc = scanf_s("%s %%n", str2, 6);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
    EXPSTR(str2, "24");

    /*stuff_stdin("      24");
    rc = scanf_s("%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");*/

    stuff_stdin("      24");
    rc = scanf_s(" %d", &len1);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    ERRNO(0);
    if ((int)len1 != 24) {
        debug_printf("%s %u wrong arg: %d\n", __FUNCTION__, __LINE__,
                     (int)len1);
        errs++;
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
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

    strcpy(str1, "keep it simple");
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    if (rc != 1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(1);
    /* flapping test! */
    /*EXPSTR(str2, "keep");*/

    /*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");
    stuff_stdin(str1);

    rc = scanf_s("%s", str2);
    NOERR()
    EXPSTR(str2, str1);

    /*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    strcpy(str1, "12345678901234567890");

    rc = scanf_s("%s", &str1[7]);
    ERR(1);
    EXPSTR(str1, "123456712345678901234567890");

    strcpy(str1, "123456789");

    rc = scanf_s("%s", &str1[8]);
    ERR(1);
    EXPSTR(str1, "12345678123456789");
    */

    /*--------------------------------------------------*/

    stuff_stdin("      24");
    rc = scanf_s("%s %n", str2, LEN, &ind);
    ERREOF(EINVAL);

    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    if (rc == -1) { /* flapping test */
        ERR(-1);
        EXPNULL(str2);
    } else if (!use_msvcrt) {
        ERR(1);
    } /* else wine returns 0 */

    /*--------------------------------------------------*/

    str1[0] = '\0';
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    if (rc != -1) {
        printf("flapping tests - abort\n");
        return errs;
    }
    ERR(-1)

    /*--------------------------------------------------*/

#ifdef __linux
    debug_printf("%s %u skip test reading from closed stream\n", __FUNCTION__,
                 __LINE__);
#else
    fclose(stream); /* crashes with linux glibc or musl: double free */

    strcpy(str1, "qqweqq");
    stuff_stdin(str1);
    /* would block reading */
    rc = scanf_s("%s", str2, LEN);
#endif

    /*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_scanf_s()); }
#endif
