/*------------------------------------------------------------------
 * test_strerror_s
 * File 'str/strerror_s.c'
 * Lines executed:88.89% of 27
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRERROR_S
#define HAVE_NATIVE 1
#else
#define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#ifndef ELAST
#define ELAST 100
#endif

#define LEN (128)

static char str1[LEN];
int test_strerror_s(void);

int test_strerror_s(void) {
    errno_t rc;
    int i;
    size_t len;
    int errs = 0;

    /*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strerror_s(NULL, LEN, 0);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

    EXPECT_BOS("empty dest or dmax")
    rc = strerror_s(str1, 0, 0);
    ERR_MSVC(ESZEROL, EINVAL);

    EXPECT_BOS("dest overflow")
    rc = strerror_s(str1, (RSIZE_MAX_STR + 1), 0);
    ERR_MSVC(ESLEMAX, EINVAL);
#endif

    /*--------------------------------------------------*/

    rc = strerror_s(str1, 3, 1);
    ERR_MSVC(ESLEMIN, 0);

    /*--------------------------------------------------*/

    for (i = 0; i <= ELAST; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK);
    }

    for (i = ELAST + 1; i < 400; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK); /* Unknown error */
    }

    for (i = 400; i <= ESLAST; i++) {
        rc = strerror_s(str1, LEN, i);
        ERR(EOK);
    }

    /*--------------------------------------------------*/

    for (i = 0; i <= ELAST; i++) {
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

    for (i = 400; i <= 409; i++) {
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

int main(void) { return (test_strerror_s()); }
