/*------------------------------------------------------------------
 * test_strncat_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int test_strncat_s (void)
{
    errno_t rc;
    int32_t ind;

/*--------------------------------------------------*/

    rc = strncat_s(NULL, LEN, str2, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = strncat_s(str1, LEN, NULL, LEN);
    if (rc != ESNULLP) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = strncat_s(str1, 0, str2, LEN);
    if (rc != ESZEROL) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = strncat_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    rc = strncat_s(str1, (RSIZE_MAX_STR), str2, (RSIZE_MAX_STR+1));
    if (rc != ESLEMAX) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 1, str2, LEN);
    if (rc != ESUNTERM) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 2, str2, LEN);
    if (rc != ESUNTERM) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
    }

/*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    rc = strncat_s(str1, 2, str2, 1);
    if (rc != ESNOSPC) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
    }

/*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    rc = strncat_s(str1, 3, str2, 1);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    ind = strcmp(str1, "ab");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 50, str2, LEN);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    ind = strcmp(str1, "aaaaaaaaaakeep it simple");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
    }

/*--------------------------------------------------*/
/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "good");
    strcpy(str2, "bye");

    rc = strncat_s(str1, 100, str2, 100);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    ind = strcmp(str1, "goodbye");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
    }

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "hello");

    rc = strncat_s(str1, 6, "", 1);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    ind = strcmp(str1, "hello");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
    }

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "hello");

    rc = strncat_s(str1, 6, "X", 2);
    if (rc != ESNOSPC) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
    }

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "abc");

    rc = strncat_s(str1, 7, "defghijklmn", 3);
    if (rc != EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
    }

    ind = strcmp(str1, "abcdef");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
    }

/*--------------------------------------------------*/

    return (0);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strncat_s());
}
#endif
