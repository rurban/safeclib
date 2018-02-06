/*------------------------------------------------------------------
 * test_strstr_s
 * File 'extstr/strstr_s.c'
 * Lines executed:100.00% of 43
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

static char   str1[LEN];
static char   str2[LEN];

int test_strstr_s (void)
{
    errno_t rc;
    char *sub;
    char *std_sub;

    rsize_t len1;
    rsize_t len2;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strstr_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)

    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strstr_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strstr_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strstr_s(str1, 0, str2, LEN, &sub);
    ERR(ESZEROL)
    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = strstr_s(str1, RSIZE_MAX_STR+1, str2, LEN, &sub);
    ERR(ESLEMAX)
    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
#endif
/*--------------------------------------------------*/

    rc = strstr_s(str1, LEN, str2, 0, &sub);
    ERR(ESZEROL)
    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = strstr_s(str1, LEN, str2, RSIZE_MAX_STR+1, &sub);
    ERR(ESLEMAX)
    if (sub) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
#endif
/*--------------------------------------------------*/

    *str1 = '\0';
    *str2 = '\0';

    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != str1) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    *str2 = '\0';

    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != str1) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "kee");

    /* substring at beginning */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != &str1[0]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "eep it");

    /* substring in the middle - left */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != &str1[1]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "ethe");

    /* substring in the middle - right */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != &str1[15]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/
               // 012345678901234567890
    strcpy(str1, "keep it all together");
    strcpy(str2, "he");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = strstr_s(str1, len1, str2, len2, &sub);
    ERR(EOK)
    if (sub != &str1[17]) {
        debug_printf("%s %u  Error rc=%d  sub=%s \n",
                     __FUNCTION__, __LINE__, rc, sub);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = strstr_s(str1, len1, str2, len2, &sub);
    ERR(EOK)
    if (sub != &str1[18]) {
        debug_printf("%s %u  Error rc=%d  sub=%s \n",
                     __FUNCTION__, __LINE__, rc, sub);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strstr_s(str1, 3, str2, LEN, &sub);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strstr_s(str1, 45, str2, LEN, &sub);
    ERR(EOK)

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strstr_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "1234");

    len1 = strlen(str1);

    rc = strstr_s(str1, len1, str2, LEN, &sub);
    ERR(ESNOTFND)
    if (sub != NULL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strstr_s(str1, LEN, str2, 2, &sub);
    ERR(EOK)
/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "eep");

    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    if (sub != &str1[1]) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    /* compare to legacy */
    std_sub = strstr(str1, str2);
    if (sub != std_sub) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strstr_s());
}
#endif
