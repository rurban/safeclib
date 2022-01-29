/*------------------------------------------------------------------
 * test_towupper.c
 *
 * Test the musl-inherited towupper regarding latest Unicode 13.0
 * Unicode has no explicit lower->upper mapping document.
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#ifdef HAVE_TOWUPPER
#include <wctype.h>
#endif
#ifndef HAVE_TOWUPPER_OK
EXTERN uint32_t _towupper(uint32_t wc);
#endif

EXTERN uint32_t _towcase(uint32_t wc, int lower);
int check_casefolding(uint32_t lwr, uint32_t upr);
int check(uint32_t wc, const char *status, const char *name);
int test_towupper(void);

#define GENCAT "DerivedGeneralCategory.txt"
#define CFOLD "CaseFolding.txt"
#ifndef PERL
/* Must have the same Unicode version 9.0, at least 5.26.
   Better 5.27.3 with Unicode 10, 5.30 with 12.1, 5.32 with 13.0, 5.34 with 14.0
 */
/*# define PERL "perl" */
/*# define PERL "cperl5.30.0"*/
#define PERL "perl5.36.0"
#endif
#define TESTPL "test-upr.pl"

char s[128];
char code[16];
char status[3];
char mapping[24];
char name[80];
FILE *f, *cf, *pl;
static int init = 0;

/* TODO: try to malloc the whole table with 2 status bits.
   This linear-searches in the file buffer for extremely low-memory systems.
 */
int check_casefolding(uint32_t lwr, uint32_t upr) {
    rewind(cf);
    while (!feof(cf)) {
        int l;
        char *p = fgets(s, sizeof(s), cf);
        char *p1;
        uint32_t wc, mp;
        int c;
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
            if (!c)
                continue;
            if (wc != upr)
                continue;
            c = sscanf(mapping, "%X", &mp);
            if (!c)
                continue;
            if (lwr != mp) {
                if (*status != 'F') {
                    /* false positives */
                    debug_printf(
                        "%u U+%04X: U+%04X != U+%04X status=%s, name=%s\n",
                        __LINE__, wc, lwr, mp, status, name);
                    /* cross-check with perl */
                    if (!init) {
                        fprintf(pl, "use v5.31.8;\n");
                        fprintf(pl, "use Unicode::UCD;\n");
                        fprintf(pl, "print Unicode::UCD::UnicodeVersion(), \" must be 13.0.0\\n\";\n");
                        fprintf(pl, "my ($l,$u,$got);\n");
                        init = 1;
                    }
                    fprintf(pl,
                            "$l=\"\\x{%X}\";$u=uc $l;$got=\"\\x{%X}\";"
                            "printf \"uc %X = %%X; got: %X\\n\","
                            "unpack(\"W*\",$u) if $u ne $got;\n",
                            lwr, lwr, upr, upr);
                    return 1;
                    /*} else {
                          printf("%u U+%04X: U+%04X != U+%04X status=F,
                       name=%s\n",
                          __LINE__, wc, lwr, mp, name);*/
                }
            }
            return 0;
        }
    }
    /*debug_printf("%u U+%04X Ll: not found in %s for U+%04X\n",
      __LINE__, upr, CFOLD, lwr);*/
    return 0;
}

/* checks a lower case letter from DerivedGeneralCategory */
int check(uint32_t wc, const char *_status, const char *_name) {
    int errs = 0;
    uint32_t upr;
    upr = wc < 128 ? (uint32_t)toupper(wc) : _towcase(wc, 0);
    if (upr == wc) {
        check_casefolding(wc, upr);
#ifdef HAVE_TOWUPPER_OK
        upr = (uint32_t)towupper(wc);
        if (upr != wc) {
            printf("%u Error towupper(U+%04X) = U+%04X status=%s, name=%s\n",
                   __LINE__, wc, upr, _status, _name);
            errs++;
            /* else system libc agrees with us.
               which says nothing. system wctype functions are generally
               extremely unreliable and outdated.
             */
        }
#elif defined HAVE_TOWUPPER
        upr = _towupper(wc);
        if (upr != wc) {
            printf("%u _towupper(U+%04X) = U+%04X status=%s, name=%s\n",
                   __LINE__, wc, upr, _status, _name);
        }
#endif
    }
    return errs;
}

int test_towupper(void) {
    int errs = 0;
    int lowfound = 0;
    uint32_t wc;
    struct stat st;

    /*--------------------------------------------------*/

    pl = fopen(TESTPL, "w");
    f = fopen(GENCAT, "r");
    if (!f) {
        printf("downloading %s ...", GENCAT);
        fflush(stdout);
        if (system("wget https://www.unicode.org/Public/14.0.0/ucd/extracted/"
                   "DerivedGeneralCategory.txt"))
            printf(" done\n");
        else {
            printf(" failed\n");
            return 0;
        }
        f = fopen(GENCAT, "r");
        if (!f)
            return 0;
    }

    cf = fopen(CFOLD, "r");
    if (!cf) {
        printf("downloading %s ...", CFOLD);
        fflush(stdout);
        if (system("wget https://www.unicode.org/Public/14.0.0/ucd/CaseFolding.txt"))
            printf(" done\n");
        else {
            printf(" failed\n");
            return 0;
        }
        cf = fopen(CFOLD, "r");
        if (!cf)
            return 0;
    }

    while (!feof(f)) {
        int l;
        char *p = fgets(s, sizeof(s), f);
        char *p1;
        /* # General_Category=Lowercase_Letter */
        if (!(p && *p))
            continue;
        if (!lowfound) {
            if (!strcmp(p, "# General_Category=Lowercase_Letter\n")) {
                lowfound = 1;
            }
            continue;
        }
        if (!memcmp(p, "# General_Category=", 19)) {
            lowfound = 0;
            continue;
        }
        if (s[0] != '#' && s[0] != '\n') {
            uint32_t from, to;
            int c;
            p = strstr(s, "; ");
            if (!p)
                continue;
            l = p - s;
            memcpy(code, s, l);
            code[l] = 0; /* uppercase only */
            status[0] = p[2];
            status[1] = p[3];
            status[2] = 0;
            p1 = strstr(&p[5], "# ");
            l = p1 - p - 5;
            strncpy(name, &p1[3], 79);
            name[strlen(name) - 1] = 0;
            strremovews_s(name, 80);

            c = sscanf(code, "%X..%X", &from, &to);
            if (c) {
                for (wc = from; wc <= to; wc++) {
                    check(wc, status, name);
                }
            } else {
                c = sscanf(code, "%X", &wc); /* upr */
            }
            if (c) {
                errs += check(wc, status, name);
            } else {
                printf("!code=%s status=%s name=%s\n", code, status, name);
            }
        }
    }
    fclose(f);
    fclose(cf);
#ifdef BSD_ALL_LIKE
    fstat(pl->_file, &st);
    fclose(pl);
#elif defined __GLIBC__
    fstat(pl->_fileno, &st);
    fclose(pl);
#else
    fclose(pl);
    stat(TESTPL, &st);
#endif
    if (st.st_size) {
        printf("Cross check with " PERL ":\n");
        fflush(stdout);
        if (system(PERL " " TESTPL)) {
            printf("Redo with perl (probably wrong Unicode version):\n");
            fflush(stdout);
            if (!system("perl " TESTPL))
                printf("perl " TESTPL " failed\n");
        }
    }
#ifndef DEBUG
    unlink(TESTPL);
#endif
    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main(void) { return (test_towupper()); }
#endif
