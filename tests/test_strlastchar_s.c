/*------------------------------------------------------------------
 * test_strlastchar_s
 * File 'extstr/strlastchar_s.c'
 * Lines executed:93.75% of 16
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

#define EXPLAST(x)                                                             \
    if (rc != x) {                                                             \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)last, rc);  \
        errs++;                                                                \
    }
#define NOLAST()                                                               \
    if (last) {                                                                \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)last, rc);  \
        errs++;                                                                \
    }

int main(void) {
    errno_t rc;

    char *last;
    char str1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strlastchar_s(NULL, LEN, 'a', &last);
    EXPLAST(ESNULLP)
    NOLAST()
    /*--------------------------------------------------*/

    EXPECT_BOS("empty lastp")
    rc = strlastchar_s(str1, LEN, 'a', NULL);
    EXPLAST(ESNULLP)
    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = strlastchar_s(str1, 0, 'a', &last);
    EXPLAST(ESZEROL)
    NOLAST()

    /*--------------------------------------------------*/

    EXPECT_BOS("dest overflow")
    rc = strlastchar_s(str1, RSIZE_MAX_STR + 1, 'a', &last);
    EXPLAST(ESLEMAX)
    NOLAST()

#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';

    rc = strlastchar_s(str1, LEN, 'a', &last);
    EXPLAST(ESNOTFND)
    NOLAST()

    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");

    rc = strlastchar_s(str1, 5, 'z', &last);
    EXPLAST(ESNOTFND)
    NOLAST()

    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simplezz");

    rc = strlastchar_s(str1, LEN, 'z', &last);
    EXPLAST(EOK)
    if (last != &str1[15]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)last, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");

    rc = strlastchar_s(str1, LEN, 'K', &last);
    ERR(EOK)
    if (last != &str1[0]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)last, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "kEEp it simple");

    rc = strlastchar_s(str1, LEN, 'E', &last);
    ERR(EOK)
    if (last != &str1[2]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)last, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "kEep it Simple");

    rc = strlastchar_s(str1, LEN, 'S', &last);
    ERR(EOK)
    if (last != &str1[8]) {
        debug_printf("%s %u  Error  str1=%p  last=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)last, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}
