/*------------------------------------------------------------------
 * test_strtok_s
 * File 'str/strtok_s.c'
 * Lines executed:82.56% of 86
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define HAVE_NATIVE defined(HAVE_STRTOK_S)
#include "test_msvcrt.h"

#if defined(_WIN32) && defined(HAVE_STRTOK_S)
# ifndef MINGW_HAS_SECURE_API
  char* strtok_s(char *_Str, const char *_Delim, char **_Context);
# endif
# define strtok_s(dest, dmax, delim, ptr) strtok_s(dest, delim, ptr)
#endif

#define LEN   ( 128 )

int main()
{
    char *p2str;
    char *p2tok;
    char *rc;

    rsize_t len;
    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

#ifdef USE_MSVCRT
    strcpy(str1, "aaaaaaaa");
    strcpy(str2, "fedcba");
#endif
    len = LEN;

    rc = strtok_s(NULL, &len, str2, NULL);
    ERRPTR(NULL);
    /* msvcrt also doesn't reset errno */
    ERRNO_MSVC(ESNULLP, EINVAL);

    rc = strtok_s(str1, NULL, str2,  &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, 0);

/*--------------------------------------------------*/

    len = 0;
    rc = strtok_s(str1, &len, str2,  &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, 0);

/*--------------------------------------------------*/

    len = RSIZE_MAX_STR + 1;
    rc = strtok_s(str1, &len, str2,  &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESLEMAX, 0);

/*--------------------------------------------------*/

    len = LEN;
    rc = strtok_s(str1, &len, NULL,  &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);

    len = 0;
    rc = strtok_s(str1, &len, NULL, &p2str);
    ERRPTR(NULL);
    ERRNO_MSVC(ESZEROL, EINVAL);

    len = LEN;
    rc = strtok_s(str1, &len, str2, NULL);
    ERRPTR(NULL);
    ERRNO_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    /* no token test */
    strcpy(str1, "aaaaaaaa");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);

/*--------------------------------------------------*/

    strcpy(str1, "jklmnopqrst");
    len = strlen(str1) - 2;             /* cheat on len */

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
#ifndef USE_MSVCRT /* has no dmax arg */
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
#endif
    }
    ERRNO_MSVC(ESUNTERM, 0);

/*--------------------------------------------------*/

    strcpy(str1, "aaamnopqrst");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok == NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
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
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    if (strcmp(p2tok, "mnopqrst")) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    ERRNO(EOK);
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [2] **/

    p2tok = strtok_s(p2str, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s- Error \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
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

    if (strcmp(p2tok, "a") ) {
        debug_printf("%s %u token -%s-  -%s- len=%d error \n",
               __FUNCTION__, __LINE__, p2tok, p2str, (int)len );
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [2] **/
    strcpy(str1, "?a???b,,,#c");
    len = strlen(str1);

    strcpy(str2, ",");   /* change the tokenizer string */

    /** p2tok = strtok_s(p2str, &len, str2, &p2str); **/
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    ERRNO(EOK);

    if (strcmp(p2tok, "??b") ) {
        debug_printf("%s %u token -%s-  -%s- len=%d error \n",
               __FUNCTION__, __LINE__, p2tok, p2str, (int)len );
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/

    strcpy(str1, ",,0,1,23,456,789,a,b,");
    len = strlen(str1);

    strcpy(str2, ",");   /* change the tokenizer string */

    debug_printf("\n");
    debug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {

        debug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        ERRNO(EOK);

        debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1);

    strcpy(str2, ",.;*");

    debug_printf("\n");
    debug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%s\" \n", str2);

    debug_printf("  p2tok = strtok_s(str1, &len, str2, &p2str); \n");
    p2tok = strtok_s(str1, &len, str2, &p2str);
    ERRNO(EOK);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    ERRNO(EOK);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    /* len is zero at this point */
    debug_printf("  p2tok = strtok_s(NULL, &len, str2, &p2str); \n");
    p2tok = strtok_s(NULL, &len, str2, &p2str);
    debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );

    debug_printf("\n");

/*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1);

    strcpy(str2, ",.;*");

    debug_printf("\n");
    debug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        ERRNO(EOK);

        debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1) - 1;     /** back off the null **/

    strcpy(str2, ",.;*");

    debug_printf("\n");
    debug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        p2tok = strtok_s(p2str, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
/*--------------------------------------------------*/

    strcpy(str1, ",.*;one,two;three,;four*.*.five-six***");

    len = strlen(str1) - 15;     /** back off a few! **/

    strcpy(str2, ",.;*");

    debug_printf("\n");
    debug_printf("String to tokenize str1 = \"%s\"  len = %u\n", str1, (unsigned)len);
    debug_printf("String of delimiters str2 = \"%s\" \n", str2);

    p2str = str1;
    p2tok = str1;

    while (p2tok && len) {
        debug_printf("  p2tok = strtok_s(p2str, &len, str2, &p2str); \n");

        /* p2tok = strtok_s(p2str, &len, str2, &p2str); */
        p2tok = strtok_s(NULL, &len, str2, &p2str);
        if (p2tok) {
            ERRNO(EOK);
            debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
        } else {
            ERRNO_MSVC(ESUNTERM, 0);
        }
    }
/*--------------------------------------------------*/

    return (errs);
}
