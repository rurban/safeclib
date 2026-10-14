/*------------------------------------------------------------------
 * test_strprefix_s
 * File 'extstr/strprefix_s.c'
 * Lines executed:94.44% of 18
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

int main(void) {
    errno_t rc;
    rsize_t len;

    char str1[LEN];
    char str2[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strprefix_s(NULL, LEN, str2);
    ERR(ESNULLP)

    EXPECT_BOS("empty src")
    rc = strprefix_s(str1, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strprefix_s(str1, 0, str2);
    ERR(ESZEROL)

    EXPECT_BOS("dest overflow")
    rc = strprefix_s(str1, RSIZE_MAX_STR + 1, str2);
    ERR(ESLEMAX)
#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strprefix_s(str1, LEN, str2);
    ERR(ESNOTFND)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 55, str2);
    ERR(EOK)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 55, str2);
    ERR(ESNOTFND)
    /*--------------------------------------------------*/

    strcpy(str1, "ke");
    strcpy(str2, "kerp");

    rc = strprefix_s(str1, 2, str2);
    ERR(EOK)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep");

    rc = strprefix_s(str1, 4, str2);
    ERR(EOK)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keeeep");

    rc = strprefix_s(str1, 4, str2);
    ERR(ESNOTFND)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    rc = strprefix_s(str1, 4, "");
    ERR(ESNOTFND)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it sipmle and very long");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    ERR(ESNOTFND)
    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simple");

    len = strlen(str1);

    rc = strprefix_s(str1, len, str2);
    ERR(EOK)
    /*--------------------------------------------------*/

    /* dmax > RSIZE_MAX_STR with unknown dest size (hidden from the compiler) */
    {
        static volatile size_t hsz = 16;
        char *hp = (char *)malloc(hsz);
        

        if (!hp) {
            return errs + 1;
        }
        memset(hp, 'x', hsz - 1);
        hp[hsz - 1] = '\0';
        rc = strprefix_s(hp, RSIZE_MAX_STR + 1, "a");
        ERR(ESLEMAX)
        free(hp);
    }

    /*--------------------------------------------------*/

    return (errs);
}
