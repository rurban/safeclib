/*------------------------------------------------------------------
 * test_strcasestr_s
 * File 'extstr/strcasestr_s.c'
 * Lines executed:100.00% of 43
 *
 *
 *------------------------------------------------------------------
 */

#define _GNU_SOURCE
#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

int main()
{
    errno_t rc;
    char *sub;
#ifdef HAVE_STRCASESTR
    char *std_sub;
#endif

    rsize_t len1;
    rsize_t len2;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcasestr_s(NULL, LEN, str2, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    SUBNULL()

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, str2, LEN, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = strcasestr_s(str1, 0, str2, LEN, &sub);
    ERR(ESZEROL)
    SUBNULL()

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = strcasestr_s(str1, RSIZE_MAX_STR+1, str2, LEN, &sub);
    ERR(ESLEMAX)
    SUBNULL()

    rc = strcasestr_s(str1, LEN, str2, RSIZE_MAX_STR+1, &sub);
    ERR(ESLEMAX)
    SUBNULL()
#endif
/*--------------------------------------------------*/

    rc = strcasestr_s(str1, LEN, str2, 0, &sub);
    ERR(ESZEROL)
    SUBNULL()

/*--------------------------------------------------*/

    *str1 = '\0';
    *str2 = '\0';

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /* compare to legacy (not counting as error) */
#ifdef HAVE_STRCASESTR
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        debug_printf("%s %u  Legacy difference rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }
#endif

/*--------------------------------------------------*/

    *str1 = '\0';
    strcpy(str1, "key");

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /* compare to legacy */
#ifdef HAVE_STRCASESTR
    std_sub = strcasestr(str1, str2);
    PTREQ(sub, std_sub)
#endif

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    *str2 = '\0';

    /* str2 being empty, must return str1 */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, str1)

    /* compare to legacy */
#if defined(HAVE_STRCASESTR)
    std_sub = strcasestr(str1, str2);
    PTREQ(sub, std_sub)
#endif

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "kee");

    /* substring at beginning */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[0])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "eep it");

    /* substring in the middle */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[1])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "ethe");

    /* substring in the middle */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[15])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    /* substring at the end */
    rc = strcasestr_s(str1, len1, str2, len2, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[18])
    /* compare to legacy */
#ifdef HAVE_STRCASESTR
    std_sub = strcasestr(str1, str2);
    PTREQ(sub, std_sub)
#endif

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "er");

    len1 = strlen(str1);
    len2 = strlen(str2);

    /* substring at the end */
    rc = strcasestr_s(str1, len1, str2, 2, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[18])
    /* compare to legacy */
#ifdef HAVE_STRCASESTR
    std_sub = strcasestr(str1, str2);
    PTREQ(sub, std_sub)
#endif

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, 3, str2, LEN, &sub);
    ERR(ESNOTFND)
    SUBNULL()

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 1, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[5])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 2, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[5])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "it all");

    rc = strcasestr_s(str1, LEN, str2, 5, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[5])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "1234");

    len1 = strlen(str1);
    rc = strcasestr_s(str1, len1, str2, LEN, &sub);
    ERR(ESNOTFND)
    SUBNULL()

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "IT ALL");

    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[5])

/*--------------------------------------------------*/

    strcpy(str1, "keep it all together");
    strcpy(str2, "EEP");

    /* validate */
    rc = strcasestr_s(str1, LEN, str2, LEN, &sub);
    ERR(EOK)
    PTREQ(sub, &str1[1])

    /* compare to legacy */
#ifdef HAVE_STRCASESTR
    std_sub = strcasestr(str1, str2);
    if (sub != std_sub) {
        debug_printf("%s %u  Legacy difference rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
    }
#endif

/*--------------------------------------------------*/

    return (errs);
}
