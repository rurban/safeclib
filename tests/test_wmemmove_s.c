/*------------------------------------------------------------------
 * test_wmemmove_s
 * File 'wchar/wmemmove_s.c'
 * Lines executed:100.00% of 23
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#ifdef HAVE_WMEMMOVE_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 1024 )

int main()
{
    errno_t rc;
    uint32_t i;
    rsize_t len;

    wchar_t  mem1[LEN];
    wchar_t  mem2[LEN];
    int errs = 0;

/*--------------------------------------------------*/
    print_msvcrt(use_msvcrt);

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = wmemmove_s(NULL, LEN, mem2, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, sizeof(wchar_t));
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = wmemmove_s(mem1, 0, mem2, LEN);
    ERR_MSVC(ESZEROL, ERANGE); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, sizeof(wchar_t));
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }
    rc = wmemmove_s(mem1, RSIZE_MAX_WMEM+1, mem2, LEN);
    ERR_MSVC(ESLEMAX, 0); /* and untouched */
    if (!use_msvcrt)
        EXPMEM(mem1, 0, LEN, 33, sizeof(wchar_t));
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = wmemmove_s(mem1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, sizeof(wchar_t));
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = wmemmove_s(mem1, 10, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, 10, 33, sizeof(wchar_t));
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    rc = wmemmove_s(mem1, LEN, mem2, RSIZE_MAX_WMEM+1);
    ERR_MSVC(ESLEMAX, ERANGE); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, sizeof(wchar_t));
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* a valid move */
    len = LEN;
    rc = wmemmove_s(mem1, len, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, sizeof(wchar_t));

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* length error */
    len = LEN/sizeof(wchar_t);
    rc = wmemmove_s(mem1, len, mem2, LEN);
    ERR_MSVC(ESNOSPC, ERANGE); /* and cleared */
    EXPMEM(mem1, 0, len, 0, sizeof(wchar_t));

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */
    len = LEN;
    rc = wmemmove_s(mem1, len, mem2, 0);
    ERR(EOK);  /* and untouched */
    EXPMEM(mem1, 0, len, 33, sizeof(wchar_t));

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* invalid length - zero dest */
#ifndef HAVE_CT_BOS_OVR
    len = LEN;
    rc = wmemmove_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    ERR_MSVC(ESLEMAX, ERANGE); /* and cleared */
    EXPMEM(mem1, 0, len, 0, sizeof(wchar_t));
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* same ptr - no move */
    rc = wmemmove_s(mem1, LEN, mem1, LEN);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 55, sizeof(wchar_t));

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = wmemmove_s(&mem1[0], len, &mem1[10], len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 35, sizeof(wchar_t));

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=10; i<LEN-10; i++) { mem1[i] = 35; }

    /* overlap move */
    len = 20;
    rc = wmemmove_s(&mem1[10], len, &mem1[0], len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, 10, 25, sizeof(wchar_t));

/*--------------------------------------------------*/

    return (errs);
}
