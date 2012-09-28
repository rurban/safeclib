/*------------------------------------------------------------------
 * test_strcpyfldin_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

int main()
{
    errno_t rc;
    uint32_t i;
    rsize_t len;
    rsize_t slen;

    char   str1[LEN];
    char   str2[LEN];


/*--------------------------------------------------*/

    rc = strcpyfldin_s(NULL, LEN, str2, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 5;
    rc = strcpyfldin_s(str1, len, NULL, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
        }
    }

/*--------------------------------------------------*/

    rc = strcpyfldin_s(str1, 0, str2, LEN);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    rc = strcpyfldin_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = strlen(str1);
    slen = strlen(str2);

    rc = strcpyfldin_s(str1, len, str2, slen);
    if (rc == EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != 0) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 1;
    rc = strcpyfldin_s(str1, len, str2, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 2;
    rc = strcpyfldin_s(str1, len, str2, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 3;
    rc = strcpyfldin_s(str1, len, str2, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }


/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    /* same string in dest and src */
    len = LEN;
    rc = strcpyfldin_s(str1, len, str1, len);
    if (rc != ESOVRLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results ");
    len = strlen(str1);

    /* overlap */
    rc = strcpyfldin_s(&str1[0], len, &str1[5], len);
    if (rc != ESOVRLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results ");
    len = 20;

    /* overlap */
    rc = strcpyfldin_s(&str1[10], len, &str1[0], len);
    if (rc != ESOVRLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=10; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    len = strlen(str1);

    strcpy(str2, "KEEP IT SIMPLE PLEASE");

    rc = strcpyfldin_s(str1, len, str2, len);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "always keep it simple");
    len = strlen(str1);

    strcpy(str2, "keep it simple");
    slen = strlen(str2);

    rc = strcpyfldin_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<slen; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
        }
    }

/*--------------------------------------------------*/

    return (0);
}
