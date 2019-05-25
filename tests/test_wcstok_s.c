/*------------------------------------------------------------------
 * test_wcstok_s
 * File 'wchar/wcstok_s.c'
 * Lines executed:84.21% of 95
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSTOK_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_WCSTOK_S)
#ifndef MINGW_HAS_SECURE_API
wchar_t *wcstok_s(wchar_t *_Str, const wchar_t *_Delim, wchar_t **_Context);
#endif
#define wcstok_s(dest, dmax, delim, ptr) wcstok_s(dest, delim, ptr)
#endif

#define LEN (128)

int main(void) {
    wchar_t *p2str = NULL;
    wchar_t *p2tok;
    wchar_t *rc;

    rsize_t len;
    wchar_t str1[LEN];
    wchar_t str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);
    wcscpy(str1, L"aaaaaaaa");
    wcscpy(str2, L"fedcba");
    len = LEN;

    rc = wcstok_s(NULL, &len, str2, &p2str);
    init_msvcrt(errno == ESNULLP, &use_msvcrt);
    ERRPTR(NULL);
    /* msvcrt also doesn't reset errno */
    ERRNO_MSVC(ESNULLP, EINVAL);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dmax")
    rc = wcstok_s(str1, NULL, str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, 0);
#endif

    /*--------------------------------------------------*/

    /* EXPECT_BOS("empty dmax") */
    rc = wcstok_s(str1, (len = 0, &len), str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, 0);

    /*--------------------------------------------------*/

    /* EXPECT_BOS("buf overflow") */
    rc = wcstok_s(str1, (len = RSIZE_MAX_WSTR + 1, &len), str2, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESLEMAX, 0);

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    len = LEN;
    EXPECT_BOS("empty delim")
    rc = wcstok_s(str1, &len, NULL, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);

    len = 0;
    EXPECT_BOS("empty delim")
    rc = wcstok_s(str1, &len, NULL, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, EINVAL);

    len = LEN;
    EXPECT_BOS("empty ptr")
    rc = wcstok_s(str1, &len, str2, NULL);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);
#endif

    /*--------------------------------------------------*/

    /* no token test */
    wcscpy(str1, L"aaaaaaaa");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls- Error \n",
                     __FUNCTION__, __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);

    /*--------------------------------------------------*/

    wcscpy(str1, L"jklmnopqrst");
    len = wcslen(str1) - 2; /* cheat on len */

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (p2tok) {
        if (!use_msvcrt) { /* has no dmax arg */
            debug_printf("%s %u token -%ls-  remaining -%ls- Error \n",
                         __FUNCTION__, __LINE__, p2tok, p2str);
            errs++;
        }
    }
    ERRNO_MSVC(ESUNTERM, 0);

    /*--------------------------------------------------*/

    wcscpy(str1, L"aaamnopqrst");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (!p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls- Error \n",
                     __FUNCTION__, __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);

    /*--------------------------------------------------*/
    /** [1] **/

    wcscpy(str1, L"aaamnopqrstfedcba");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (!p2tok) {
        debug_printf("%s %u token -null-  remaining -%ls- Error \n",
                     __FUNCTION__, __LINE__, p2str);
        errs++;
    } else if (wcscmp(p2tok, L"mnopqrst")) {
        debug_printf("%s %u token -%ls-  remaining -%ls- Error \n",
                     __FUNCTION__, __LINE__, p2tok, p2str);
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */
    ERRNO(EOK);

    /*--------------------------------------------------*/
    /** [2] **/

    p2tok = wcstok_s(p2str, &len, str2, &p2str);
    if (p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls- Error \n",
                     __FUNCTION__, __LINE__, p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/
    /** [1] **/

    wcscpy(str1, L"?a???b,,,#c");
    len = wcslen(str1);

    wcscpy(str2, L"?");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    ERRNO(EOK);

    if (wcscmp(p2tok, L"a")) {
        debug_printf("%s %u token -%ls-  -%ls- len=%d Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str, (int)len);
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/
    /** [2] **/
    wcscpy(str1, L"?a???b,,,#c");
    len = wcslen(str1);

    wcscpy(str2, L","); /* change the tokenizer string */

    /** p2tok = wcstok_s(p2str, &len, str2, &p2str); **/
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    ERRNO(EOK);

    if (wcscmp(p2tok, L"??b")) {
        debug_printf("%s %u token -%ls-  -%ls- len=%d Error \n", __FUNCTION__,
                     __LINE__, p2tok, p2str, (int)len);
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

    /*--------------------------------------------------*/

    wcscpy(str1, L",,0,1,23,456,789,a,b,");
    len = wcslen(str1);

    wcscpy(str2, L","); /* change the tokenizer string */

    xdebug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {

        xdebug_printf("%u  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n",
                      __LINE__);
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        if (p2tok) {
            xdebug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok,
                          p2str, (int)len);
        }
        ERRNO(EOK);
    }
    /*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1);

    wcscpy(str2, L",.;*");

    xdebug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    xdebug_printf("%u  p2tok = wcstok_s(str1, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(str1, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len);

    xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len);

    /*xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
    __LINE__); p2tok = wcstok_s(NULL, &len, str2, &p2str);
    xdebug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );*/

    /* len is zero at this point */
    xdebug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n",
                  __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    if (p2tok) {
        xdebug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok,
                      p2str, (int)len);
    }

    ERRNO(EOK);
    xdebug_printf("\n");

    /*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1);

    wcscpy(str2, L",.;*");

    xdebug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        ERRNO(EOK);
        if (p2tok) {
            xdebug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok,
                          p2str, (int)len);
        }
    }
    /*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1) - 1; /** back off the null **/

    wcscpy(str2, L",.;*");

    xdebug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            xdebug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok,
                          p2str, (int)len);
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
    /*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1) - 15; /** back off a few! **/

    wcscpy(str2, L",.;*");

    xdebug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1,
                  (unsigned)len);
    xdebug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        xdebug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        /* p2tok = wcstok_s(p2str, &len, str2, &p2str); */
        p2tok = wcstok_s(NULL, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            xdebug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok,
                          p2str, (int)len);
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
    /*--------------------------------------------------*/

    return (errs);
}
