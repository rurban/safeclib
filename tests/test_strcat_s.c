/*------------------------------------------------------------------
 * test_strcat_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int test_strcat_s (void)
{
    errno_t rc;
    int32_t  ind;
    int32_t  len1;
    int32_t  len2;
    int32_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcat_s(NULL, LEN, str2);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strcat_s(str1, LEN, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strcat_s(str1, 0, str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = strcat_s(str1, (RSIZE_MAX_STR+1), str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 1, str2);
    ERR(ESUNTERM)
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 2, str2);
    ERR(ESUNTERM)
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(&str1[0], "aaaaaaaaaa");
    strcpy(&str2[0], "keep it simple");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = strcat_s(str1, 50, str2);
    ERR(EOK)
    len3 = strlen(str1);
    if (len3 != (len1+len2)) {
        debug_printf("%s %u lengths wrong: %u  %u  %u \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 1, str2);
    ERR(ESNOSPC)
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 2, str2);
    ERR(ESNOSPC)
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 20, str2);
    ERR(EOK)
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strcat_s(str1, LEN, str2);
    ERR(EOK)
    EXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, str2)

/*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, LEN, str2);
    ERR(EOK)
    EXPSTR(str1, "qqweqqkeep it simple")

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 12, str2);
    ERR(ESNOSPC)
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = strcat_s(str1, 52, str2);
    ERR(EOK)
    EXPSTR(str1, "1234keep it simple")

/*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = strcat_s(str1, 8, &str1[7]);
    ERR(ESOVRLP)
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = strcat_s(str1, 9, &str1[8]);
    ERR(ESOVRLP)
    EXPNULL(str1)

/*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = strcat_s(str2, 31, &str1[0]);
    ERR(EOK)
    EXPSTR(str2, "123keep it simple")

/*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = strcat_s(str2, 10, str1);
    ERR(EOK)
    EXPSTR(str2, "123456789")

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strcat_s());
}
#endif
