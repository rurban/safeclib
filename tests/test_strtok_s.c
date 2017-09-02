/*------------------------------------------------------------------
 * test_strtok_s
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
    char *p2str;
    char *p2tok;

    rsize_t len;
    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    p2tok = strtok_s(str1, NULL, str2,  &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u   Error rc=%d \n",
               __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    len = 0;
    p2tok = strtok_s(str1, &len, str2,  &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u   Error rc=%d \n",
               __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    len = RSIZE_MAX_STR + 1;
    p2tok = strtok_s(str1, &len, str2,  &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    len = 0;
    p2tok = strtok_s(str1, &len, NULL,  &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    p2tok = strtok_s(str1, &len, str2,  NULL);
    if (p2tok != NULL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    /* no token test */
    strcpy(str1, "aaaaaaaa");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    strcpy(str1, "jklmnopqrst");
    len = strlen(str1) - 2;             /* cheat on len */

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    if (p2tok != NULL) {
        debug_printf("token -%s-  -%s- \n", p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
    debug_printf("test: %u \n", __LINE__);

    strcpy(str1, "aaamnopqrst");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok == NULL) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
/*--------------------------------------------------*/
/** [1] **/
    debug_printf("test: %u \n", __LINE__);

    strcpy(str1, "aaamnopqrstfedcba");
    len = strlen(str1);

    strcpy(str2, "fedcba");

    p2tok = strtok_s(str1, &len, str2, &p2str);
    if (p2tok == NULL) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    if (strcmp(p2tok, "mnopqrst")) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/** [2] **/

    p2tok = strtok_s(p2str, &len, str2, &p2str);
    if (p2tok != NULL) {
        debug_printf("%s %u token -%s-  remaining -%s-  \n",
                     __FUNCTION__, __LINE__,  p2tok, p2str);
        errs++;
    }
    /* printf("token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len ); */

/*--------------------------------------------------*/
/*--------------------------------------------------*/
/** [1] **/
    debug_printf("test: %u \n", __LINE__);

    strcpy(str1, "?a???b,,,#c");
    len = strlen(str1);

    strcpy(str2, "?");

    p2tok = strtok_s(str1, &len, str2, &p2str);

    if (strcmp(p2tok, "a") ) {
        debug_printf("%s %u token -%s-  -%s- len=%d \n",
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

    if (strcmp(p2tok, "??b") ) {
        debug_printf("%s %u token -%s-  -%s- len=%d \n",
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

        debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
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

        debug_printf("  token -%s-  -%s- len=%d \n", p2tok, p2str, (int)len );
    }
/*--------------------------------------------------*/

    return (errs);
}
