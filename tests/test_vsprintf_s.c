/*------------------------------------------------------------------
 * test_vsprintf_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#define ERR(n)                                     \
    if (rc != (n)) {                               \
        debug_printf("%s %u   Error rc=%u \n",     \
                     __FUNCTION__, __LINE__,  rc); \
        errs++;                                    \
    }
#define NOERR()                                    \
    if (rc < 0) {                                  \
        debug_printf("%s %u   Error rc=%u \n",     \
                 __FUNCTION__, __LINE__,  rc);     \
        errs++;                                    \
    }

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int vtprintf_s (char *restrict dest, rsize_t dmax, const char *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vsprintf_s(dest, dmax, fmt, ap);
    va_end(ap);
    return rc;
}
 
int test_vsprintf_s (void)
{
    errno_t rc;
    int32_t  ind;
    int32_t  len1;
    int32_t  len2;
    int32_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    /* not testable
      rc = vtprintf_s(str1, LEN, "%s", NULL);
      ERR(ESNULLP)
    */

/*--------------------------------------------------*/

    rc = vtprintf_s(str1, LEN, NULL, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = vtprintf_s(str1, 0, "%s", str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = vtprintf_s(str1, (RSIZE_MAX_STR+1), "%s", str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 1, "%s", str2);
    ERR(ESNOSPC)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");
    
    rc = vtprintf_s(str1, 2, "%s", str2);
    ERR(ESNOSPC)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    len1 = strlen(str1);
    len2 = strlen(str2);

    rc = vtprintf_s(str1, 50, "%s", str2);
    ERR(len2)
    len3 = strlen(str1);
    if (len3 != len2) {
        debug_printf("%s %u lengths wrong: %u  %u  %u \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 1, "%s", str2);
    ERR(ESNOSPC)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 2, "%s", str2);
    ERR(ESNOSPC)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 20, "%s", str2);
    NOERR()
    ind = strcmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = vtprintf_s(str1, LEN, "%s", str2);
    ERR(0)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, LEN, "%s", str2);
    NOERR()
    ind = strcmp(str1, str2);
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, LEN, "%s", str2);
    NOERR()
    ind = strcmp(str1, "keep it simple");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 12, "%s", str2);
    ERR(ESNOSPC)

/*--------------------------------------------------*/

    strcpy(str1, "1234");
    strcpy(str2, "keep it simple");

    rc = vtprintf_s(str1, 52, "%s", str2);
    NOERR()
    ind = strcmp(str1, "keep it simple");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "12345678901234567890");

    rc = vtprintf_s(str1, 8, "%s", &str1[7]);
    ERR(ESNOSPC)
    if (str1[0] != '\0') {
        debug_printf("%s %u  Expected null  \n",
                     __FUNCTION__, __LINE__);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "123456789");

    rc = vtprintf_s(str1, 9, "%s", &str1[8]);
    ERR(1) /* overlapping allowed */
    ind = strcmp(str1, "9");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str2, "123");
    strcpy(str1, "keep it simple");

    rc = vtprintf_s(str2, 31, "%s", &str1[0]);
    NOERR()
    ind = strcmp(str2, "keep it simple");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str2, "1234");
    strcpy(str1, "56789");

    rc = vtprintf_s(str2, 10, "%s", str1);
    NOERR()
    ind = strcmp(str2, "56789");
    if (ind != 0) {
        debug_printf("%s %u   Error -%s- \n",
                     __FUNCTION__, __LINE__,  str1);
        errs++;
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_vsprintf_s());
}
#endif
