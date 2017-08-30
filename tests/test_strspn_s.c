/*------------------------------------------------------------------
 * test_strspn_s
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

int test_strspn_s (void)
{
    errno_t rc = 0;
    rsize_t count = 0;
    int std_count = 0;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strspn_s(NULL, LEN, str2, LEN, &count);
    ERR(ESNULLP)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strspn_s(str1, LEN, NULL, LEN, &count);
    ERR(ESNULLP)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strspn_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strspn_s(str1, 0, str2, LEN, &count);
    ERR(ESZEROL)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strspn_s(str1, LEN, str2, 0, &count);
    ERR(ESZEROL)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strspn_s(str1, RSIZE_MAX_STR+1, str2, LEN, &count);
    ERR(ESLEMAX)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strspn_s(str1, RSIZE_MAX_STR, str2, RSIZE_MAX_STR+1, &count);
    ERR(ESLEMAX)
    if (count != 0) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
               __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strspn_s(str1, 1, str2, LEN, &count);
    ERR(EOK)
    if (count != 1) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strspn_s(str1, 2, str2, LEN, &count);
    ERR(EOK)
    if (count != 2) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strspn_s(str1, 3, str2, 12, &count);
    ERR(EOK)
    if (count != 3) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "k");

    rc = strspn_s(str1, LEN, str2, 1, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "ke");

    rc = strspn_s(str1, LEN, str2, 2, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "notincluded");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "1234567890");

    rc = strspn_s(str1, LEN, str2, LEN, &count);
    ERR(EOK)
    std_count = strspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
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
    return (test_strspn_s());
}
#endif
