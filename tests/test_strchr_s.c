/*------------------------------------------------------------------
 * test_strchr_s
 * File 'strchr_s.c'
 * Lines executed:94.44% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

static char str[LEN];
int test_strchr_s(void);

int test_strchr_s(void) {
    errno_t rc;
    int ch;
    char *sub;
    char *std_sub;

    size_t len;
    int errs = 0;

    /*--------------------------------------------------*/

    ch = 0;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strchr_s(NULL, LEN, ch, &sub);
    ERR(ESNULLP);
    SUBNULL();

    EXPECT_BOS("empty resultp")
    rc = strchr_s(str, LEN, ch, NULL);
    ERR(ESNULLP);

    EXPECT_BOS("empty dest or dmax")
    rc = strchr_s(str, 0, ch, &sub);
    ERR(ESZEROL)
    SUBNULL();

    EXPECT_BOS("dest overflow")
    rc = strchr_s(str, RSIZE_MAX_STR + 1, ch, &sub);
    ERR(ESLEMAX)
    SUBNULL();

    EXPECT_BOS("ch overflow >255")
    rc = strchr_s(str, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL();
#endif

    /*--------------------------------------------------*/

    *str = '\0';
    rc = strchr_s(str, LEN, 0, &sub);
    ERR(EOK);
    PTREQ(sub, str);

    /*--------------------------------------------------*/

    strcpy(str, "keep it all together");
    len = strlen(str);

    rc = strchr_s(str, LEN, 0, &sub);
    ERR(EOK);
    PTREQ(sub, &str[len]);

    /*--------------------------------------------------*/

    /* at beginning */
    rc = strchr_s(str, LEN, 'k', &sub);
    ERR(EOK)
    PTREQ(sub, &str[0]);

    /* in the middle - left */
    rc = strchr_s(str, LEN, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &str[1]);

    /* in the middle - right */
    rc = strchr_s(str, LEN, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &str[13]);

    /*--------------------------------------------------*/
    /* 012345678901234567890 */
    strcpy(str, "keep it all together");
    len = strlen(str);

    rc = strchr_s(str, len, 'h', &sub);
    ERR(EOK)
    PTREQ(sub, &str[17]);

    rc = strchr_s(str, len, 'r', &sub);
    ERR(EOK)
    PTREQ(sub, &str[19]);

    /*--------------------------------------------------*/

    rc = strchr_s(str, 3, 'i', &sub);
    ERR(ESNOTFND)
    SUBNULL();

    rc = strchr_s(str, LEN, 'i', &sub);
    ERR(EOK)

    /*--------------------------------------------------*/

    strcpy(str, "keep it all together");

    rc = strchr_s(str, len, 1, &sub);
    ERR(ESNOTFND);
    SUBNULL();

    rc = strchr_s(str, LEN, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &str[1]);
    /* compare to legacy */
    std_sub = strchr(str, 'e');
    PTREQ(sub, std_sub);

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strchr_s()); }
#endif
