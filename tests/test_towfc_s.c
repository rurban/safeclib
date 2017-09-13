/*------------------------------------------------------------------
 * test_towfc_s.c
 * File 'towfc_s.c'
 * Lines executed:91.55% of 71
 *
 * Full case-folding regarding latest Unicode (10.0) CaseFolding.txt
 * Some F characters fold to multiples.
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
#include <wchar.h>
#include <errno.h>
#ifndef HAVE_TOWLOWER
#define EXTERN
EXTERN wint_t towlower(wint_t wc);
#else
#include "../src/extwchar/towctrans.c"
#endif

int     iswfc(wint_t wc);
errno_t towfc_s(wchar_t *restrict dest, rsize_t dmax, const wchar_t *restrict src);

#define CFOLD "CaseFolding.txt"

int ignore_f = 1;

int test_towfc_s (void)
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
                int n;
                errno_t rc;
                wchar_t tmp[4];
                wchar_t s[2];

                tmp[0] = L'\0';
                n = iswfc(wc);

                c = sscanf(mapping, "%X", &m1);
                s[0] = wc;
                s[1] = 0;
                rc = towfc_s(tmp, 4, s);

                if (*status == 'T')
                    m1 = m; /* ignore turkish special rule */
                /* we have 104 unhandled F multi-char mappings,
                   handle them via towfc() (wide full-lowercase)
                 */
                else if (*status == 'F') { /* lower is bigger than upper, ignored */
                    const wchar_t cmp[4];
                    int len = wcslen(tmp);
                    if (n < 2)
                        printf("Error: iswfc(U+%04X) => %d (towfc=>%d) \"%s\" %s\n",
                               wc, n, len, mapping, name);
                    else if (n != len)
                        printf("Error: towfc(U+%04X) => %d (iswfc=>%d) \"%s\" %s\n",
                               wc, len, n, mapping, name);
                    /*wsprintf(cmp, "%s");
                      int diff = wcscmp(tmp, mapping);*/
                    if (0) {
                    if (m != wc) {
                        printf("    { 0x%04X, %s },\t/* %s */\n",
                               wc, mapping, name);
                            /*printf("U+%04X => U+%04X lower=(%s) F %s\n",
                              wc, m, mapping, name);*/
                    }
                    else {
                        printf("    { 0x%04X, %s },\t/* %s */\n",
                               wc, mapping, name);
                        /*printf("U+%04X lower=(%s) F %s\n", wc, mapping, name);*/
                    }
                    }
                }
                else if (0 && m1 != m) {
                    m = __towcase(wc, 1);
                    if (wc != m)
                        printf("Error U+%04X => U+%04X lower=%s status=%s name=%s\n",
                               wc, m, mapping, status, name);
                    else
                        printf("Error U+%04X lower=%s status=%s name=%s\n",
                               wc, mapping, status, name);
                }
            } else {
                printf("Error code=%s status=%s lower=%s name=%s\n", code, status, mapping, name);
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
    return (test_towfc_s());
}
#endif
