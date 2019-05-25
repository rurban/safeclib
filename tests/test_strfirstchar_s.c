/*------------------------------------------------------------------
 * test_strfirstchar_s
 * File 'extstr/strfirstchar_s.c'
 * Lines executed:93.33% of 15
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

#define EXPFIRST(x)                                                            \
    if (rc != x) {                                                             \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)first, rc); \
        errs++;                                                                \
    }
#define NOFIRST()                                                              \
    if (first) {                                                               \
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n",              \
                     __FUNCTION__, __LINE__, (void *)str1, (void *)first, rc); \
        errs++;                                                                \
    }

int main(void) {
    errno_t rc;
    rsize_t len;

    char *first;
    char str1[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strfirstchar_s(NULL, LEN, 'a', &first);
    EXPFIRST(ESNULLP)
    NOFIRST()
    /*--------------------------------------------------*/

    EXPECT_BOS("empty firstp")
    rc = strfirstchar_s(str1, LEN, 'a', NULL);
    EXPFIRST(ESNULLP)

    /*--------------------------------------------------*/

    EXPECT_BOS("empty dest or dmax")
    rc = strfirstchar_s(str1, 0, 'a', &first);
    EXPFIRST(ESZEROL)
    NOFIRST()

    EXPECT_BOS("dest overflow")
    rc = strfirstchar_s(str1, RSIZE_MAX_STR + 1, 'a', &first);
    EXPFIRST(ESLEMAX)
    NOFIRST()

#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';

    rc = strfirstchar_s(str1, LEN, 'a', &first);
    EXPFIRST(ESNOTFND)
    NOFIRST()

    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");

    rc = strfirstchar_s(str1, LEN, 'z', &first);
    EXPFIRST(ESNOTFND)
    NOFIRST()
    /*--------------------------------------------------*/

    strcpy(str1, "kEEp it simple");

    rc = strfirstchar_s(str1, LEN, 'E', &first);
    ERR(EOK)
    if (first != &str1[1]) {
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simpleZ");
    len = strlen(str1);

    rc = strfirstchar_s(str1, len, 'Z', &first);
    ERR(EOK)
    if (first != &str1[14]) {
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simpleZZ");

    rc = strfirstchar_s(str1, LEN, 'Z', &first);
    ERR(EOK)
    if (first != &str1[14]) {
        debug_printf("%s %u  Error  str1=%p  first=%p  rc=%d \n", __FUNCTION__,
                     __LINE__, (void *)str1, (void *)first, rc);
        errs++;
    }
    /*--------------------------------------------------*/

    return (errs);
}
