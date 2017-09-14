/* 
 * This file is built by cperl regen/regcharclass-safec.pl.
 * cp_high means only codepoints > 0xff
 */

#ifndef H_HANGUL
#define H_HANGUL 1

/*
	HANGUL (Korean) has special normalization rules.
        Unicode 9.0
*/

#define Hangul_SBase  0xAC00
#define Hangul_SFinal 0xD7A3
#define Hangul_SCount  11172

#define Hangul_NCount    588

#define Hangul_LBase  0x1100
#define Hangul_LFinal 0x1112
#define Hangul_LCount     19

#define Hangul_VBase  0x1161
#define Hangul_VFinal 0x1175
#define Hangul_VCount     21

#define Hangul_TBase  0x11A7
#define Hangul_TFinal 0x11C2
#define Hangul_TCount     28

#define Hangul_IsS(u)  ((Hangul_SBase <= (u)) && ((u) <= Hangul_SFinal))
#define Hangul_IsN(u)  (((u) - Hangul_SBase) % Hangul_TCount == 0)
#define Hangul_IsLV(u) (Hangul_IsS(u) && Hangul_IsN(u))
#define Hangul_IsL(u)  ((Hangul_LBase <= (u)) && ((u) <= Hangul_LFinal))
#define Hangul_IsV(u)  ((Hangul_VBase <= (u)) && ((u) <= Hangul_VFinal))
#define Hangul_IsT(u)  ((Hangul_TBase  < (u)) && ((u) <= Hangul_TFinal))

/* much better would be the simple Hangul_IsS(cp) check */
/*** GENERATED CODE for all Hangul codepoints ***/
#define is_HANGUL_cp_high(cp)                               \
    ( ( 0x1100 <= cp && cp <= 0x11FF ) || ( 0x11FF < cp &&  \
    ( 0x302E == cp || ( 0x302E < cp &&                      \
    ( 0x302F == cp || ( 0x302F < cp &&                      \
    ( ( 0x3131 <= cp && cp <= 0x318E ) || ( 0x318E < cp &&  \
    ( ( 0x3200 <= cp && cp <= 0x321E ) || ( 0x321E < cp &&  \
    ( ( 0x3260 <= cp && cp <= 0x327E ) || ( 0x327E < cp &&  \
    ( ( 0xA960 <= cp && cp <= 0xA97C ) || ( 0xA97C < cp &&  \
    ( ( 0xAC00 <= cp && cp <= 0xD7A3 ) || ( 0xD7A3 < cp &&  \
    ( ( 0xD7B0 <= cp && cp <= 0xD7C6 ) || ( 0xD7C6 < cp &&  \
    ( ( 0xD7CB <= cp && cp <= 0xD7FB ) || ( 0xD7FB < cp &&  \
    ( ( 0xFFA0 <= cp && cp <= 0xFFBE ) || ( 0xFFBE < cp &&  \
    ( ( 0xFFC2 <= cp && cp <= 0xFFC7 ) || ( 0xFFC7 < cp &&  \
    ( ( 0xFFCA <= cp && cp <= 0xFFCF ) || ( 0xFFCF < cp &&  \
    ( ( 0xFFD2 <= cp && cp <= 0xFFD7 ) || ( 0xFFDA <= cp && cp <= 0xFFDC ) \
    )))))))))))))))))))))))))) )

#endif /* H_HANGUL */
