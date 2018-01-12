/*------------------------------------------------------------------
 * test_strerror_s
 * File 'str/strerror_s.c'
 * Lines executed:96.77% of 31
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifndef ELAST
#define ELAST 100
#endif

#define LEN   ( 128 )

static char   str1[LEN];

int test_strerror_s (void)
{
    errno_t rc;
    int i;
    size_t len;
    int errs = 0;

/*--------------------------------------------------*/

    rc = strerror_s(NULL, LEN, 0);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strerror_s(str1, 0, 0);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = strerror_s(str1, (RSIZE_MAX_STR+1), 0);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = strerror_s(str1, 3, 1);
    ERR(ESLEMIN);

/*--------------------------------------------------*/

    for (i=0; i<=ELAST; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK);
    }

    for (i=ELAST+1; i<400; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK); /* Unknown error */
    }

    for (i=400; i<=ESLAST; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK);
    }

/*--------------------------------------------------*/

    for (i=0; i<=ELAST; i++) {
        rc = strerror_s(str1, 30, i);
        len = strerrorlen_s(i);
        if (len < 30) {
            ERR(EOK);
        } else {
            size_t ind = strlen(str1);
            if (ind != 29) {
                INDCMP(!= 29);
                debug_printf("%s %u  strerror_s(%d) => %d:%d \"%s\" \n",
                         __FUNCTION__, __LINE__, i, (int)len,
                         (int)strlen(str1), str1);
            }
            ERR(EOK);
        }
    }

    for (i=400; i<=409; i++) {
        rc = strerror_s(str1, 30, i);
        len = strerrorlen_s(i);
        if (len < 30) {
            ERR(EOK);
        } else {
            size_t ind = strlen(str1);
            if (ind != 29) {
                INDCMP(!= 29);
                debug_printf("%s %u  strerror_s(%d) => %d:%d \"%s\" \n",
                         __FUNCTION__, __LINE__, i, (int)len,
                         (int)strlen(str1), str1);
            }
            ERR(EOK);
        }
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strerror_s());
}
#endif
