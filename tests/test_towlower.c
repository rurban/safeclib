/*------------------------------------------------------------------
 * test_towlower
 * Test the musl-inherited towupper/towlower regarding Unicode 9.0
 * CaseFolding.txt
 * Some F characters fold to multiples, which cannot be handled here.
 *
 *------------------------------------------------------------------
 */

/*#include "test_private.h"
  #include "safe_lib.h"*/
#include "../config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#ifndef HAVE_TOWLOWER
#define EXTERN
EXTERN wint_t towlower(wint_t wc);
#else
#include "../src/extwchar/towctrans.c"
#endif

#define CFOLD "CaseFolding.txt"

int ignore_f = 1;

int test_towlower (void)
{
    errno_t rc;
    int errs = 0;
    int c;
    char s[128];
    char code[8];
    char status[2];
    char mapping[24];
    char name[80];
    FILE *f;

    wint_t wc, m;

/*--------------------------------------------------*/

    f = fopen(CFOLD, "r");
    if (!f) {
        printf("downloading %s ...", CFOLD);
        fflush(stdout);
        system("wget ftp://ftp.unicode.org/Public/UNIDATA/CaseFolding.txt");
        printf(" done\n");
        f = fopen(CFOLD, "r");
    }
    while (!feof(f)) {
        int l;
        char *p = fgets(s, sizeof(s), f);
        char *p1;
        if (p && *p && s[0] != '#' && s[0] != '\n') {
            p = strstr(s, "; ");
            l = p-s;
            memcpy(code, s, l);
            code[l] = 0;
            *status = p[2];
            status[1] = 0;
            p1 = strstr(&p[5], "; ");
            l = p1-p-5;
            memcpy(mapping, &p[5], l);
            mapping[l] = 0;
            strcpy(name, &p1[4]);
            name[strlen(name)-1] = 0;

            c = sscanf(code, "%X", &wc);
            if (c) {
                wint_t m1;
                m = (unsigned)wc < 128 ? tolower(wc) : __towcase(wc, 1);
                c = sscanf(mapping, "%X", &m1);
                if (*status == 'T')
                    m1 = m;
                /* we have 104 unhandled F multi-char mappings */
                else if (*status == 'F') { /* lower is bigger than upper, ignored */
                    if (!ignore_f) {
                        if (m != wc)
                            printf("U+%04X => U+%04X lower=(%s) F %s\n",
                                   wc, m, mapping, name);
                        else
                            printf("U+%04X lower=(%s) F %s\n", wc, mapping, name);
                    }
                }
                else if (m1 != m) {
                    m = __towcase(wc, 1);
                    if (wc != m)
                        printf("Error U+%04X => U+%04X lower=%s status=%s name=%s\n",
                               wc, m, mapping, status, name);
                    else
                        printf("Error U+%04X lower=%s status=%s name=%s\n",
                               wc, mapping, status, name);
                }
            } else {
                printf("Error code=%s status=%s lower=%s name=%s\n",
                       code, status, mapping, name);
            }
        }
    }
    fclose(f);
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_towlower());
}
#endif
