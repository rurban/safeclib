/*------------------------------------------------------------------
 * test_strnterminate_s
 * File 'extstr/strnterminate_s.c'
 * Lines executed:86.96% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN (128)

int main(void) {
    rsize_t len;
    rsize_t std_len;
    rsize_t max_len;
    char dest[LEN];
    int errs = 0;

    /*--------------------------------------------------*/

    strcpy(dest, "");
    max_len = 3;
#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    len = strnterminate_s(NULL, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
#endif
    /*--------------------------------------------------*/

    strcpy(dest, "test");
    max_len = 0;
    len = strnterminate_s(dest, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "test");
    max_len = RSIZE_MAX_STR + 1;
    len = strnterminate_s(dest, max_len);
    if (len != 0) {
        debug_printf("%s %u   Len=%u \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "ff");
    std_len = strlen(dest);
    max_len = RSIZE_MAX_STR;

    len = strnterminate_s(dest, max_len);

#ifdef HAVE___BUILTIN_OBJECT_SIZE
    /* dmax > sizeof dest */
    if (std_len == len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
#else
    if (std_len != len) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
#endif
    /*--------------------------------------------------*/

    strcpy(dest, "ff");
    len = strnterminate_s(dest, 1);

    if (len != 0) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "f");
    max_len = LEN;
    len = strnterminate_s(dest, max_len);

    if (len != 1) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "f");
    max_len = LEN;
    len = strnterminate_s(dest, max_len);

    if (len != 1) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "ff");
    max_len = LEN;
    len = strnterminate_s(dest, max_len);

    if (len != 2) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "test");
    max_len = LEN;
    len = strnterminate_s(dest, max_len);

    if (len != 4) {
        debug_printf("%s %u   std_len=%u  len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)std_len, (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "testing");
    max_len = 1;
    len = strnterminate_s(dest, max_len);

    if (len != 0) {
        debug_printf("%s %u   len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "testing");
    max_len = 2;
    len = strnterminate_s(dest, max_len);

    if (len != max_len - 1) {
        debug_printf("%s %u   len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "testing");
    max_len = 3;
    len = strnterminate_s(dest, max_len);

    if (len != max_len - 1) {
        debug_printf("%s %u   len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "testing");
    max_len = 33;
    len = strnterminate_s(dest, max_len);

    if (len != strlen(dest)) {
        debug_printf("%s %u   len=%u  \n", __FUNCTION__, __LINE__,
                     (unsigned)len);
        errs++;
    }
    /*--------------------------------------------------*/

    strcpy(dest, "012345678901234567890");
    len = 21;

    while (len) {
        debug_printf(" strnterminate_s()  len=%u \n", (unsigned)len);
        len = strnterminate_s(dest, len);
        if (len != strlen(dest)) {
            debug_printf("%s %u   len=%u  \n", __FUNCTION__, __LINE__,
                         (unsigned)len);
            errs++;
        }
    }

    /*--------------------------------------------------*/

    return (errs);
}
