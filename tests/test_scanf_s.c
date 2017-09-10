/*------------------------------------------------------------------
 * test_scanf_s
 * File 'io/scanf_s.c'
 * Lines executed:100.00% of 19
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];
#define TMP   "tmpscanf"
static FILE*  stream = NULL;

void stuff_stdin(char *restrict dest)
{
    stream = fopen(TMP, "w+");
    fprintf(stream, "%s\n", dest);
    fclose(stream);
    freopen(TMP, "r", stdin);
}
 
int test_scanf_s (void)
{
    errno_t rc;
    int32_t  ind;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    int errs = 0;
   
/*--------------------------------------------------*/

    rc = scanf_s(NULL, NULL);
    ERREOF(ESNULLP);

    /* TODO: should error */
#if 0
    rc = scanf_s("%s", NULL);
    ERREOF(ESNULLP);
#endif

/*--------------------------------------------------*/

    stuff_stdin("      24");
    rc = scanf_s("%s %%n", str2, LEN);
#ifdef BSD_LIKE
    if (rc != 0) { /* BSD's return -1 on %%n */
        printf("%s %u wrong fscanf(\"\",L\"%%n\"): %d\n",
                     __FUNCTION__, __LINE__, (int)rc);
    } else
#endif
    ERR(1);
    ERRNO(0);

    stuff_stdin("      24");
    rc = scanf_s("%s %%n", str2, 6);
    ERR(1);
    ERRNO(0);
    EXPSTR(str2, "24");

    /*stuff_stdin("      24");
    rc = scanf_s("%s %%n", str3, 6);
    ERR(1);
    ERRNO(0);
    EXPSTR(str3, "24");*/

    stuff_stdin("      24");
    rc = scanf_s(" %d", &len1);
    ERR(1);
    ERRNO(0);
    if ((int)len1 != 24) {
        debug_printf("%s %u wrong arg: %d\n",
                     __FUNCTION__, __LINE__, (int)len1);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    len1 = strlen(str1);
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    ERR(1)
    len2 = strlen(str2);
    len3 = strlen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        size_t len1 = strlen(str1);
#endif
        debug_printf("%s %u lengths wrong: %d  %d  %d \n",
                     __FUNCTION__, __LINE__, (int)len1, (int)len2, (int)len3);
        errs++;
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    ERR(1);
    EXPSTR(str2, "keep");

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    ERR(-1);
    EXPNULL(str2);

/*--------------------------------------------------*/

    str1[0] = '\0';
    stuff_stdin(str1);

    rc = scanf_s("%s", str2, LEN);
    ERR(-1)
    EXPNULL(str2);

/*--------------------------------------------------*/

    strcpy(str1, "qqweqq");
    strcpy(str2, "keep it simple");
    stuff_stdin(str1);

    rc = scanf_s("%s", str2);
    NOERR()
    EXPSTR(str2, str1);

/*--------------------------------------------------*/

    /* overlapping works fine on darwin, different on linux glibc */
    /*
    strcpy(str1, "12345678901234567890");

    rc = scanf_s("%s", &str1[7]);
    ERR(1);
    EXPSTR(str1, "123456712345678901234567890");

    strcpy(str1, "123456789");

    rc = scanf_s("%s", &str1[8]);
    ERR(1);
    EXPSTR(str1, "12345678123456789");
    */

/*--------------------------------------------------*/

    stuff_stdin("      24");
    rc = scanf_s("%s %n", str2, LEN, &ind);
    ERREOF(EINVAL);

/*--------------------------------------------------*/

#ifndef __GLIBC__
    fclose(stream); /* crashes with linux glibc: double free */
#endif

#if 0
    strcpy(str1, "qqweqq");
    stuff_stdin(str1);
    /* would block reading */
    rc = scanf_s("%s", str2, LEN);
#endif

/*--------------------------------------------------*/

    unlink(TMP);
    
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_scanf_s());
}
#endif
