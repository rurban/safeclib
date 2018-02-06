/*------------------------------------------------------------------
 * test_strcpyfld_s
 * File 'extstr/strcpyfld_s.c'
 * Lines executed:97.56% of 41
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
    int ind;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcpyfld_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaa");
    slen = 5;
    rc = strcpyfld_s(str1, 0, str2, slen);
    ERR(ESZEROL); /* and untouched */
    EXPSTR(str1, "aaaaa");

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    slen = 5;
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR+1), str2, slen);
    ERR(ESLEMAX);  /* and untouched */
    EXPSTR(str1, "aaaaa");

/*--------------------------------------------------*/

    len = 5;
    slen = len+1; /* ESLEMAX over ESNOSPC */
    rc = strcpyfld_s(str1, (RSIZE_MAX_STR+1), str2, slen);
    ERR(ESLEMAX);  /* and untouched */
    EXPSTR(str1, "aaaaa");
#endif
/*--------------------------------------------------*/

    len = 5;
    rc = strcpyfld_s(str1, len, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfld_s(NULL, len, str2, 0);
    ERR(EOK); /* and untouched */

    rc = strcpyfld_s(str1, 0, str2, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

    rc = strcpyfld_s(str1, len, NULL, 0);
    ERR(EOK); /* and untouched */
    EXPSTR(str1, "aaaaa");

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = 5;
    slen = 5;
    rc = strcpyfld_s(str1, len, NULL, slen);
    ERR(ESNULLP); /* and cleared */
    CHECK_SLACK(str1, len);

/*--------------------------------------------------*/

    len = 5;
    rc = strcpyfld_s(str1, len-1, str2, len);
    ERR(ESNOSPC); /* and cleared */
    CHECK_SLACK(str1, len-1);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 1;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 1;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 2;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 2;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 2;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 2;

    rc = strcpyfld_s(str2, len, str1, slen);
    ERR(EOK)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 10;

    rc = strcpyfld_s(str1, len, &str1[5], 10);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(str1, len);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 10;

    rc = strcpyfld_s(&str1[5], len, &str1[0], 10);
    ERR(ESOVRLP); /* and cleared */
    CHECK_SLACK(&str1[5], len);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = 3;

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = 3;
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);

    strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    slen = strlen(str2);
    str2[0] = '\0';

    rc = strcpyfld_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
            errs++;
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
    ERR(EOK)
    for (i=0; i<slen; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d   rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i],  rc );
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
