/*------------------------------------------------------------------
 * test_strcpyfld_s
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

    rc = strcpyfld_s(NULL, LEN, str2, slen);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

   strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = 5;
    slen = 5;
    rc = strcpyfld_s(str1, len, NULL, slen);
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

    len = 5;
    slen = 5;
    rc = strcpyfld_s(str1, 0, str2, slen);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 5;
    slen = 5;
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR+1), str2, slen);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 5;
    slen = 0;
    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    len = 5;
    slen = len+1;
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR+1), str2, slen);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 1;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 1;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 2;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 2;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 3;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 3;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = strlen(str2);
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1) + 2;
    str1[20] = '\0';

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = strlen(str1);
    str2[0] = '\0';
    str2[20] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

    for (i=0; i<slen; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
        }
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/

    return (0);
}
