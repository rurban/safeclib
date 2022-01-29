/*------------------------------------------------------------------
 * test_towfc_s.c
 * File 'towfc_s.c'
 * Lines executed:98.28% of 174
 *
 * Full case-folding regarding latest Unicode (14.0) CaseFolding.txt
 * Some F characters fold to multiples.
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <unistd.h>

#define CFOLD "CaseFolding.txt"

#if SIZEOF_WCHAR_T > 2
#define MAX_LEN 4
#define _dec_w16(src) *(src)
#else
#define MAX_LEN 8
EXTERN uint32_t _dec_w16(wchar_t *src);
#endif
int test_towfc_s(void);

int test_towfc_s(void) {
    int rc;
    int ind;
    int errs = 0;
    int c;
    wchar_t result[MAX_LEN];
    char s[128];
    char code[8];
    char status[2];
    char mapping[24];
    char name[80];
    FILE *f;

    uint32_t wc;

    /*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    EXPECT_BOS("empty dest")
    rc = towfc_s(NULL, MAX_LEN, 1);
    NEGERR(ESNULLP);

    EXPECT_BOS("dmax underflow <4")
    rc = towfc_s(result, 3, 1);
    NEGERR(ESLEMIN);

    EXPECT_BOS("dest overflow")
    rc = towfc_s(result, RSIZE_MAX_WSTR + 1, 1);
    NEGERR(ESLEMAX);

    if (_BOS_KNOWN(result)) {
        EXPECT_BOS("dest overflow")
        rc = towfc_s(result, MAX_LEN + 1, 1);
        NEGERR(EOVERFLOW);
    }
#endif

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
            memcpy(mapping, &p[5], l);
            mapping[l] = 0;
            strcpy(name, &p1[4]);
            if (strlen(name))
                name[strlen(name) - 1] = 0;

            c = sscanf(code, "%X", &wc);
            if (c) {
                uint32_t m0;
                int n, len;
                uint32_t cp;

                result[0] = L'\0';
                n = iswfc(wc);

                rc = towfc_s(result, MAX_LEN, wc);
                len = wcslen(result);
                cp = _dec_w16(result);
                if (cp > 0x20000) /* XXX */
                    cp -= 0x10000;
                if (rc < 0) {
                    ERR(ESNOTFND);
                    if (n) {
                        printf("%s %u  Error %d U+%04X n=%d 0x%x\n",
                               __FUNCTION__, __LINE__, rc, wc, n, (unsigned)cp);
                        errs++;
                    }
                }
#if SIZEOF_WCHAR_T > 2
                else if (rc != n && n) { /* n may be 0, but fc 1 */
                    printf("%s %u  Error %d U+%04X n=%d 0x%x\n", __FUNCTION__,
                           __LINE__, rc, wc, n, (int)cp);
                    errs++;
                }
#endif
                c = sscanf(mapping, "%X", &m0);

                /* we have 104 unhandled F multi-char mappings,
                   handle them via towfc() (wide full-lowercase) */
                if (*status == 'F') { /* lower is bigger than upper, ignored */
                    uint32_t m1, m2;
                    /* check the length, must be or 3 */
                    if (n < 2) {
                        errs++;
                        printf("%s %u  Error: iswfc(U+%04X) => %d (towfc=>%d) "
                               "\"%s\" %s\n",
                               __FUNCTION__, __LINE__, wc, n, len, mapping,
                               name);
                    } else {
                        static wchar_t cmp[MAX_LEN];
                        if (n != len) {
                            errs++;
                            printf("%s %u  Error: towfc(U+%04X) => %d "
                                   "(iswfc=>%d) \"%s\" %s\n",
                                   __FUNCTION__, __LINE__, wc, len, n, mapping,
                                   name);
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
                            printf("%s %u  Error: Wrong n=%d\n", __FUNCTION__,
                                   __LINE__, n);
                            goto next_line;
                        }
                        cmp[0] = (wchar_t)m0;
                        cmp[1] = (wchar_t)m1;
                        ind = wcscmp(result, cmp);
                        INDZERO();
                        if (ind) {
                            printf(
                                "%s %u  Error: towfc(U+%04X) => %X... \"%s\"\n",
                                __FUNCTION__, __LINE__, wc, cp, mapping);
                            if (n == 3)
                                printf("    { 0x%04X, 0x%04X,0x%04X,0x%04X "
                                       "},\t/* %s */\n",
                                       wc, m0, m1, m2, name);
                            else
                                printf("    { 0x%04X, 0x%04X,0x%04X },\t/* %s "
                                       "*/\n",
                                       wc, m0, m1, name);
                        }
                    }
                } else if (*status == 'T') { /* Turkish special case */
                    if (wc != 0x130 && wc != 0x49) {
                        printf("%s %u  Error: Unknown turkish wc=%d\n",
                               __FUNCTION__, __LINE__, wc);
                        errs++;
                    } else {
                        ind = n;
                        /* this is turkish insensitive, only wcsfc_s checks the
                         * locale */
                        if (wc == 0x130) {
                            INDCMP(!= 2);
                            if (_BOS_KNOWN(result)) {
                              WEXPSTR(result, L"i̇");
                            }
                        } else {
                            INDCMP(!= 1);
                            WEXPSTR(result, L"i");
                        }
                    }
                } else if (*status == 'S') { /* ignore as we handle the other F case */
                    ;
                } else { /* the simple 1:1 C case */
#if SIZEOF_WCHAR_T > 2
                    if (n > 1)
#else
                    if (n > 2)
#endif
                    {
                        errs++;
                        printf("%s %u  Error: iswfc(U+%04X) => %d (towfc=>%d) "
                               "\"%s\" status=%s %s\n", __FUNCTION__, __LINE__, wc,
                               n, len, mapping, status, name);
                    } else if (cp != m0) {
                        errs++;
                        printf("%s %u  Error: towfc(U+%04X) => %X != %X \"%s\" "
                               "status=%s %s\n", __FUNCTION__, __LINE__, wc, cp, m0,
                               mapping, status, name);
                    }
                }
            }
        }
    next_line:;
    }
    fclose(f);
    return (errs);
}

int main(void) { return (test_towfc_s()); }
