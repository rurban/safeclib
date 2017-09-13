/*------------------------------------------------------------------
 * test_towfc_s.c
 * File 'towfc_s.c'
 * Lines executed:94.78% of 115
 *
 * Full case-folding regarding latest Unicode (10.0) CaseFolding.txt
 * Some F characters fold to multiples.
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <stdlib.h>
#include <unistd.h>

#define CFOLD "CaseFolding.txt"

int test_towfc_s (void)
{
    errno_t rc;
    int ind;
    int errs = 0;
    int c;
    char s[128];
    char code[8];
    char status[2];
    char mapping[24];
    char name[80];
    FILE *f;

    wint_t wc;

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
                wint_t m0;
                int n, len;
                wchar_t result[4];

                result[0] = L'\0';
                n = iswfc(wc);

                rc = towfc_s(result, 4, wc);
                if (rc) {
                    ind = n;
                    ERR(ESNOTFND);
                    INDZERO();
                }
                else {
                    ind = n;
                    ERR(EOK);
                    /*INDCMP(<= 0);*/
                }
                len = wcslen(result);
                c = sscanf(mapping, "%X", &m0);

                /* we have 104 unhandled F multi-char mappings,
                   handle them via towfc() (wide full-lowercase) */
                if (*status == 'F') { /* lower is bigger than upper, ignored */
                    wint_t m1, m2;
                    /* check the length, must be or 3 */
                    if (n < 2) {
                        errs++;
                        printf("Error: iswfc(U+%04X) => %d (towfc=>%d) \"%s\" %s\n",
                               wc, n, len, mapping, name);
                    }
                    else {
                        static wchar_t cmp[4];
                        if (n != len) {
                            errs++;
                            printf("Error: towfc(U+%04X) => %d (iswfc=>%d) \"%s\" %s\n",
                                   wc, len, n, mapping, name);
                        }
                        /* also compare the content */
                        if (n == 2) {
                            c = sscanf(mapping, "%4X %4X", &m0, &m1);
                            cmp[2] = 0;
                        } else if (n == 3) {
                            c = sscanf(mapping, "%4X %4X %4X", &m0, &m1, &m2);
                            cmp[2] = (wchar_t)m2;
                            cmp[3] = 0;
                        } else {
                            errs++;
                            printf("Error: Wrong n=%d\n", n);
                            goto next_line;
                        }
                        cmp[0] = (wchar_t)m0;
                        cmp[1] = (wchar_t)m1;
                        ind = wcscmp(result, cmp);
                        INDZERO();
                        if (ind) {
                            printf("Error: towfc(U+%04X) => %X... \"%s\"\n",
                                   wc, result[0], mapping);
                            if (n == 3)
                                printf("    { 0x%04X, 0x%04X,0x%04X,0x%04X },\t/* %s */\n",
                                       wc, m0, m1, m2, name);
                            else
                                printf("    { 0x%04X, 0x%04X,0x%04X },\t/* %s */\n",
                                       wc, m0, m1, name);
                        }
                    }
                }
                else
                if (*status == 'T') { /* Turkish special case */
                    if (wc != 0x130 && wc != 0x49) {
                        printf("Error: Unknown turkish wc=%d\n", wc);
                        errs++;
                    } else {
                        ind = n;
                        /* this is turkish insensitive, only wcsfc_s checks the locale */
                        if (wc == 0x130) {
                            INDCMP(!= 2);
                            WEXPSTR(result, L"i̇");
                        }
                        else {
                            INDCMP(!= 1);
                            WEXPSTR(result, L"i");
                        }
                    }
                } else if (*status == 'S') { /* ignore as we handle the other F case */
                    ;
                } else { /* the simple 1:1 C case */
                    if (n > 1) {
                        errs++;
                        printf("Error: iswfc(U+%04X) => %d (towfc=>%d) \"%s\" status=%s %s\n",
                               wc, n, len, mapping, status, name);
                    } else if (result[0] != m0) {
                        errs++;
                        printf("Error: towfc(U+%04X) => %X \"%s\" status=%s %s\n",
                               wc, result[0], mapping, status, name);
                    }
                }
            }
        }
      next_line: ;
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
