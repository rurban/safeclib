/*------------------------------------------------------------------
 * test_wmemcmp_s
 * File 'extwchar/wmemcmp_s.c'
 * Lines executed:100.00% of 38
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 128 )

#define sgn(i) ((i)>0 ? 1 : ((i)<0 ? -1 : 0))

int main()
{
    errno_t rc;

    uint32_t len;
    int32_t  ind;
    int32_t  std_ind;
    wchar_t i;

    wchar_t  mem1[LEN];
    wchar_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = wmemcmp_s(NULL, LEN, mem2, LEN, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, LEN, NULL, LEN, &ind);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, LEN, mem2, LEN, NULL);
    if (rc != ESNULLP) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, 0, mem2, LEN, &ind);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, LEN, mem2, 0, &ind);
    if (rc != ESZEROL) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = wmemcmp_s(mem1, RSIZE_MAX_WMEM+1, mem2, LEN, &ind);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
#endif
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = wmemcmp_s(mem1, LEN, mem2, RSIZE_MAX_WMEM+1, &ind);
    if (rc != ESLEMAX) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
#endif
/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, LEN-2, mem2, LEN, &ind);
    if (rc != ESNOSPC) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }

/*--------------------------------------------------*/

    rc = wmemcmp_s(mem1, LEN, mem1, LEN, &ind);
    if (rc != 0 || ind) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    len = 1;
    rc = wmemcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }
#ifdef HAVE_WMEMCMP
    std_ind = wmemcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    len = 2;
    rc = wmemcmp_s(mem1, len, mem2, len, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }
#ifdef HAVE_WMEMCMP
    std_ind = wmemcmp(mem1, mem2, len);
    if (ind != std_ind) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    rc = wmemcmp_s(mem1, LEN, mem2, LEN, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
    if (ind != 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }
#ifdef HAVE_WMEMCMP
    std_ind = wmemcmp(mem1, mem2, LEN);
    if (ind != std_ind && ind != sgn(std_ind)) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = i; }
    for (i=0; i<LEN; i++) { mem2[i] = i; }

    for (i=10; i<LEN-10; i++) { mem2[i] = 5; }

    rc = wmemcmp_s(mem1, LEN, mem2, LEN, &ind);
    if (rc != EOK) {
        debug_printf("%s %u  Ind=%d  Error rc=%u \n",
                     __FUNCTION__, __LINE__, ind, rc );
        errs++;
    }
    if (ind == 0) {
        debug_printf("%s %u  Ind=%d  rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, rc );
        errs++;
    }

#ifdef HAVE_WMEMCMP
    /* ANSI C may return more then signum here: 1 instead of 5 */
    std_ind = wmemcmp(mem1, mem2, LEN);
    if (ind != std_ind && ind != sgn(std_ind)) {
        debug_printf("%s %u  Ind=%d  std_ind=%d rc=%u \n",
                     __FUNCTION__, __LINE__,  ind, std_ind, rc );
        errs++;
    }
#endif

/*--------------------------------------------------*/

    return (errs);
}
