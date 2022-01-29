/*------------------------------------------------------------------
 * test_towlower.c
 *
 * Test our musl-inherited towupper/towlower regarding Unicode 12.0
 * CaseFolding.txt
 * Some F characters fold to multiples, which cannot be handled here,
 * see test_towcf_s.c instead.
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <ctype.h>

EXTERN uint32_t _towcase(uint32_t wc, int lower);
int test_towlower(void);

#define CFOLD "CaseFolding.txt"

int ignore_f = 1;

int test_towlower(void) {
    int errs = 0;
    int c;
    char s[128];
    char code[8];
    char status[2];
    char mapping[24];
    char name[80];
    FILE *f;

    uint32_t wc, lwr;

    /*--------------------------------------------------*/

    f = fopen(CFOLD, "r");
    if (!f) {
        printf("downloading %s ...", CFOLD);
        fflush(stdout);
        if (system("wget https://www.unicode.org/Public/14.0.0/ucd/CaseFolding.txt"))
            printf(" done\n");
        else {
            printf(" failed\n");
            return 0;
        }
        f = fopen(CFOLD, "r");
        if (!f)
            return 0;
    }
    while (!feof(f)) {
        int l;
        char *p = fgets(s, sizeof(s), f);
        char *p1;
        if (p && *p && s[0] != '#' && s[0] != '\n') {
            p = strstr(s, "; ");
            l = p - s;
            memcpy(code, s, l);
            code[l] = 0;
            *status = p[2];
            status[1] = 0;
            p1 = strstr(&p[5], "; ");
            l = p1 - p - 5;
            memcpy(mapping, &p[5], l); /* the other cases */
            mapping[l] = 0;
            strcpy(name, &p1[4]);
            name[strlen(name) - 1] = 0;

            c = sscanf(code, "%X", &wc);
            if (c) {
                uint32_t mp;
                lwr = wc < 128 ? (uint32_t)tolower(wc) : _towcase(wc, 1);
                c = sscanf(mapping, "%X", &mp);
                if (*status == 'T')
                    mp = lwr;
                /* we have 104 unhandled F multi-char mappings */
                else if (*status ==
                         'F') { /* lower is bigger than upper, ignored */
                    if (!ignore_f) {
                        if (lwr != wc)
                            printf("U+%04X => U+%04X lower=(%s) F %s\n", wc,
                                   lwr, mapping, name);
                        else
                            printf("U+%04X lower=(%s) F %s\n", wc, mapping,
                                   name);
                    }
                } else if (mp != lwr) {
                    lwr = _towcase(wc, 1);
                    if (wc != lwr)
                        printf("Error U+%04X => U+%04X lower=%s status=%s "
                               "name=%s\n",
                               wc, lwr, mapping, status, name);
                    else
                        printf("Error U+%04X lower=%s status=%s name=%s\n", wc,
                               mapping, status, name);
                }
            } else {
                printf("Error code=%s status=%s lower=%s name=%s\n", code,
                       status, mapping, name);
            }
        }
    }
    fclose(f);
    return (errs);
}

int main(void) { return (test_towlower()); }
