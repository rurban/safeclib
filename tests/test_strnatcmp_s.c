/*------------------------------------------------------------------
 * test_strnatcmp_s
 * File 'extstr/strnatcmp_s.c'
 * Lines executed:98.65% of 74
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include "config.h"

#define sgn(i) ((i) > 0 ? 1 : ((i) < 0 ? -1 : 0))

#define LEN (128)
#define SHORT_LEN (5)

static char str1[LEN];
static char str2[LEN];
#if !defined(HAVE_CT_BOS_OVR) && defined(HAVE___BUILTIN_OBJECT_SIZE)
static char str4[4];
#endif
int test_strnatcmp_s(void);

int test_strnatcmp_s(void) {
    errno_t rc;
    int ind;
    int errs = 0;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = strnatcmp_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty src")
    rc = strnatcmp_s(str1, LEN, NULL, &ind);
    ERR(ESNULLP)
    INDZERO()

    EXPECT_BOS("empty resultp")
    rc = strnatcmp_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)

    EXPECT_BOS("empty dest or dmax")
    rc = strnatcmp_s(str1, 0, str2, &ind);
    ERR(ESZEROL)
    INDZERO()

    EXPECT_BOS("dest overflow")
    rc = strnatcmp_s(str1, RSIZE_MAX_STR + 1, str2, &ind);
    ERR(ESLEMAX)
    INDZERO()

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    EXPECT_BOS("dest overflow")
    rc = strnatcmp_s(str1, LEN + 1, str2, &ind);
    ERR(EOVERFLOW)
    INDZERO()

    strcpy(str1, "test");
    memcpy(str4, "test", 4);
    rc = strnatcmp_s(str1, LEN, str4, &ind);
    ERR(ESUNTERM)
    INDZERO()
#endif
#endif
    /*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strnatcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simple");

    rc = strnatcmp_s(str1, 5, str2, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    strcpy(str1, "Keep it simple");
    strcpy(str2, "keep it simple");

    rc = strnatcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keeP it simple");

    rc = strnatcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");

    rc = strnatcmp_s(str1, LEN, str1, &ind);
    ERR(EOK)
    INDZERO()

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simplified");
    strcpy(str2, "keep it simple");

    rc = strnatcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(<= 0)

    /*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    strcpy(str2, "keep it simplified");

    rc = strnatcmp_s(str1, LEN, str2, &ind);
    ERR(EOK)
    INDCMP(>= 0)

    /*--------------------------------------------------*/

    rc = strnatcmp_s("img12", 6, "img10", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("img10", 6, "img12", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("img9", 5, "img10", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("img10", 6, "img9", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("img10", 6, "img10", &ind);
    ERR(EOK)
    INDCMP(!= 0)

    rc = strnatcmp_s("a19", 4, "a28", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("a29", 4, "a18", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("a15", 4, "a2", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("a12b", 5, "a13c", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("x01", 4, "x02", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("x02", 4, "x01", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("a0", 3, "a01", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("a01", 4, "a0", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcmp_s("a01x", 5, "a01y", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("a01", 4, "a01", &ind);
    ERR(EOK)
    INDCMP(!= 0)

    rc = strnatcmp_s("a05", 4, "a1", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcmp_s("a1", 3, "a05", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    /* case folding */
    rc = strnatcasecmp_s("ABC", 4, "abd", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    rc = strnatcasecmp_s("abd", 4, "ABC", &ind);
    ERR(EOK)
    INDCMP(!= 1)

    rc = strnatcasecmp_s("AbC", 4, "aBc", &ind);
    ERR(EOK)
    INDZERO()

    rc = strnatcmp_s("AbC", 4, "aBc", &ind);
    ERR(EOK)
    INDCMP(!= -1)

    /*--------------------------------------------------*/

    /* dmax > RSIZE_MAX_STR with unknown dest size (hidden from the compiler) */
    {
        static volatile size_t hsz = 16;
        char *hp = (char *)malloc(hsz);
        int r;

        if (!hp) {
            return errs + 1;
        }
        memset(hp, 'x', hsz - 1);
        hp[hsz - 1] = '\0';
        rc = strnatcmp_s(hp, RSIZE_MAX_STR + 1, "a", &r);
        ERR(ESLEMAX)
        free(hp);
    }

    /*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_strnatcmp_s()); }
#endif
