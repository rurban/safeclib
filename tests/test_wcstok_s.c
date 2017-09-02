/*------------------------------------------------------------------
 * test_wcstok_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

int main()
{
#ifdef DEBUG
    errno_t rc = 0;
#endif
    wchar_t *p2str;
    wchar_t *p2tok;

    rsize_t  len;
    wchar_t  str1[LEN];
    wchar_t  str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    p2tok = wcstok_s(str1, NULL, str2,  &p2str);
    if (p2tok) {
        debug_printf("%s %u   Error rc=%d \n",
               __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    len = 0;
    p2tok = wcstok_s(str1, &len, str2,  &p2str);
    if (p2tok) {
        debug_printf("%s %u   Error rc=%d \n",
               __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    len = RSIZE_MAX_STR + 1;
    p2tok = wcstok_s(str1, &len, str2,  &p2str);
    if (p2tok) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    len = 0;
    p2tok = wcstok_s(str1, &len, NULL,  &p2str);
    if (p2tok) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    p2tok = wcstok_s(str1, &len, str2,  NULL);
    if (p2tok) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    /* no token test */
    wcscpy(str1, L"aaaaaaaa");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    wcscpy(str1, L"jklmnopqrst");
    len = wcslen(str1) - 2;             /* cheat on len */

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("line:%u \n", __LINE__);

    wcscpy(str1, L"aaamnopqrst");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (!p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
/** [1] **/
    debug_printf("line:%u \n", __LINE__);

    wcscpy(str1, L"aaamnopqrstfedcba");
    len = wcslen(str1);

    wcscpy(str2, L"fedcba");

    p2tok = wcstok_s(str1, &len, str2, &p2str);
    if (!p2tok) {
        debug_printf("%s %u token -null-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__, p2str);
        errs++;
    } else if (wcscmp(p2tok, L"mnopqrst")) {
        debug_printf("%s %u token -%ls-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [2] **/

    p2tok = wcstok_s(p2str, &len, str2, &p2str);
    if (p2tok) {
        debug_printf("%s %u token -%ls-  remaining -%ls-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [1] **/
    debug_printf("line:%u \n", __LINE__);

    wcscpy(str1, L"?a???b,,,#c");
    len = wcslen(str1);

    wcscpy(str2, L"?");

    p2tok = wcstok_s(str1, &len, str2, &p2str);

    if (wcscmp(p2tok, L"a") ) {
        debug_printf("%s %u token -%ls-  -%ls- len=%d \n",
               __FUNCTION__, __LINE__, p2tok, p2str, (int)len );
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [2] **/
    debug_printf("line:%u \n", __LINE__);
    wcscpy(str1, L"?a???b,,,#c");
    len = wcslen(str1);

    wcscpy(str2, L",");   /* change the tokenizer string */

    /** p2tok = wcstok_s(p2str, &len, str2, &p2str); **/
    p2tok = wcstok_s(NULL, &len, str2, &p2str);

    if (wcscmp(p2tok, L"??b") ) {
        debug_printf("%s %u token -%ls-  -%ls- len=%d \n",
               __FUNCTION__, __LINE__, p2tok, p2str, (int)len );
        errs++;
    }
    /* printf("token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/

    wcscpy(str1, L",,0,1,23,456,789,a,b,");
    len = wcslen(str1);

    wcscpy(str2, L",");   /* change the tokenizer string */

    debug_printf("line:%u\n", __LINE__);
    debug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {

        debug_printf("%u  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n", __LINE__);
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        if (p2tok)
            debug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__,
                         p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1);

    wcscpy(str2, L",.;*");

    debug_printf("line:%u\n", __LINE__);
    debug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    debug_printf("%u  p2tok = wcstok_s(str1, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(str1, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );

    /*debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%ls-  -%ls- len=%d \n", p2tok, p2str, (int)len );*/

    /* len is zero at this point */
    debug_printf("%u  p2tok = wcstok_s(NULL, &len, str2, &p2str); \n", __LINE__);
    p2tok = wcstok_s(NULL, &len, str2, &p2str);
    if (p2tok)
        debug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__, p2tok, p2str, (int)len );

    debug_printf("\n");

/*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1);

    wcscpy(str2, L",.;*");

    debug_printf("line:%u\n", __LINE__);
    debug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        if (p2tok)
            debug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__,
                         p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1) - 1;     /** back off the null **/

    wcscpy(str2, L",.;*");

    debug_printf("line:%u\n", __LINE__);
    debug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        p2tok = wcstok_s(p2str, &len, str2, &p2str);
        if (p2tok)
            debug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__,
                         p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    wcscpy(str1, L",.*;one,two;three,;four*.*.five-six***");

    len = wcslen(str1) - 15;     /** back off a few! **/

    wcscpy(str2, L",.;*");

    debug_printf("line:%u\n", __LINE__);
    debug_printf("String to tokenize str1 = \"%ls\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%ls\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = wcstok_s(p2str, &len, str2, &p2str); \n");
        /* p2tok = wcstok_s(p2str, &len, str2, &p2str); */
        p2tok = wcstok_s(NULL, &len, str2, &p2str);
        if (p2tok)
            debug_printf("%u  token -%ls-  -%ls- len=%d \n", __LINE__,
                         p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    return (errs);
}
