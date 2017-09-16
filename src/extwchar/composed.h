/* ex: set ro ft=c: -*- mode: c; buffer-read-only: t -*-
 * This file is built by cperl regen/regcharclass-safec.pl
 * cp_high means only codepoints > 0xff
 */

#ifndef H_COMPOSED   /* Guard against nested #includes */
#define H_COMPOSED 1

/*
	COMPOSED: If it starts with a composed codepoint which can be decomposed

	&regcharclass_multi_char_folds::composed_all_first()
*/
/*** GENERATED CODE ***/
#define is_COMPOSED_cp_high(cp)                                                 \
    ((0x100<=cp&&cp<=0x10F)||(0x10F<cp&&                                        \
    ((0x112<=cp&&cp<=0x125)||(0x125<cp&&                                        \
    ((0x128<=cp&&cp<=0x130)||(0x130<cp&&                                        \
    ((0x134<=cp&&cp<=0x137)||(0x137<cp&&                                        \
    ((0x139<=cp&&cp<=0x13E)||(0x13E<cp&&                                        \
    ((0x143<=cp&&cp<=0x148)||(0x148<cp&&                                        \
    ((0x14C<=cp&&cp<=0x151)||(0x151<cp&&                                        \
    ((0x154<=cp&&cp<=0x165)||(0x165<cp&&                                        \
    ((0x168<=cp&&cp<=0x17E)||(0x17E<cp&&                                        \
    (0x1A0==cp||(0x1A0<cp&&                                                     \
    (0x1A1==cp||(0x1A1<cp&&                                                     \
    (0x1AF==cp||(0x1AF<cp&&                                                     \
    (0x1B0==cp||(0x1B0<cp&&                                                     \
    ((0x1CD<=cp&&cp<=0x1DC)||(0x1DC<cp&&                                        \
    ((0x1DE<=cp&&cp<=0x1E3)||(0x1E3<cp&&                                        \
    ((0x1E6<=cp&&cp<=0x1F0)||(0x1F0<cp&&                                        \
    (0x1F4==cp||(0x1F4<cp&&                                                     \
    (0x1F5==cp||(0x1F5<cp&&                                                     \
    ((0x1F8<=cp&&cp<=0x21B)||(0x21B<cp&&                                        \
    (0x21E==cp||(0x21E<cp&&                                                     \
    (0x21F==cp||(0x21F<cp&&                                                     \
    ((0x226<=cp&&cp<=0x233)||(0x233<cp&&                                        \
    (0x385==cp||(0x385<cp&&                                                     \
    (0x386==cp||(0x386<cp&&                                                     \
    ((0x388<=cp&&cp<=0x38A)||(0x38A<cp&&                                        \
    (0x38C==cp||(0x38C<cp&&                                                     \
    ((0x38E<=cp&&cp<=0x390)||(0x390<cp&&                                        \
    ((0x3AA<=cp&&cp<=0x3B0)||(0x3B0<cp&&                                        \
    ((0x3CA<=cp&&cp<=0x3CE)||(0x3CE<cp&&                                        \
    (0x3D3==cp||(0x3D3<cp&&                                                     \
    (0x3D4==cp||(0x3D4<cp&&                                                     \
    (0x400==cp||(0x400<cp&&                                                     \
    (0x401==cp||(0x401<cp&&                                                     \
    (0x403==cp||(0x403<cp&&                                                     \
    (0x407==cp||(0x407<cp&&                                                     \
    ((0x40C<=cp&&cp<=0x40E)||(0x40E<cp&&                                        \
    (0x419==cp||(0x419<cp&&                                                     \
    (0x439==cp||(0x439<cp&&                                                     \
    (0x450==cp||(0x450<cp&&                                                     \
    (0x451==cp||(0x451<cp&&                                                     \
    (0x453==cp||(0x453<cp&&                                                     \
    (0x457==cp||(0x457<cp&&                                                     \
    ((0x45C<=cp&&cp<=0x45E)||(0x45E<cp&&                                        \
    (0x476==cp||(0x476<cp&&                                                     \
    (0x477==cp||(0x477<cp&&                                                     \
    (0x4C1==cp||(0x4C1<cp&&                                                     \
    (0x4C2==cp||(0x4C2<cp&&                                                     \
    ((0x4D0<=cp&&cp<=0x4D3)||(0x4D3<cp&&                                        \
    (0x4D6==cp||(0x4D6<cp&&                                                     \
    (0x4D7==cp||(0x4D7<cp&&                                                     \
    ((0x4DA<=cp&&cp<=0x4DF)||(0x4DF<cp&&                                        \
    ((0x4E2<=cp&&cp<=0x4E7)||(0x4E7<cp&&                                        \
    ((0x4EA<=cp&&cp<=0x4F5)||(0x4F5<cp&&                                        \
    (0x4F8==cp||(0x4F8<cp&&                                                     \
    (0x4F9==cp||(0x4F9<cp&&                                                     \
    ((0x622<=cp&&cp<=0x626)||(0x626<cp&&                                        \
    (0x6C0==cp||(0x6C0<cp&&                                                     \
    (0x6C2==cp||(0x6C2<cp&&                                                     \
    (0x6D3==cp||(0x6D3<cp&&                                                     \
    (0x929==cp||(0x929<cp&&                                                     \
    (0x931==cp||(0x931<cp&&                                                     \
    (0x934==cp||(0x934<cp&&                                                     \
    (0xB94==cp||(0xB94<cp&&                                                     \
    (0x1026==cp||(0x1026<cp&&                                                   \
    (0x1B06==cp||(0x1B06<cp&&                                                   \
    (0x1B08==cp||(0x1B08<cp&&                                                   \
    (0x1B0A==cp||(0x1B0A<cp&&                                                   \
    (0x1B0C==cp||(0x1B0C<cp&&                                                   \
    (0x1B0E==cp||(0x1B0E<cp&&                                                   \
    (0x1B12==cp||(0x1B12<cp&&                                                   \
    ((0x1E00<=cp&&cp<=0x1E99)||(0x1E99<cp&&                                     \
    (0x1E9B==cp||(0x1E9B<cp&&                                                   \
    ((0x1EA0<=cp&&cp<=0x1EF9)||(0x1EF9<cp&&                                     \
    ((0x1F00<=cp&&cp<=0x1F15)||(0x1F15<cp&&                                     \
    ((0x1F18<=cp&&cp<=0x1F1D)||(0x1F1D<cp&&                                     \
    ((0x1F20<=cp&&cp<=0x1F45)||(0x1F45<cp&&                                     \
    ((0x1F48<=cp&&cp<=0x1F4D)||(0x1F4D<cp&&                                     \
    ((0x1F50<=cp&&cp<=0x1F57)||(0x1F57<cp&&                                     \
    (0x1F59==cp||(0x1F59<cp&&                                                   \
    (0x1F5B==cp||(0x1F5B<cp&&                                                   \
    (0x1F5D==cp||(0x1F5D<cp&&                                                   \
    ((0x1F5F<=cp&&cp<=0x1F70)||(0x1F70<cp&&                                     \
    (0x1F72==cp||(0x1F72<cp&&                                                   \
    (0x1F74==cp||(0x1F74<cp&&                                                   \
    (0x1F76==cp||(0x1F76<cp&&                                                   \
    (0x1F78==cp||(0x1F78<cp&&                                                   \
    (0x1F7A==cp||(0x1F7A<cp&&                                                   \
    (0x1F7C==cp||(0x1F7C<cp&&                                                   \
    ((0x1F80<=cp&&cp<=0x1FB4)||(0x1FB4<cp&&                                     \
    ((0x1FB6<=cp&&cp<=0x1FBA)||(0x1FBA<cp&&                                     \
    (0x1FBC==cp||(0x1FBC<cp&&                                                   \
    ((0x1FC1<=cp&&cp<=0x1FC4)||(0x1FC4<cp&&                                     \
    ((0x1FC6<=cp&&cp<=0x1FC8)||(0x1FC8<cp&&                                     \
    (0x1FCA==cp||(0x1FCA<cp&&                                                   \
    ((0x1FCC<=cp&&cp<=0x1FD2)||(0x1FD2<cp&&                                     \
    ((0x1FD6<=cp&&cp<=0x1FDA)||(0x1FDA<cp&&                                     \
    ((0x1FDD<=cp&&cp<=0x1FE2)||(0x1FE2<cp&&                                     \
    ((0x1FE4<=cp&&cp<=0x1FEA)||(0x1FEA<cp&&                                     \
    (0x1FEC==cp||(0x1FEC<cp&&                                                   \
    (0x1FED==cp||(0x1FED<cp&&                                                   \
    ((0x1FF2<=cp&&cp<=0x1FF4)||(0x1FF4<cp&&                                     \
    ((0x1FF6<=cp&&cp<=0x1FF8)||(0x1FF8<cp&&                                     \
    (0x1FFA==cp||(0x1FFA<cp&&                                                   \
    (0x1FFC==cp||(0x1FFC<cp&&                                                   \
    (0x219A==cp||(0x219A<cp&&                                                   \
    (0x219B==cp||(0x219B<cp&&                                                   \
    (0x21AE==cp||(0x21AE<cp&&                                                   \
    ((0x21CD<=cp&&cp<=0x21CF)||(0x21CF<cp&&                                     \
    (0x2204==cp||(0x2204<cp&&                                                   \
    (0x2209==cp||(0x2209<cp&&                                                   \
    (0x220C==cp||(0x220C<cp&&                                                   \
    (0x2224==cp||(0x2224<cp&&                                                   \
    (0x2226==cp||(0x2226<cp&&                                                   \
    (0x2241==cp||(0x2241<cp&&                                                   \
    (0x2244==cp||(0x2244<cp&&                                                   \
    (0x2247==cp||(0x2247<cp&&                                                   \
    (0x2249==cp||(0x2249<cp&&                                                   \
    (0x2260==cp||(0x2260<cp&&                                                   \
    (0x2262==cp||(0x2262<cp&&                                                   \
    ((0x226D<=cp&&cp<=0x2271)||(0x2271<cp&&                                     \
    (0x2274==cp||(0x2274<cp&&                                                   \
    (0x2275==cp||(0x2275<cp&&                                                   \
    (0x2278==cp||(0x2278<cp&&                                                   \
    (0x2279==cp||(0x2279<cp&&                                                   \
    (0x2280==cp||(0x2280<cp&&                                                   \
    (0x2281==cp||(0x2281<cp&&                                                   \
    (0x2284==cp||(0x2284<cp&&                                                   \
    (0x2285==cp||(0x2285<cp&&                                                   \
    (0x2288==cp||(0x2288<cp&&                                                   \
    (0x2289==cp||(0x2289<cp&&                                                   \
    ((0x22AC<=cp&&cp<=0x22AF)||(0x22AF<cp&&                                     \
    ((0x22E0<=cp&&cp<=0x22E3)||(0x22E3<cp&&                                     \
    ((0x22EA<=cp&&cp<=0x22ED)||(0x22ED<cp&&                                     \
    (0x304C==cp||(0x304C<cp&&                                                   \
    (0x304E==cp||(0x304E<cp&&                                                   \
    (0x3050==cp||(0x3050<cp&&                                                   \
    (0x3052==cp||(0x3052<cp&&                                                   \
    (0x3054==cp||(0x3054<cp&&                                                   \
    (0x3056==cp||(0x3056<cp&&                                                   \
    (0x3058==cp||(0x3058<cp&&                                                   \
    (0x305A==cp||(0x305A<cp&&                                                   \
    (0x305C==cp||(0x305C<cp&&                                                   \
    (0x305E==cp||(0x305E<cp&&                                                   \
    (0x3060==cp||(0x3060<cp&&                                                   \
    (0x3062==cp||(0x3062<cp&&                                                   \
    (0x3065==cp||(0x3065<cp&&                                                   \
    (0x3067==cp||(0x3067<cp&&                                                   \
    (0x3069==cp||(0x3069<cp&&                                                   \
    (0x3070==cp||(0x3070<cp&&                                                   \
    (0x3071==cp||(0x3071<cp&&                                                   \
    (0x3073==cp||(0x3073<cp&&                                                   \
    (0x3074==cp||(0x3074<cp&&                                                   \
    (0x3076==cp||(0x3076<cp&&                                                   \
    (0x3077==cp||(0x3077<cp&&                                                   \
    (0x3079==cp||(0x3079<cp&&                                                   \
    (0x307A==cp||(0x307A<cp&&                                                   \
    (0x307C==cp||(0x307C<cp&&                                                   \
    (0x307D==cp||(0x307D<cp&&                                                   \
    (0x3094==cp||(0x3094<cp&&                                                   \
    (0x309E==cp||(0x309E<cp&&                                                   \
    (0x30AC==cp||(0x30AC<cp&&                                                   \
    (0x30AE==cp||(0x30AE<cp&&                                                   \
    (0x30B0==cp||(0x30B0<cp&&                                                   \
    (0x30B2==cp||(0x30B2<cp&&                                                   \
    (0x30B4==cp||(0x30B4<cp&&                                                   \
    (0x30B6==cp||(0x30B6<cp&&                                                   \
    (0x30B8==cp||(0x30B8<cp&&                                                   \
    (0x30BA==cp||(0x30BA<cp&&                                                   \
    (0x30BC==cp||(0x30BC<cp&&                                                   \
    (0x30BE==cp||(0x30BE<cp&&                                                   \
    (0x30C0==cp||(0x30C0<cp&&                                                   \
    (0x30C2==cp||(0x30C2<cp&&                                                   \
    (0x30C5==cp||(0x30C5<cp&&                                                   \
    (0x30C7==cp||(0x30C7<cp&&                                                   \
    (0x30C9==cp||(0x30C9<cp&&                                                   \
    (0x30D0==cp||(0x30D0<cp&&                                                   \
    (0x30D1==cp||(0x30D1<cp&&                                                   \
    (0x30D3==cp||(0x30D3<cp&&                                                   \
    (0x30D4==cp||(0x30D4<cp&&                                                   \
    (0x30D6==cp||(0x30D6<cp&&                                                   \
    (0x30D7==cp||(0x30D7<cp&&                                                   \
    (0x30D9==cp||(0x30D9<cp&&                                                   \
    (0x30DA==cp||(0x30DA<cp&&                                                   \
    (0x30DC==cp||(0x30DC<cp&&                                                   \
    (0x30DD==cp||(0x30DD<cp&&                                                   \
    (0x30F4==cp||(0x30F4<cp&&                                                   \
    ((0x30F7<=cp&&cp<=0x30FA)||(0x30FA<cp&&                                     \
    (0x30FE==cp||(0x30FE<cp&&                                                   \
    (0x1109A==cp||(0x1109A<cp&&                                                 \
    (0x1109C==cp||0x110AB==cp)                                                  \
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))


/*
	DECOMPOSED_REST: The remaining 923 non-mark and non-hangul normalizable 1st cp's

	&regcharclass_multi_char_folds::decomposed_all_rest_first()
*/
/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_cp_high(cp)                       \
    ( 0x17F == cp || ( 0x17F < cp &&                         \
    ( 0x1B7 == cp || ( 0x1B7 < cp &&                         \
    ( 0x292 == cp || ( 0x292 < cp &&                         \
    ( 0x391 == cp || ( 0x391 < cp &&                         \
    ( 0x395 == cp || ( 0x395 < cp &&                         \
    ( 0x397 == cp || ( 0x397 < cp &&                         \
    ( 0x399 == cp || ( 0x399 < cp &&                         \
    ( 0x39F == cp || ( 0x39F < cp &&                         \
    ( 0x3A1 == cp || ( 0x3A1 < cp &&                         \
    ( 0x3A5 == cp || ( 0x3A5 < cp &&                         \
    ( 0x3A9 == cp || ( 0x3A9 < cp &&                         \
    ( 0x3B1 == cp || ( 0x3B1 < cp &&                         \
    ( 0x3B5 == cp || ( 0x3B5 < cp &&                         \
    ( 0x3B7 == cp || ( 0x3B7 < cp &&                         \
    ( 0x3B9 == cp || ( 0x3B9 < cp &&                         \
    ( 0x3BF == cp || ( 0x3BF < cp &&                         \
    ( 0x3C1 == cp || ( 0x3C1 < cp &&                         \
    ( 0x3C5 == cp || ( 0x3C5 < cp &&                         \
    ( 0x3C9 == cp || ( 0x3C9 < cp &&                         \
    ( 0x3D2 == cp || ( 0x3D2 < cp &&                         \
    ( 0x406 == cp || ( 0x406 < cp &&                         \
    ( 0x410 == cp || ( 0x410 < cp &&                         \
    ( 0x413 == cp || ( 0x413 < cp &&                         \
    ( ( 0x415 <= cp && cp <= 0x418 ) || ( 0x418 < cp &&      \
    ( 0x41A == cp || ( 0x41A < cp &&                         \
    ( 0x41E == cp || ( 0x41E < cp &&                         \
    ( 0x423 == cp || ( 0x423 < cp &&                         \
    ( 0x427 == cp || ( 0x427 < cp &&                         \
    ( 0x42B == cp || ( 0x42B < cp &&                         \
    ( 0x42D == cp || ( 0x42D < cp &&                         \
    ( 0x430 == cp || ( 0x430 < cp &&                         \
    ( 0x433 == cp || ( 0x433 < cp &&                         \
    ( ( 0x435 <= cp && cp <= 0x438 ) || ( 0x438 < cp &&      \
    ( 0x43A == cp || ( 0x43A < cp &&                         \
    ( 0x43E == cp || ( 0x43E < cp &&                         \
    ( 0x443 == cp || ( 0x443 < cp &&                         \
    ( 0x447 == cp || ( 0x447 < cp &&                         \
    ( 0x44B == cp || ( 0x44B < cp &&                         \
    ( 0x44D == cp || ( 0x44D < cp &&                         \
    ( 0x456 == cp || ( 0x456 < cp &&                         \
    ( 0x474 == cp || ( 0x474 < cp &&                         \
    ( 0x475 == cp || ( 0x475 < cp &&                         \
    ( 0x4D8 == cp || ( 0x4D8 < cp &&                         \
    ( 0x4D9 == cp || ( 0x4D9 < cp &&                         \
    ( 0x4E8 == cp || ( 0x4E8 < cp &&                         \
    ( 0x4E9 == cp || ( 0x4E9 < cp &&                         \
    ( 0x627 == cp || ( 0x627 < cp &&                         \
    ( 0x648 == cp || ( 0x648 < cp &&                         \
    ( 0x64A == cp || ( 0x64A < cp &&                         \
    ( 0x6C1 == cp || ( 0x6C1 < cp &&                         \
    ( 0x6D2 == cp || ( 0x6D2 < cp &&                         \
    ( 0x6D5 == cp || ( 0x6D5 < cp &&                         \
    ( 0x928 == cp || ( 0x928 < cp &&                         \
    ( 0x930 == cp || ( 0x930 < cp &&                         \
    ( 0x933 == cp || ( 0x933 < cp &&                         \
    ( 0xB92 == cp || ( 0xB92 < cp &&                         \
    ( 0x1025 == cp || ( 0x1025 < cp &&                       \
    ( 0x1B05 == cp || ( 0x1B05 < cp &&                       \
    ( 0x1B07 == cp || ( 0x1B07 < cp &&                       \
    ( 0x1B09 == cp || ( 0x1B09 < cp &&                       \
    ( 0x1B0B == cp || ( 0x1B0B < cp &&                       \
    ( 0x1B0D == cp || ( 0x1B0D < cp &&                       \
    ( 0x1B11 == cp || ( 0x1B11 < cp &&                       \
    ( 0x1FBF == cp || ( 0x1FBF < cp &&                       \
    ( 0x1FFE == cp || ( 0x1FFE < cp &&                       \
    ( 0x2190 == cp || ( 0x2190 < cp &&                       \
    ( 0x2192 == cp || ( 0x2192 < cp &&                       \
    ( 0x2194 == cp || ( 0x2194 < cp &&                       \
    ( 0x21D0 == cp || ( 0x21D0 < cp &&                       \
    ( 0x21D2 == cp || ( 0x21D2 < cp &&                       \
    ( 0x21D4 == cp || ( 0x21D4 < cp &&                       \
    ( 0x2203 == cp || ( 0x2203 < cp &&                       \
    ( 0x2208 == cp || ( 0x2208 < cp &&                       \
    ( 0x220B == cp || ( 0x220B < cp &&                       \
    ( 0x2223 == cp || ( 0x2223 < cp &&                       \
    ( 0x2225 == cp || ( 0x2225 < cp &&                       \
    ( 0x223C == cp || ( 0x223C < cp &&                       \
    ( 0x2243 == cp || ( 0x2243 < cp &&                       \
    ( 0x2245 == cp || ( 0x2245 < cp &&                       \
    ( 0x2248 == cp || ( 0x2248 < cp &&                       \
    ( 0x224D == cp || ( 0x224D < cp &&                       \
    ( 0x2261 == cp || ( 0x2261 < cp &&                       \
    ( 0x2264 == cp || ( 0x2264 < cp &&                       \
    ( 0x2265 == cp || ( 0x2265 < cp &&                       \
    ( 0x2272 == cp || ( 0x2272 < cp &&                       \
    ( 0x2273 == cp || ( 0x2273 < cp &&                       \
    ( 0x2276 == cp || ( 0x2276 < cp &&                       \
    ( 0x2277 == cp || ( 0x2277 < cp &&                       \
    ( ( 0x227A <= cp && cp <= 0x227D ) || ( 0x227D < cp &&   \
    ( 0x2282 == cp || ( 0x2282 < cp &&                       \
    ( 0x2283 == cp || ( 0x2283 < cp &&                       \
    ( 0x2286 == cp || ( 0x2286 < cp &&                       \
    ( 0x2287 == cp || ( 0x2287 < cp &&                       \
    ( 0x2291 == cp || ( 0x2291 < cp &&                       \
    ( 0x2292 == cp || ( 0x2292 < cp &&                       \
    ( 0x22A2 == cp || ( 0x22A2 < cp &&                       \
    ( 0x22A8 == cp || ( 0x22A8 < cp &&                       \
    ( 0x22A9 == cp || ( 0x22A9 < cp &&                       \
    ( 0x22AB == cp || ( 0x22AB < cp &&                       \
    ( ( 0x22B2 <= cp && cp <= 0x22B5 ) || ( 0x22B5 < cp &&   \
    ( 0x3046 == cp || ( 0x3046 < cp &&                       \
    ( 0x304B == cp || ( 0x304B < cp &&                       \
    ( 0x304D == cp || ( 0x304D < cp &&                       \
    ( 0x304F == cp || ( 0x304F < cp &&                       \
    ( 0x3051 == cp || ( 0x3051 < cp &&                       \
    ( 0x3053 == cp || ( 0x3053 < cp &&                       \
    ( 0x3055 == cp || ( 0x3055 < cp &&                       \
    ( 0x3057 == cp || ( 0x3057 < cp &&                       \
    ( 0x3059 == cp || ( 0x3059 < cp &&                       \
    ( 0x305B == cp || ( 0x305B < cp &&                       \
    ( 0x305D == cp || ( 0x305D < cp &&                       \
    ( 0x305F == cp || ( 0x305F < cp &&                       \
    ( 0x3061 == cp || ( 0x3061 < cp &&                       \
    ( 0x3064 == cp || ( 0x3064 < cp &&                       \
    ( 0x3066 == cp || ( 0x3066 < cp &&                       \
    ( 0x3068 == cp || ( 0x3068 < cp &&                       \
    ( 0x306F == cp || ( 0x306F < cp &&                       \
    ( 0x3072 == cp || ( 0x3072 < cp &&                       \
    ( 0x3075 == cp || ( 0x3075 < cp &&                       \
    ( 0x3078 == cp || ( 0x3078 < cp &&                       \
    ( 0x307B == cp || ( 0x307B < cp &&                       \
    ( 0x309D == cp || ( 0x309D < cp &&                       \
    ( 0x30A6 == cp || ( 0x30A6 < cp &&                       \
    ( 0x30AB == cp || ( 0x30AB < cp &&                       \
    ( 0x30AD == cp || ( 0x30AD < cp &&                       \
    ( 0x30AF == cp || ( 0x30AF < cp &&                       \
    ( 0x30B1 == cp || ( 0x30B1 < cp &&                       \
    ( 0x30B3 == cp || ( 0x30B3 < cp &&                       \
    ( 0x30B5 == cp || ( 0x30B5 < cp &&                       \
    ( 0x30B7 == cp || ( 0x30B7 < cp &&                       \
    ( 0x30B9 == cp || ( 0x30B9 < cp &&                       \
    ( 0x30BB == cp || ( 0x30BB < cp &&                       \
    ( 0x30BD == cp || ( 0x30BD < cp &&                       \
    ( 0x30BF == cp || ( 0x30BF < cp &&                       \
    ( 0x30C1 == cp || ( 0x30C1 < cp &&                       \
    ( 0x30C4 == cp || ( 0x30C4 < cp &&                       \
    ( 0x30C6 == cp || ( 0x30C6 < cp &&                       \
    ( 0x30C8 == cp || ( 0x30C8 < cp &&                       \
    ( 0x30CF == cp || ( 0x30CF < cp &&                       \
    ( 0x30D2 == cp || ( 0x30D2 < cp &&                       \
    ( 0x30D5 == cp || ( 0x30D5 < cp &&                       \
    ( 0x30D8 == cp || ( 0x30D8 < cp &&                       \
    ( 0x30DB == cp || ( 0x30DB < cp &&                       \
    ( ( 0x30EF <= cp && cp <= 0x30F2 ) || ( 0x30F2 < cp &&   \
    ( 0x30FD == cp || ( 0x30FD < cp &&                       \
    ( 0x11099 == cp || ( 0x11099 < cp &&                     \
    ( 0x1109B == cp || 0x110A5 == cp )                       \
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#endif /* H_COMPOSED */
