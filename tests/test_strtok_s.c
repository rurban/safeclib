/*------------------------------------------------------------------
 * test_strtok_s
 * File 'str/strtok_s.c'
 * Lines executed:80.65% of 93
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRTOK_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_STRTOK_S)
#ifndef MINGW_HAS_SECURE_API
char *strtok_s(char *_Str, const char *_Delim, char **_Context);
#endif
#define strtok_s(dest, dmax, delim, ptr) strtok_s(dest, delim, ptr)
#endif

#define LEN (128)

int main(void) {
    char *p2str;
    char *p2tok;
    char *rc;

    rsize_t len;
    char str1[LEN];
    char str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    print_msvcrt(use_msvcrt);
    strcpy(str1, "aaaaaaaa");
    strcpy(str2, "fedcba");
    len = LEN;

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty ptr")
    rc = strtok_s(NULL, &len, str2, NULL);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERRPTR(NULL);
    /* msvcrt also doesn't reset errno */
    ERRNO_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty dmax")
    rc = strtok_s(str1, NULL, str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, 0);
#endif

    /*--------------------------------------------------*/

    len = 0;
    rc = strtok_s(str1, &len, str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, 0);

    /*--------------------------------------------------*/

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    /* EXPECT_BOS_TODO("dest overflow") */
    rc = strtok_s(str1, (len = LEN + 1, &len), str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(EOVERFLOW, 0);
#else
    rc = strtok_s(str1, (len = RSIZE_MAX_STR + 1, &len), str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESLEMAX, 0);
#endif
    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    len = LEN;
    EXPECT_BOS("empty delim")
    rc = strtok_s(str1, &len, NULL, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);

    len = 0;
    EXPECT_BOS("empty delim")
    rc = strtok_s(str1, &len, NULL, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, EINVAL);

    len = LEN;
    EXPECT_BOS("empty ptr")
    rc = strtok_s(str1, &len, str2, NULL);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    /* no token test */
    strcpy(str1, "aaaaaaaa");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);

    /*--------------------------------------------------*/

    strcpy(str1, "jklmnopqrst");
    len = strlen(str1) - 2; /* cheat on len */

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
        if (!use_msvcrt) { /* has no dmax arg */
            debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                         __FUNCTION__, __LINE__, p2tok, p2str);
            errs++;
        }
    }
    ERRNO_MSVC(ESUNTERM, 0);

    /*--------------------------------------------------*/

    strcpy(str1, "aaamnopqrst");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok == NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);

    /*--------------------------------------------------*/
    /** [1] **/

    strcpy(str1, "aaamnopqrstfedcba");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok == NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str);
        errs++;
    }
    if (strcmp(p2tok, "mnopqrst")) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/
    /** [2] **/

    p2tok = strtok_s(p2str, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/
    /** [1] **/
    /*debug_printf("test: %u \n", __LINE__);*/

    strcpy(str1, "?a???b,,,#c");
    len = strlen(str1);

    strcpy(str2, "?");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    ERRNO(EOK);

    if (strcmp(p2tok, "a")) {
        debug_printf("%s %u token -%s-  -%s- len=%d error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str, (int)len);
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/
    /** [2] **/
    strcpy(str1, "?a???b,,,#c");
    len = strlen(str1);

    strcpy(str2, ","); /* change the tokenizer string */

    /** p2tok = strtok_s(p2str, &len, str2, &p2str); **/
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    ERRNO(EOK);

    if (p2tok && strcmp(p2tok, "??b")) {
        debug_printf("%s %u token -%s-  -%s- len=%d error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str, (int)len);
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/

    strcpy(str1, ",,0,1,23,456,789,a,b,");
    len = strlen(str1);

    strcpy(str2, ","); /* change the tokenizer string */

    xdebug_printf("\n");
    xdebug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        ERRNO(EOK);

        xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);
    }
    /*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1);

    strcpy(str2, ",.;*");

    xdebug_printf("\n");
    xdebug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%s\" \n", str2);

    xdebug_printf("  p2tok = strtok_s(str1, &len, str2, &p2str); \n");
    p2tok = strtok_s(str1, &len, str2, &p2str);
    ERRNO(EOK);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");

    p2tok = strtok_s(NULL, &len, str2, &p2str);
    ERRNO(EOK);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");

    p2tok = strtok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    /* len is zero at this point */
    xdebug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("\n");

    /*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1);

    strcpy(str2, ",.;*");

    xdebug_printf("\n");
    xdebug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        ERRNO(EOK);

        xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len);
    }
    /*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1) - 1; /** back off the null **/

    strcpy(str2, ",.;*");

    xdebug_printf("\n");
    xdebug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str,
                          (int)len);
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
    /*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1) - 15; /** back off a few! **/

    strcpy(str2, ",.;*");

    xdebug_printf("\n");
    xdebug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        /* p2tok = strtok_s(p2str, &len, str2, &p2str); */
        p2tok = strtok_s(NULL, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            xdebug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str,
                          (int)len);
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
    /*--------------------------------------------------*/

    return (errs);
}
