/*------------------------------------------------------------------
 * test_strstr_s
 * File 'extstr/strstr_s.c'
 * Lines executed:91.49% of 47
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)
#define SHORT_LEN (5)

static char str1[LEN];
static char str2[LEN];
int test_strstr_s(void);

int test_strstr_s(void) {
    errno_t rc;
    char *sub;
    char *std_sub;

    rsize_t len1;
    rsize_t len2;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strstr_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty src")
    rc = strstr_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

    EXPECT_BOS("empty substringp")
    rc = strstr_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strstr_s(str1, 0, str2, LEN, &sub);
    ERR(ESZEROL)
    SUBNULL()

    EXPECT_BOS("dest overflow")
    rc = strstr_s(str1, RSIZE_MAX_STR + 1, str2, LEN, &sub);
    ERR(ESLEMAX)
    SUBNULL()

    EXPECT_BOS("src overflow")
    rc = strstr_s(str1, LEN, str2, RSIZE_MAX_STR + 1, &sub);
    ERR(ESLEMAX)
    SUBNULL()

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strstr_s(str1, LEN + 1, str2, LEN, &sub);
    ERR(EOVERFLOW)
    SUBNULL()

    EXPECT_BOS("src overflow")
    rc = strstr_s(str1, LEN, str2, LEN + 1, &sub);
    ERR(EOVERFLOW)
    SUBNULL()
#endif

#endif /* HAVE_CT_BOS_OVR */

    /*--------------------------------------------------*/

    strcpy(str2, "a");
    rc = strstr_s(str1, LEN, str2, 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

    strcpy(str1, "keep it all together");
    strcpy(str2, "keep");

    /* equality with restricted lengths (slen > dmax) */
    rc = strstr_s(str1, 3, str2, LEN, &sub);
    ERR(ESNOTFND)
    SUBNULL()

    /*--------------------------------------------------*/

    *str1 = '\0';
    *str2 = '\0';

    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    *str2 = '\0';

    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "kee");

    /* substring at beginning */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "");

    /* allow slen = 0 with empty src */
    rc = strstr_s(str1, LEN, str2, 0, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "eep it");

    /* substring in the middle - left */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[1])

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "ethe");

    /* substring in the middle - right */
    rc = strstr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[15])

    /*--------------------------------------------------*/
    // 012345678901234567890
    strcpy(str1, "keep it all together");
    strcpy(str2, "he");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = strstr_s(str1, len1, str2, len2, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[17])

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = strstr_s(str1, len1, str2, len2, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[18])

    /*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strstr_s(str1, 3, str2, LEN, &sub);
    ERR(ESNOTFND)
    SUBNULL()

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
    SUBNULL()

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
    PTREQ(sub, &str1[1])

    /* compare to legacy */
    std_sub = strstr(str1, str2);
    PTREQ(sub, std_sub)

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strstr_s()); }
#endif
