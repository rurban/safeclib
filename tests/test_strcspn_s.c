/*------------------------------------------------------------------
 * test_strcspn_s
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

int test_strcspn_s (void)
{
    errno_t rc;
    rsize_t count;
    int std_count;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcspn_s(NULL, LEN, str2, LEN, &count);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, NULL, LEN, &count);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, str2, LEN, NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, 0, str2, LEN, &count);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, LEN, str2, 0, &count);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, RSIZE_MAX_STR+1, str2, LEN, &count);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strcspn_s(str1, RSIZE_MAX_STR, str2, RSIZE_MAX_STR+1, &count);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 0) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
    std_count = strcspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 1, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 1) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 2, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 2) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 3, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 3) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "KEEP");

    rc = strcspn_s(str1, 6, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 6) {
        debug_printf("%s %u  Error  count=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "ABCDEF");

    rc = strcspn_s(str1, 6, str2, 2, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    if (count != 6) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    std_count = strcspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "notincluded");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    std_count = strcspn(str1, str2);
    if ((int)count != std_count) {
        debug_printf("%s %u  count=%d  std_count=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)count, std_count, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "1234567890");

    rc = strcspn_s(str1, LEN, str2, LEN, &count);
    if (rc != EOK) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    std_count = strcspn(str1, str2);
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
    return (test_strcspn_s());
}
#endif
