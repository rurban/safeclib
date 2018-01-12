/*------------------------------------------------------------------
 * test_strrchr_s
 * File 'strrchr_s.c'
 * Lines executed:100.00% of 21
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <string.h>

#define LEN   ( 128 )

static char   str[LEN];

int test_strrchr_s (void)
{
    errno_t rc;
    int  ch;
    char *sub;
#ifdef HAVE_STRRCHR
    char *std_sub;
#endif
    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    ch = 0;
    rc = strrchr_s(NULL, LEN, ch, &sub);
    ERR(ESNULLP);
    SUBNULL();

    rc = strrchr_s(str, LEN, ch, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = strrchr_s(str, 0, ch, &sub);
    ERR(ESZEROL)
    SUBNULL();

/*--------------------------------------------------*/

    rc = strrchr_s(str, RSIZE_MAX_STR+1, ch, &sub);
    ERR(ESLEMAX)
    SUBNULL();

    rc = strrchr_s(str, LEN, 256, &sub);
    ERR(ESLEMAX)
    SUBNULL();

/*--------------------------------------------------*/

    memset(str, 0, LEN);
    rc = strrchr_s(str, LEN, 0, &sub);
    ERR(ESZEROL);
    SUBNULL();

/*--------------------------------------------------*/

    strcpy(str, "keep it all together");
    len = strlen(str);

    rc = strrchr_s(str, len+1, 0, &sub);
    ERR(EOK);
    PTREQ(sub, &str[len]);

/*--------------------------------------------------*/

    /* at beginning */
    rc = strrchr_s(str, len, 'k', &sub);
    ERR(EOK)
    PTREQ(sub, &str[0]);

    /* in the middle - left */
    rc = strrchr_s(str, 5, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &str[2]);

    /* in the middle - right */
    rc = strrchr_s(str, len, 'o', &sub);
    ERR(EOK)
    PTREQ(sub, &str[13]);

/*--------------------------------------------------*/
              /* 012345678901234567890 */
    strcpy(str, "keep it all together");
    len = strlen(str);

    rc = strrchr_s(str, len, 'h', &sub);
    ERR(EOK)
    PTREQ(sub, &str[17]);

    rc = strrchr_s(str, len, 'r', &sub);
    ERR(EOK)
    PTREQ(sub, &str[19]);

/*--------------------------------------------------*/

    rc = strrchr_s(str, 3, 'i', &sub);
    ERR(ESNOTFND)
    SUBNULL();

    rc = strrchr_s(str, len, 'i', &sub);
    ERR(EOK)

/*--------------------------------------------------*/

    strcpy(str, "keep it all together");

    rc = strrchr_s(str, len, 1, &sub);
    ERR(ESNOTFND);
    SUBNULL();

    rc = strrchr_s(str, len, 'e', &sub);
    ERR(EOK)
    PTREQ(sub, &str[18]);
#ifdef HAVE_STRRCHR
    /* compare to legacy.
       note that truncated len is different here!
       legacy uses the full strlen, even if > dlen
     */
    std_sub = strrchr(str, 'e');
    PTREQ(sub, std_sub);
#endif

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strrchr_s());
}
#endif
