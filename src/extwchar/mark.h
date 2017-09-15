/* ex: set ro ft=c: -*- mode: c; buffer-read-only: t -*-
 * This file is built by cperl regen/regcharclass-safec.pl.
 * cp_high means only codepoints > 0xff
 */

#ifndef H_MARK
#define H_MARK 1

/*
	MARK: all Combining Mark codepoints
        Unicode 9.0
	\p{IsM}
*/

/*** GENERATED CODE ***/
/* SIC fatal error: bracket nesting level exceeded maximum of 256.
   -fbracket-depth=512
 */
#define is_MARK_2(cp)                               \
    ((0x111CA<=cp&&cp<=0x111CC)||(0x111CC<cp&&      \
    ((0x1122C<=cp&&cp<=0x11237)||(0x11237<cp&&      \
    ((0x112DF<=cp&&cp<=0x112EA)||(0x112EA<cp&&      \
    ((0x11300<=cp&&cp<=0x11303)||(0x11303<cp&&      \
    (0x1133C==cp||(0x1133C<cp&&                     \
    ((0x1133E<=cp&&cp<=0x11344)||(0x11344<cp&&      \
    (0x11347==cp||(0x11347<cp&&                     \
    (0x11348==cp||(0x11348<cp&&                     \
    ((0x1134B<=cp&&cp<=0x1134D)||(0x1134D<cp&&      \
    (0x11357==cp||(0x11357<cp&&                     \
    (0x11362==cp||(0x11362<cp&&                     \
    (0x11363==cp||(0x11363<cp&&                     \
    ((0x11366<=cp&&cp<=0x1136C)||(0x1136C<cp&&      \
    ((0x11370<=cp&&cp<=0x11374)||(0x11374<cp&&      \
    ((0x114B0<=cp&&cp<=0x114C3)||(0x114C3<cp&&      \
    ((0x115AF<=cp&&cp<=0x115B5)||(0x115B5<cp&&      \
    ((0x115B8<=cp&&cp<=0x115C0)||(0x115C0<cp&&      \
    (0x115DC==cp||(0x115DC<cp&&                     \
    (0x115DD==cp||(0x115DD<cp&&                     \
    ((0x11630<=cp&&cp<=0x11640)||(0x11640<cp&&      \
    ((0x116AB<=cp&&cp<=0x116B7)||(0x116B7<cp&&      \
    ((0x1171D<=cp&&cp<=0x1172B)||(0x1172B<cp&&      \
    ((0x16AF0<=cp&&cp<=0x16AF4)||(0x16AF4<cp&&      \
    ((0x16B30<=cp&&cp<=0x16B36)||(0x16B36<cp&&      \
    ((0x16F51<=cp&&cp<=0x16F7E)||(0x16F7E<cp&&      \
    ((0x16F8F<=cp&&cp<=0x16F92)||(0x16F92<cp&&      \
    (0x1BC9D==cp||(0x1BC9D<cp&&                     \
    (0x1BC9E==cp||(0x1BC9E<cp&&                     \
    ((0x1D165<=cp&&cp<=0x1D169)||(0x1D169<cp&&      \
    ((0x1D16D<=cp&&cp<=0x1D172)||(0x1D172<cp&&      \
    ((0x1D17B<=cp&&cp<=0x1D182)||(0x1D182<cp&&      \
    ((0x1D185<=cp&&cp<=0x1D18B)||(0x1D18B<cp&&      \
    ((0x1D1AA<=cp&&cp<=0x1D1AD)||(0x1D1AD<cp&&      \
    ((0x1D242<=cp&&cp<=0x1D244)||(0x1D244<cp&&      \
    ((0x1DA00<=cp&&cp<=0x1DA36)||(0x1DA36<cp&&      \
    ((0x1DA3B<=cp&&cp<=0x1DA6C)||(0x1DA6C<cp&&      \
    (0x1DA75==cp||(0x1DA75<cp&&                     \
    (0x1DA84==cp||(0x1DA84<cp&&                     \
    ((0x1DA9B<=cp&&cp<=0x1DA9F)||(0x1DA9F<cp&&      \
    ((0x1DAA1<=cp&&cp<=0x1DAAF)||(0x1DAAF<cp&&      \
    ((0x1E8D0<=cp&&cp<=0x1E8D6)||(0xE0100<=cp&&     \
    cp<=0xE01EF)                                    \
)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#define is_MARK_1(cp)                               \
    ((0xAAB2<=cp&&cp<=0xAAB4)||(0xAAB4<cp&&         \
    (0xAAB7==cp||(0xAAB7<cp&&                       \
    (0xAAB8==cp||(0xAAB8<cp&&                       \
    (0xAABE==cp||(0xAABE<cp&&                       \
    (0xAABF==cp||(0xAABF<cp&&                       \
    (0xAAC1==cp||(0xAAC1<cp&&                       \
    ((0xAAEB<=cp&&cp<=0xAAEF)||(0xAAEF<cp&&         \
    (0xAAF5==cp||(0xAAF5<cp&&                       \
    (0xAAF6==cp||(0xAAF6<cp&&                       \
    ((0xABE3<=cp&&cp<=0xABEA)||(0xABEA<cp&&         \
    (0xABEC==cp||(0xABEC<cp&&                       \
    (0xABED==cp||(0xABED<cp&&                       \
    (0xFB1E==cp||(0xFB1E<cp&&                       \
    ((0xFE00<=cp&&cp<=0xFE0F)||(0xFE0F<cp&&         \
    ((0xFE20<=cp&&cp<=0xFE2F)||(0xFE2F<cp&&         \
    (0x101FD==cp||(0x101FD<cp&&                     \
    (0x102E0==cp||(0x102E0<cp&&                     \
    ((0x10376<=cp&&cp<=0x1037A)||(0x1037A<cp&&      \
    ((0x10A01<=cp&&cp<=0x10A03)||(0x10A03<cp&&      \
    (0x10A05==cp||(0x10A05<cp&&                     \
    (0x10A06==cp||(0x10A06<cp&&                     \
    ((0x10A0C<=cp&&cp<=0x10A0F)||(0x10A0F<cp&&      \
    ((0x10A38<=cp&&cp<=0x10A3A)||(0x10A3A<cp&&      \
    (0x10A3F==cp||(0x10A3F<cp&&                     \
    (0x10AE5==cp||(0x10AE5<cp&&                     \
    (0x10AE6==cp||(0x10AE6<cp&&                     \
    ((0x11000<=cp&&cp<=0x11002)||(0x11002<cp&&      \
    ((0x11038<=cp&&cp<=0x11046)||(0x11046<cp&&      \
    ((0x1107F<=cp&&cp<=0x11082)||(0x11082<cp&&      \
    ((0x110B0<=cp&&cp<=0x110BA)||(0x110BA<cp&&      \
    ((0x11100<=cp&&cp<=0x11102)||(0x11102<cp&&      \
    ((0x11127<=cp&&cp<=0x11134)||(0x11134<cp&&      \
    (0x11173==cp||(0x11173<cp&&                     \
    ((0x11180<=cp&&cp<=0x11182)||(0x11182<cp&&      \
    ((0x111B3<=cp&&cp<=0x111C0)||(0x111C0<cp&&      \
    is_MARK_2(cp) \
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#define is_MARK_cp_high(cp)                         \
    ((0x300<=cp&&cp<=0x36F)||(0x36F<cp&&            \
    ((0x483<=cp&&cp<=0x489)||(0x489<cp&&            \
    ((0x591<=cp&&cp<=0x5BD)||(0x5BD<cp&&            \
    (0xFC6==cp||(0xFC6<cp&&                         \
    ((0x102B<=cp&&cp<=0x103E)||(0x103E<cp&&         \
    ((0x1056<=cp&&cp<=0x1059)||(0x1059<cp&&         \
    ((0x105E<=cp&&cp<=0x1060)||(0x1060<cp&&         \
    ((0x1062<=cp&&cp<=0x1064)||(0x1064<cp&&         \
    ((0x1067<=cp&&cp<=0x106D)||(0x106D<cp&&         \
    ((0x1071<=cp&&cp<=0x1074)||(0x1074<cp&&         \
    ((0x1082<=cp&&cp<=0x108D)||(0x108D<cp&&         \
    (0x108F==cp||(0x108F<cp&&                       \
    ((0x109A<=cp&&cp<=0x109D)||(0x109D<cp&&         \
    ((0x135D<=cp&&cp<=0x135F)||(0x135F<cp&&         \
    ((0x1712<=cp&&cp<=0x1714)||(0x1714<cp&&         \
    ((0x1732<=cp&&cp<=0x1734)||(0x1734<cp&&         \
    (0x1752==cp||(0x1752<cp&&                       \
    (0x1753==cp||(0x1753<cp&&                       \
    (0x1772==cp||(0x1772<cp&&                       \
    (0x1773==cp||(0x1773<cp&&                       \
    ((0x17B4<=cp&&cp<=0x17D3)||(0x17D3<cp&&         \
    (0x17DD==cp||(0x17DD<cp&&                       \
    ((0x180B<=cp&&cp<=0x180D)||(0x180D<cp&&         \
    (0x18A9==cp||(0x18A9<cp&&                       \
    ((0x1920<=cp&&cp<=0x192B)||(0x192B<cp&&         \
    ((0x1930<=cp&&cp<=0x193B)||(0x193B<cp&&         \
    ((0x1A17<=cp&&cp<=0x1A1B)||(0x1A1B<cp&&         \
    ((0x1A55<=cp&&cp<=0x1A5E)||(0x1A5E<cp&&         \
    ((0x1A60<=cp&&cp<=0x1A7C)||(0x1A7C<cp&&         \
    (0x1A7F==cp||(0x1A7F<cp&&                       \
    ((0x1AB0<=cp&&cp<=0x1ABE)||(0x1ABE<cp&&         \
    ((0x1B00<=cp&&cp<=0x1B04)||(0x1B04<cp&&         \
    ((0x1B34<=cp&&cp<=0x1B44)||(0x1B44<cp&&         \
    ((0x1B6B<=cp&&cp<=0x1B73)||(0x1B73<cp&&         \
    ((0x1B80<=cp&&cp<=0x1B82)||(0x1B82<cp&&         \
    ((0x1BA1<=cp&&cp<=0x1BAD)||(0x1BAD<cp&&         \
    ((0x1BE6<=cp&&cp<=0x1BF3)||(0x1BF3<cp&&         \
    ((0x1C24<=cp&&cp<=0x1C37)||(0x1C37<cp&&         \
    ((0x1CD0<=cp&&cp<=0x1CD2)||(0x1CD2<cp&&         \
    ((0x1CD4<=cp&&cp<=0x1CE8)||(0x1CE8<cp&&         \
    (0x1CED==cp||(0x1CED<cp&&                       \
    ((0x1CF2<=cp&&cp<=0x1CF4)||(0x1CF4<cp&&         \
    (0x1CF8==cp||(0x1CF8<cp&&                       \
    (0x1CF9==cp||(0x1CF9<cp&&                       \
    ((0x1DC0<=cp&&cp<=0x1DF5)||(0x1DF5<cp&&         \
    ((0x1DFC<=cp&&cp<=0x1DFF)||(0x1DFF<cp&&         \
    ((0x20D0<=cp&&cp<=0x20F0)||(0x20F0<cp&&         \
    ((0x2CEF<=cp&&cp<=0x2CF1)||(0x2CF1<cp&&         \
    (0x2D7F==cp||(0x2D7F<cp&&                       \
    ((0x2DE0<=cp&&cp<=0x2DFF)||(0x2DFF<cp&&         \
    ((0x302A<=cp&&cp<=0x302F)||(0x302F<cp&&         \
    (0x3099==cp||(0x3099<cp&&                       \
    (0x309A==cp||(0x309A<cp&&                       \
    ((0xA66F<=cp&&cp<=0xA672)||(0xA672<cp&&         \
    ((0xA674<=cp&&cp<=0xA67D)||(0xA67D<cp&&         \
    (0xA69E==cp||(0xA69E<cp&&                       \
    (0xA69F==cp||(0xA69F<cp&&                       \
    (0xA6F0==cp||(0xA6F0<cp&&                       \
    (0xA6F1==cp||(0xA6F1<cp&&                       \
    (0xA802==cp||(0xA802<cp&&                       \
    (0xA806==cp||(0xA806<cp&&                       \
    (0xA80B==cp||(0xA80B<cp&&                       \
    ((0xA823<=cp&&cp<=0xA827)||(0xA827<cp&&         \
    (0xA880==cp||(0xA880<cp&&                       \
    (0xA881==cp||(0xA881<cp&&                       \
    ((0xA8B4<=cp&&cp<=0xA8C4)||(0xA8C4<cp&&         \
    ((0xA8E0<=cp&&cp<=0xA8F1)||(0xA8F1<cp&&         \
    ((0xA926<=cp&&cp<=0xA92D)||(0xA92D<cp&&         \
    ((0xA947<=cp&&cp<=0xA953)||(0xA953<cp&&         \
    ((0xA980<=cp&&cp<=0xA983)||(0xA983<cp&&         \
    ((0xA9B3<=cp&&cp<=0xA9C0)||(0xA9C0<cp&&         \
    (0xA9E5==cp||(0xA9E5<cp&&                       \
    ((0xAA29<=cp&&cp<=0xAA36)||(0xAA36<cp&&         \
    (0xAA43==cp||(0xAA43<cp&&                       \
    (0xAA4C==cp||(0xAA4C<cp&&                       \
    (0xAA4D==cp||(0xAA4D<cp&&                       \
    ((0xAA7B<=cp&&cp<=0xAA7D)||(0xAA7D<cp&&         \
    (0xAAB0==cp||(0xAAB0<cp&&                       \
    is_MARK_1(cp) \
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#endif /* H_MARK */
