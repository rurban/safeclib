/*------------------------------------------------------------------
 * test_strcmpfld_s
 * File 'extstr/strcmpfld_s.c'
 * Lines executed:88.24% of 17
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX (128)
#define LEN (128)

int main(void) {
    errno_t rc;
    uint32_t i;
    rsize_t len;
    int ind;

    char str1[LEN];
    char str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strcmpfld_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)

    len = 5;
    EXPECT_BOS("empty src")
    rc = strcmpfld_s(str1, len, NULL, &ind);
    ERR(ESNULLP)

    len = 5;
    EXPECT_BOS("empty resultp")
    rc = strcmpfld_s(str1, len, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strcmpfld_s(str1, 0, str2, &ind);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strcmpfld_s(str1, (RSIZE_MAX_STR + 1), str2, &ind);
    ERR(ESLEMAX)

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strcmp_s(str1, LEN + 1, str2, &ind);
    ERR(EOVERFLOW)
    INDZERO()
#endif
#endif
    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = 1;
    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = 2;
    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = 3;
    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    len = strlen(str1);
    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/
    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str1[10] = '\0';

    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[10] = '\0';

    len = strlen(str1);
    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);
    str1[0] = '\0';
    str1[15] = '\0';

    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = '\0';
    str2[15] = '\0';

    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);
    str1[3] = '\0'; /* now place a null */

    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[3] = '\0';

    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    len = strlen(str1);
    str1[1] = '\0'; /* now place nulls */
    str1[20] = '\0';

    strcpy(str2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[1] = '\0';
    str2[20] = '\0';

    rc = strcmpfld_s(str1, len, str2, &ind);
    ERR(EOK)
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  ind=%d  rc=%u \n",
                         __FUNCTION__, __LINE__, i, str1[i], i, str2[i], ind,
                         rc);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}
