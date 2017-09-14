/* ex: set ro ft=c: -*- mode: c; buffer-read-only: t -*-
 * This file is built by cperl regen/regcharclass-safec.pl.
 * unlike the others this looks at a UTF-8 stream
 */

#ifndef H_DEC_REST
#define H_DEC_REST 1

/*
	DECOMPOSED_REST: The remaining 869 non-mark and non-hangul normalizables
        Unicode 9.0
	&regcharclass_multi_char_folds::decomposed_rest()
*/
/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part0(s,e,is_utf8)                          \
( ( 0xCC == ((const U8*)s)[2] ) ?                                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ?    \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : 4 )                                           \
			: ( 0xCD == ((const U8*)s)[4] ) ?                   \
			    ( ( 0x82 == ((const U8*)s)[5] ) ?               \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : ( 0x85 == ((const U8*)s)[5] ) ? 6 : 4 )       \
			: 4 )                                               \
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part1(s,e,is_utf8)                          \
( ( 0x99 == ((const U8*)s)[1] ) ?                                           \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 ) ? 4\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0xA1 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xA5 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 ) ? 4\
		    : ( 0x94 == ((const U8*)s)[3] ) ?                       \
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0xB1 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ?    \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : 4 )                                           \
			: ( 0xCD == ((const U8*)s)[4] ) ?                   \
			    ( ( 0x82 == ((const U8*)s)[5] ) ?               \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : ( 0x85 == ((const U8*)s)[5] ) ? 6 : 4 )       \
			: 4 )                                               \
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( 0xB7 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ?    \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : 4 )                                           \
			: ( 0xCD == ((const U8*)s)[4] ) ?                   \
			    ( ( 0x82 == ((const U8*)s)[5] ) ?               \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : ( 0x85 == ((const U8*)s)[5] ) ? 6 : 4 )       \
			: 4 )                                               \
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( 0xB9 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x88 == ((const U8*)s)[3] || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part2(s,e,is_utf8)                          \
( ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0x98 || ( ((const U8*)s)[1] & 0xFE ) == 0xA8 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part3(s,e,is_utf8)                          \
( ( 0xD8 == ((const U8*)s)[0] ) ?                                           \
	    ( ( ( ( 0xA7 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x93 <= ((const U8*)s)[3] && ((const U8*)s)[3] <= 0x95 ) ) ? 4 : 0 )\
	: ( 0xD9 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFD ) == 0x88 ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xDB == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x81 == ((const U8*)s)[1] || 0x92 == ((const U8*)s)[1] || 0x95 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xE0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xA4 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( 0xA8 == ((const U8*)s)[2] || 0xB0 == ((const U8*)s)[2] || 0xB3 == ((const U8*)s)[2] ) && ( 0xE0 == ((const U8*)s)[3] ) ) && ( 0xA4 == ((const U8*)s)[4] ) ) && ( 0xBC == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( ( ( ( ( 0xAE == ((const U8*)s)[1] ) && ( 0x92 == ((const U8*)s)[2] ) ) && ( 0xE0 == ((const U8*)s)[3] ) ) && ( 0xAF == ((const U8*)s)[4] ) ) && ( 0x97 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	: ( 0xE1 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x80 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( 0xA5 == ((const U8*)s)[2] ) && ( 0xE1 == ((const U8*)s)[3] ) ) && ( 0x80 == ((const U8*)s)[4] ) ) && ( 0xAE == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( ( ( ( ( 0xAC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFD ) == 0x85 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x89 ) || ((const U8*)s)[2] == 0x8D || ((const U8*)s)[2] == 0x91 ) ) && ( 0xE1 == ((const U8*)s)[3] ) ) && ( 0xAC == ((const U8*)s)[4] ) ) && ( 0xB5 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	: ( 0xE3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ((const U8*)s)[2] == 0x86 || ((const U8*)s)[2] == 0x8B || ( ( ((const U8*)s)[2] & 0xFD ) == 0x8D ) || ( ( ((const U8*)s)[2] & 0xF1 ) == 0x91 ) || ((const U8*)s)[2] == 0xA1 || ( ( ((const U8*)s)[2] & 0xFD ) == 0xA4 ) || ((const U8*)s)[2] == 0xA8 ) ?\
		    ( ( ( ( 0xE3 == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
		: ( ( ( ( 0xAF == ((const U8*)s)[2] || 0xB2 == ((const U8*)s)[2] || 0xB5 == ((const U8*)s)[2] || 0xB8 == ((const U8*)s)[2] || 0xBB == ((const U8*)s)[2] ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] || 0x9A == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( 0x82 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( ((const U8*)s)[2] == 0x9D || ((const U8*)s)[2] == 0xA6 || ((const U8*)s)[2] == 0xAB || ( ( ((const U8*)s)[2] & 0xFD ) == 0xAD ) || ( ( ((const U8*)s)[2] & 0xF1 ) == 0xB1 ) ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( 0x83 == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x84 == ((const U8*)s)[2] || 0x86 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] || ( 0xAF <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0xB2 ) || 0xBD == ((const U8*)s)[2] ) ?\
		    ( ( ( ( 0xE3 == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
		: ( ( ( ( 0x8F == ((const U8*)s)[2] || 0x92 == ((const U8*)s)[2] || 0x95 == ((const U8*)s)[2] || 0x98 == ((const U8*)s)[2] || 0x9B == ((const U8*)s)[2] ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] || 0x9A == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : 0 )                                                           \
	: ( ( ( ( ( ( ( ( 0xF0 == ((const U8*)s)[0] ) && ( 0x91 == ((const U8*)s)[1] ) ) && ( 0x82 == ((const U8*)s)[2] ) ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x99 ) || ((const U8*)s)[3] == 0xA5 ) ) && ( 0xF0 == ((const U8*)s)[4] ) ) && ( 0x91 == ((const U8*)s)[5] ) ) && ( 0x82 == ((const U8*)s)[6] ) ) && ( 0xBA == ((const U8*)s)[7] ) ) ? 8 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part4(s,e,is_utf8)                          \
( ( 0x91 == ((const U8*)s)[1] ) ?                                           \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ?    \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : 4 )                                           \
			: ( 0xCD == ((const U8*)s)[4] ) ?                   \
			    ( ( 0x82 == ((const U8*)s)[5] ) ?               \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : ( 0x85 == ((const U8*)s)[5] ) ? 6 : 4 )       \
			: 4 )                                               \
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9F ) ) ?\
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( ( 0xCC == ((const U8*)s)[4] ) && ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0x97 == ((const U8*)s)[1] || 0xA9 == ((const U8*)s)[1] ) ? is_DECOMPOSED_REST_safe_part0(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part1(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part5(s,e,is_utf8)                          \
( ( 0xCF == ((const U8*)s)[0] ) ?                                           \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x85 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x88 == ((const U8*)s)[3] || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x89 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ?    \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : 4 )                                           \
			: ( 0xCD == ((const U8*)s)[4] ) ?                   \
			    ( ( 0x82 == ((const U8*)s)[5] ) ?               \
				( ( ( 0xCD == ((const U8*)s)[6] ) && ( 0x85 == ((const U8*)s)[7] ) ) ? 8 : 6 )\
			    : ( 0x85 == ((const U8*)s)[5] ) ? 6 : 4 )       \
			: 4 )                                               \
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ((const U8*)s)[1] == 0x86 || ((const U8*)s)[1] == 0x97 || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9E ) || ( ( ((const U8*)s)[1] & 0xEF ) == 0xA7 ) || ((const U8*)s)[1] == 0xAB || ((const U8*)s)[1] == 0xAD ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x90 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x96 ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x86 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x93 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9A ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ((const U8*)s)[3] == 0x86 ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ) ? 4 : 0 )\
	    : ( ( ( 0xA3 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	: ( 0xD1 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x83 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	    : ( 0x87 == ((const U8*)s)[1] || 0x8B == ((const U8*)s)[1] || 0x8D == ((const U8*)s)[1] || 0x96 == ((const U8*)s)[1] ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0xB4 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8F == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD3 == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part2(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part3(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part6(s,e,is_utf8)                          \
( ( 0xCC == ((const U8*)s)[1] ) ?                                           \
		( ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || 0x86 == ((const U8*)s)[2] || ( 0x89 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8C ) || 0x8F == ((const U8*)s)[2] || 0x91 == ((const U8*)s)[2] || ( 0xA3 == ((const U8*)s)[2] || 0xA4 == ((const U8*)s)[2] ) || 0xA8 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB0 == ((const U8*)s)[2] ) ? 3\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xF7 ) == 0x84 ) ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part7(s,e,is_utf8)                          \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x56 ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ((const U8*)s)[2] & 0xDF ) == 0x83 ) ) ? 3 : 0 )\
	: ( 0x57 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x58 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x59 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x89 ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x5A ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x68 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x69 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x6A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x74 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ((const U8*)s)[2] == 0xA3 || ( ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x77 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF5 ) == 0x80 ) || ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0xA3 ) ) ? 3 : 0 )\
	: ( 0x79 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0xC3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x86 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] || 0x84 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC5 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xBF == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x87 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC6 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xB7 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCA == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCE == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part4(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part5(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part8(s,e,is_utf8)                          \
( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0xA7 ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part9(s,e,is_utf8)                          \
( ( 0x48 == ((const U8*)s)[0] ) ?                                           \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE ) ) ? 3 : 0 )\
	: ( 0x49 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x4A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4B ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4C ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA7 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4D ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4E ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xDB ) == 0x83 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4F ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x86 || ( ( ((const U8*)s)[2] & 0xFD ) == 0x89 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] || 0x84 == ((const U8*)s)[4] || 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xDF ) == 0x88 ) ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x50 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x52 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xEF ) == 0x81 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x87 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xA7 || ((const U8*)s)[2] == 0xB1 ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x53 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x87 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x82 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) ? 3 : 0 )\
	    : 0 )                                                           \
	: ( 0x54 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x55 ) ? is_DECOMPOSED_REST_safe_part6(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part7(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part10(s,e,is_utf8)                         \
( ( 0xCC == ((const U8*)s)[2] ) ?                                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] || 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part11(s,e,is_utf8)                         \
( ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9F ) ) ?\
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( ( 0xCC == ((const U8*)s)[4] ) && ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0x97 == ((const U8*)s)[1] || 0xA9 == ((const U8*)s)[1] ) ?  \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] || 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x99 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 ) ? 4\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0xA1 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xA5 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 ) ? 4\
		    : ( 0x94 == ((const U8*)s)[3] ) ?                       \
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: 0 )                                                       \
	    : ( 0xB1 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ? 4          \
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] || 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( 0xB7 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] || 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( 0xB9 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x88 == ((const U8*)s)[3] || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part12(s,e,is_utf8)                         \
( ( 0x83 == ((const U8*)s)[1] ) ?                                           \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	    : ( 0x87 == ((const U8*)s)[1] || 0x8B == ((const U8*)s)[1] || 0x8D == ((const U8*)s)[1] || 0x96 == ((const U8*)s)[1] ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0xB4 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8F == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part13(s,e,is_utf8)                         \
( ( 0xD3 == ((const U8*)s)[0] ) ?                                           \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0x98 || ( ((const U8*)s)[1] & 0xFE ) == 0xA8 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD8 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xA7 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x93 <= ((const U8*)s)[3] && ((const U8*)s)[3] <= 0x95 ) ) ? 4 : 0 )\
	: ( 0xD9 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFD ) == 0x88 ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xDB == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x81 == ((const U8*)s)[1] || 0x92 == ((const U8*)s)[1] || 0x95 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xE0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xA4 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( 0xA8 == ((const U8*)s)[2] || 0xB0 == ((const U8*)s)[2] || 0xB3 == ((const U8*)s)[2] ) && ( 0xE0 == ((const U8*)s)[3] ) ) && ( 0xA4 == ((const U8*)s)[4] ) ) && ( 0xBC == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( ( ( ( ( 0xAE == ((const U8*)s)[1] ) && ( 0x92 == ((const U8*)s)[2] ) ) && ( 0xE0 == ((const U8*)s)[3] ) ) && ( 0xAF == ((const U8*)s)[4] ) ) && ( 0x97 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	: ( 0xE1 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x80 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( 0xA5 == ((const U8*)s)[2] ) && ( 0xE1 == ((const U8*)s)[3] ) ) && ( 0x80 == ((const U8*)s)[4] ) ) && ( 0xAE == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( ( ( ( ( 0xAC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFD ) == 0x85 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x89 ) || ((const U8*)s)[2] == 0x8D || ((const U8*)s)[2] == 0x91 ) ) && ( 0xE1 == ((const U8*)s)[3] ) ) && ( 0xAC == ((const U8*)s)[4] ) ) && ( 0xB5 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	: ( 0xE3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ((const U8*)s)[2] == 0x86 || ((const U8*)s)[2] == 0x8B || ( ( ((const U8*)s)[2] & 0xFD ) == 0x8D ) || ( ( ((const U8*)s)[2] & 0xF1 ) == 0x91 ) || ((const U8*)s)[2] == 0xA1 || ( ( ((const U8*)s)[2] & 0xFD ) == 0xA4 ) || ((const U8*)s)[2] == 0xA8 ) ?\
		    ( ( ( ( 0xE3 == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
		: ( ( ( ( 0xAF == ((const U8*)s)[2] || 0xB2 == ((const U8*)s)[2] || 0xB5 == ((const U8*)s)[2] || 0xB8 == ((const U8*)s)[2] || 0xBB == ((const U8*)s)[2] ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] || 0x9A == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( 0x82 == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ( ((const U8*)s)[2] == 0x9D || ((const U8*)s)[2] == 0xA6 || ((const U8*)s)[2] == 0xAB || ( ( ((const U8*)s)[2] & 0xFD ) == 0xAD ) || ( ( ((const U8*)s)[2] & 0xF1 ) == 0xB1 ) ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : ( 0x83 == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x84 == ((const U8*)s)[2] || 0x86 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] || ( 0xAF <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0xB2 ) || 0xBD == ((const U8*)s)[2] ) ?\
		    ( ( ( ( 0xE3 == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] ) ) ? 6 : 0 )\
		: ( ( ( ( 0x8F == ((const U8*)s)[2] || 0x92 == ((const U8*)s)[2] || 0x95 == ((const U8*)s)[2] || 0x98 == ((const U8*)s)[2] || 0x9B == ((const U8*)s)[2] ) && ( 0xE3 == ((const U8*)s)[3] ) ) && ( 0x82 == ((const U8*)s)[4] ) ) && ( 0x99 == ((const U8*)s)[5] || 0x9A == ((const U8*)s)[5] ) ) ? 6 : 0 )\
	    : 0 )                                                           \
	: 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part14(s,e,is_utf8)                         \
( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF5 ) == 0x80 ) || ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0xA3 ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part15(s,e,is_utf8)                         \
( ( 0x79 == ((const U8*)s)[0] ) ?                                           \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0xC3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x86 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] || 0x84 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC5 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xBF == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x87 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC6 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xB7 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCA == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCE == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x91 == ((const U8*)s)[1] ) ? is_DECOMPOSED_REST_safe_part10(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part11(s,e,is_utf8) )\
	: ( 0xCF == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x85 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ? 4\
		    : ( 0x88 == ((const U8*)s)[3] || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x89 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) ?            \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ?\
			( ( 0xCC == ((const U8*)s)[4] ) ?                   \
			    ( ( ( ((const U8*)s)[5] & 0xFE ) == 0x80 ) ? 6 : 4 )\
			: ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x82 == ((const U8*)s)[5] || 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : 0 )                                                   \
		: ( 0xCD == ((const U8*)s)[2] ) ?                           \
		    ( ( 0x82 == ((const U8*)s)[3] ) ?                       \
			( ( ( 0xCD == ((const U8*)s)[4] ) && ( 0x85 == ((const U8*)s)[5] ) ) ? 6 : 4 )\
		    : ( 0x85 == ((const U8*)s)[3] ) ? 4 : 0 )               \
		: 0 )                                                       \
	    : ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ((const U8*)s)[1] == 0x86 || ((const U8*)s)[1] == 0x97 || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9E ) || ( ( ((const U8*)s)[1] & 0xEF ) == 0xA7 ) || ((const U8*)s)[1] == 0xAB || ((const U8*)s)[1] == 0xAD ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x90 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x96 ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x86 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x93 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9A ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ((const U8*)s)[3] == 0x86 ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ) ? 4 : 0 )\
	    : ( ( ( 0xA3 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	: ( 0xD1 == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part12(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part13(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part16(s,e,is_utf8)                         \
( ( 0xCC == ((const U8*)s)[1] ) ?                                           \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x86 || ( ( ((const U8*)s)[2] & 0xFD ) == 0x89 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] || 0x84 == ((const U8*)s)[4] || 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xDF ) == 0x88 ) ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part17(s,e,is_utf8)                         \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x50 ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x52 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xEF ) == 0x81 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x87 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xA7 || ((const U8*)s)[2] == 0xB1 ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x53 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x87 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x82 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) ? 3 : 0 )\
	    : 0 )                                                           \
	: ( 0x54 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x55 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || 0x86 == ((const U8*)s)[2] || ( 0x89 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8C ) || 0x8F == ((const U8*)s)[2] || 0x91 == ((const U8*)s)[2] || ( 0xA3 == ((const U8*)s)[2] || 0xA4 == ((const U8*)s)[2] ) || 0xA8 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB0 == ((const U8*)s)[2] ) ? 3\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xF7 ) == 0x84 ) ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x56 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ((const U8*)s)[2] & 0xDF ) == 0x83 ) ) ? 3 : 0 )\
	: ( 0x57 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x58 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x59 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x89 ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x5A ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x68 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x69 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x6A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x74 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ((const U8*)s)[2] == 0xA3 || ( ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x77 == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part14(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part15(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part18(s,e,is_utf8)                         \
( ( 0xCC == ((const U8*)s)[1] ) ?                                           \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA5 || ((const U8*)s)[2] == 0xA8 ) ? 3\
		: ( ( ((const U8*)s)[2] & 0xFB ) == 0x82 ) ?                \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x8A == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFB ) == 0x82 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part19(s,e,is_utf8)                         \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x42 ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x43 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ? 3\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x44 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x45 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xF6 ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x86 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x46 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x47 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0xA7 ) ) ? 3 : 0 )\
	: ( 0x48 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE ) ) ? 3 : 0 )\
	: ( 0x49 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x4A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4B ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4C ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA7 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4D ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4E ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xDB ) == 0x83 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4F ) ? is_DECOMPOSED_REST_safe_part16(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part17(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part20(s,e,is_utf8)                         \
( ( 0x91 == ((const U8*)s)[1] ) ?                                           \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9F ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ) ? 4 : 0 )\
	    : ( 0x97 == ((const U8*)s)[1] || 0xA9 == ((const U8*)s)[1] ) ?  \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x99 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ) ? 4 : 0 )\
	    : ( 0xA1 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xA5 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x94 ) ) ? 4 : 0 )\
	    : ( 0xB1 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xB7 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xB9 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part21(s,e,is_utf8)                         \
( ( 0xCF == ((const U8*)s)[0] ) ?                                           \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x85 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x89 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ((const U8*)s)[1] == 0x86 || ((const U8*)s)[1] == 0x97 || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9E ) || ( ( ((const U8*)s)[1] & 0xEF ) == 0xA7 ) || ((const U8*)s)[1] == 0xAB || ((const U8*)s)[1] == 0xAD ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x90 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x96 ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x86 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x93 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9A ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ((const U8*)s)[3] == 0x86 ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ) ? 4 : 0 )\
	    : ( ( ( 0xA3 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	: ( 0xD1 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x83 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	    : ( 0x87 == ((const U8*)s)[1] || 0x8B == ((const U8*)s)[1] || 0x8D == ((const U8*)s)[1] || 0x96 == ((const U8*)s)[1] ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0xB4 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8F == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0x98 || ( ((const U8*)s)[1] & 0xFE ) == 0xA8 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD8 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xA7 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x93 <= ((const U8*)s)[3] && ((const U8*)s)[3] <= 0x95 ) ) ? 4 : 0 )\
	: ( 0xD9 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFD ) == 0x88 ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( ( ( ( 0xDB == ((const U8*)s)[0] ) && ( 0x81 == ((const U8*)s)[1] || 0x92 == ((const U8*)s)[1] || 0x95 == ((const U8*)s)[1] ) ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part22(s,e,is_utf8)                         \
( ( 0xCC == ((const U8*)s)[1] ) ?                                           \
		( ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || 0x86 == ((const U8*)s)[2] || ( 0x89 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8C ) || 0x8F == ((const U8*)s)[2] || 0x91 == ((const U8*)s)[2] || ( 0xA3 == ((const U8*)s)[2] || 0xA4 == ((const U8*)s)[2] ) || 0xA8 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB0 == ((const U8*)s)[2] ) ? 3\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xF7 ) == 0x84 ) ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part23(s,e,is_utf8)                         \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x56 ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ((const U8*)s)[2] & 0xDF ) == 0x83 ) ) ? 3 : 0 )\
	: ( 0x57 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x58 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x59 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x89 ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x5A ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x68 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x69 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x6A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x74 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ((const U8*)s)[2] == 0xA3 || ( ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x77 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF5 ) == 0x80 ) || ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0xA3 ) ) ? 3 : 0 )\
	: ( 0x79 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0xC3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x86 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] || 0x84 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC5 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xBF == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x87 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC6 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xB7 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCA == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCE == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part20(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part21(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part24(s,e,is_utf8)                         \
( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0xA7 ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part25(s,e,is_utf8)                         \
( ( 0x48 == ((const U8*)s)[0] ) ?                                           \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE ) ) ? 3 : 0 )\
	: ( 0x49 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x88 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( 0x4A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4B ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4C ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA7 == ((const U8*)s)[2] || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4D ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4E ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xDB ) == 0x83 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4F ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x86 || ( ( ((const U8*)s)[2] & 0xFD ) == 0x89 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x83 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] || 0x84 == ((const U8*)s)[4] || 0x88 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xDF ) == 0x88 ) ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x9B == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[4] & 0xDF ) == 0x83 ) || ((const U8*)s)[4] == 0x89 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x50 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x52 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xEF ) == 0x81 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x87 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xA7 || ((const U8*)s)[2] == 0xB1 ) ? 3\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x53 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( 0x81 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x87 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x82 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) ? 3 : 0 )\
	    : 0 )                                                           \
	: ( 0x54 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x55 ) ? is_DECOMPOSED_REST_safe_part22(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part23(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part26(s,e,is_utf8)                         \
( ( 0x91 == ((const U8*)s)[1] ) ?                                           \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9F ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ) ? 4 : 0 )\
	    : ( 0x97 == ((const U8*)s)[1] || 0xA9 == ((const U8*)s)[1] ) ?  \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x99 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ) ? 4 : 0 )\
	    : ( 0xA1 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xA5 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x94 ) ) ? 4 : 0 )\
	    : ( 0xB1 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xB7 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0xB9 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part27(s,e,is_utf8)                         \
( ( 0xCF == ((const U8*)s)[0] ) ?                                           \
	    ( ( 0x81 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x85 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x93 || ((const U8*)s)[3] == 0x94 ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( 0x89 == ((const U8*)s)[1] ) ?                               \
		( ( 0xCC == ((const U8*)s)[2] ) ?                           \
		    ( ( ( ((const U8*)s)[3] & 0xFE ) == 0x80 || ( 0x93 == ((const U8*)s)[3] || 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
		: ( ( 0xCD == ((const U8*)s)[2] ) && ( 0x82 == ((const U8*)s)[3] || 0x85 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD0 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ((const U8*)s)[1] == 0x86 || ((const U8*)s)[1] == 0x97 || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9E ) || ( ( ((const U8*)s)[1] & 0xEF ) == 0xA7 ) || ((const U8*)s)[1] == 0xAB || ((const U8*)s)[1] == 0xAD ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x90 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x96 ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x86 == ((const U8*)s)[3] || 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x93 ) || ( ( ((const U8*)s)[1] & 0xDF ) == 0x9A ) ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x95 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ((const U8*)s)[3] == 0x86 ) ) ? 4 : 0 )\
	    : ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xF7 ) == 0x80 ) || ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) ) ) ? 4 : 0 )\
	    : ( ( ( 0xA3 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	: ( 0xD1 == ((const U8*)s)[0] ) ?                                   \
	    ( ( 0x83 == ((const U8*)s)[1] ) ?                               \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( ( ( ((const U8*)s)[3] & 0xFD ) == 0x84 ) || ((const U8*)s)[3] == 0x88 || ((const U8*)s)[3] == 0x8B ) ) ? 4 : 0 )\
	    : ( 0x87 == ((const U8*)s)[1] || 0x8B == ((const U8*)s)[1] || 0x8D == ((const U8*)s)[1] || 0x96 == ((const U8*)s)[1] ) ?\
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0xB4 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8F == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFE ) == 0x98 || ( ((const U8*)s)[1] & 0xFE ) == 0xA8 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x88 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xD8 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xA7 == ((const U8*)s)[1] ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x93 <= ((const U8*)s)[3] && ((const U8*)s)[3] <= 0x95 ) ) ? 4 : 0 )\
	: ( 0xD9 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( ( ((const U8*)s)[1] & 0xFD ) == 0x88 ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( ( ( ( 0xDB == ((const U8*)s)[0] ) && ( 0x81 == ((const U8*)s)[1] || 0x92 == ((const U8*)s)[1] || 0x95 == ((const U8*)s)[1] ) ) && ( 0xD9 == ((const U8*)s)[2] ) ) && ( 0x94 == ((const U8*)s)[3] ) ) ? 4 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part28(s,e,is_utf8)                         \
( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part29(s,e,is_utf8)                         \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x52 ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xEF ) == 0x81 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x87 ) || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x53 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) ) ? 3 : 0 )\
	: ( 0x54 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x55 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF4 ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0x9B || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA4 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x56 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ((const U8*)s)[2] & 0xDF ) == 0x83 ) ) ? 3 : 0 )\
	: ( 0x57 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x58 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x59 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x89 ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x5A ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x68 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x69 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( 0x6A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x74 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ((const U8*)s)[2] == 0xA3 || ( ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x77 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF5 ) == 0x80 ) || ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0xA3 ) ) ? 3 : 0 )\
	: ( 0x79 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0xC3 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x86 ) ?                    \
		( ( ( 0xCC == ((const U8*)s)[2] ) && ( 0x81 == ((const U8*)s)[3] || 0x84 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	    : ( ( ( ( ((const U8*)s)[1] & 0xDF ) == 0x98 ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x81 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC5 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xBF == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x87 == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xC6 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0xB7 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCA == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( ( 0x92 == ((const U8*)s)[1] ) && ( 0xCC == ((const U8*)s)[2] ) ) && ( 0x8C == ((const U8*)s)[3] ) ) ? 4 : 0 )\
	: ( 0xCE == ((const U8*)s)[0] ) ? is_DECOMPOSED_REST_safe_part26(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part27(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part30(s,e,is_utf8)                         \
( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part31(s,e,is_utf8)                         \
( ( ( ((const U8*)s)[0] & 0xDF ) == 0x4D ) ?                                \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4E ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xDB ) == 0x83 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4F ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0x8B ) || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0x9B || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x50 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x52 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xEF ) == 0x81 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x87 ) || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x53 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) ) ? 3 : 0 )\
	: ( 0x54 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || ( ((const U8*)s)[2] & 0xFE ) == 0xA6 || 0xAD == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x55 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF4 ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0x9B || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA4 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x56 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ((const U8*)s)[2] & 0xDF ) == 0x83 ) ) ? 3 : 0 )\
	: ( 0x57 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x82 ) || ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x58 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x59 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x89 ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x5A ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x68 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x69 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xFD ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( 0x6A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( 0x74 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ((const U8*)s)[2] == 0xA3 || ( ( ((const U8*)s)[2] & 0xFE ) == 0xA6 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( 0x77 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xF5 ) == 0x80 ) || ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0xA3 ) ) ? 3 : 0 )\
	: ( ( ( 0x79 == ((const U8*)s)[0] ) && ( 0xCC == ((const U8*)s)[1] ) ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x87 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part32(s,e,is_utf8)                         \
( ( is_utf8 ) ?                                                             \
	( ( ( ((const U8*)s)[0] & 0xDF ) == 0x41 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x86 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0x8C == ((const U8*)s)[2] || 0x8F == ((const U8*)s)[2] || 0x91 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xA5 == ((const U8*)s)[2] || 0xA8 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x42 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x43 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xDF ) == 0x87 ) || ((const U8*)s)[2] == 0x8C ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x44 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x45 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x46 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x47 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0xA7 ) ) ? 3 : 0 )\
	: ( 0x48 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE ) ) ? 3 : 0 )\
	: ( 0x49 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( 0x4A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4B ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4C ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4D ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x81 == ((const U8*)s)[2] || 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4E ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xDB ) == 0x83 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4F ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0x8B ) || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0x9B || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x50 ) ? is_DECOMPOSED_REST_safe_part28(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part29(s,e,is_utf8) )\
    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part33(s,e,is_utf8)                         \
( ( ((e)-(s) > 2) && ( is_utf8 ) ) ? ( ( ( ((const U8*)s)[0] & 0xDF ) == 0x41 ) ?\
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( 0x80 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x84 ) || ( 0x86 <= ((const U8*)s)[2] && ((const U8*)s)[2] <= 0x8A ) || 0x8C == ((const U8*)s)[2] || 0x8F == ((const U8*)s)[2] || 0x91 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xA5 == ((const U8*)s)[2] || 0xA8 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x42 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x43 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xDF ) == 0x87 ) || ((const U8*)s)[2] == 0x8C ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x44 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x45 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x46 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x47 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x82 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0xA7 ) ) ? 3 : 0 )\
	: ( 0x48 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x82 || ((const U8*)s)[2] == 0x87 || ( ( ((const U8*)s)[2] & 0xFB ) == 0x88 ) || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAE ) ) ? 3 : 0 )\
	: ( 0x49 == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ( ( ((const U8*)s)[2] & 0xFC ) == 0x80 ) || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ( ( ((const U8*)s)[2] & 0xFE ) == 0x88 ) || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA3 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xB0 ) ) ? 3 : 0 )\
	: ( 0x4A == ((const U8*)s)[0] ) ?                                   \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x82 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4B ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x81 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x4C ) ? is_DECOMPOSED_REST_safe_part30(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part31(s,e,is_utf8) ) : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part34(s,e,is_utf8)                         \
( ( is_utf8 ) ?                                                             \
	( ( ( ((const U8*)s)[0] & 0xDF ) == 0x41 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA5 || ((const U8*)s)[2] == 0xA8 ) ? 3\
		: ( ( ((const U8*)s)[2] & 0xFB ) == 0x82 ) ?                \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x8A == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFB ) == 0x82 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x42 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x43 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ? 3\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x44 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x45 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xF6 ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x86 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x46 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x47 ) ? is_DECOMPOSED_REST_safe_part8(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part9(s,e,is_utf8) )\
    : 0 )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe_part35(s,e,is_utf8)                         \
( ((e)-(s) > 5) ?                                                           \
    ( ( is_utf8 ) ?                                                         \
	( ( ( ((const U8*)s)[0] & 0xDF ) == 0x41 ) ? is_DECOMPOSED_REST_safe_part18(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part19(s,e,is_utf8) )\
    : 0 )                                                                   \
: ((e)-(s) > 4) ?                                                           \
    ( ( is_utf8 ) ?                                                         \
	( ( ( ((const U8*)s)[0] & 0xDF ) == 0x41 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xFE ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xF7 ) == 0x84 ) || ((const U8*)s)[2] == 0x89 || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA5 || ((const U8*)s)[2] == 0xA8 ) ? 3\
		: ( ( ((const U8*)s)[2] & 0xFB ) == 0x82 ) ?                \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x87 == ((const U8*)s)[2] || 0x88 == ((const U8*)s)[2] ) ?\
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x84 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x8A == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFB ) == 0x82 ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x42 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] || 0xA3 == ((const U8*)s)[2] || 0xB1 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x43 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( 0x81 == ((const U8*)s)[2] || 0x82 == ((const U8*)s)[2] ) || 0x87 == ((const U8*)s)[2] || 0x8C == ((const U8*)s)[2] ) ? 3\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x81 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x44 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( ((const U8*)s)[2] == 0x87 || ((const U8*)s)[2] == 0x8C || ( ( ((const U8*)s)[2] & 0xFB ) == 0xA3 ) || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB1 ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x45 ) ?                        \
	    ( ( 0xCC == ((const U8*)s)[1] ) ?                               \
		( ( ( ( ((const U8*)s)[2] & 0xF6 ) == 0x80 ) || ((const U8*)s)[2] == 0x83 || ( ( ((const U8*)s)[2] & 0xFE ) == 0x86 ) || ((const U8*)s)[2] == 0x8C || ((const U8*)s)[2] == 0x8F || ((const U8*)s)[2] == 0x91 || ((const U8*)s)[2] == 0xA8 || ((const U8*)s)[2] == 0xAD || ((const U8*)s)[2] == 0xB0 ) ? 3\
		: ( 0x82 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 || 0x83 == ((const U8*)s)[4] || 0x89 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0x84 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( ( ((const U8*)s)[4] & 0xFE ) == 0x80 ) ) ? 5 : 3 )\
		: ( 0xA3 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x82 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: ( 0xA7 == ((const U8*)s)[2] ) ?                           \
		    ( ( ( 0xCC == ((const U8*)s)[3] ) && ( 0x86 == ((const U8*)s)[4] ) ) ? 5 : 3 )\
		: 0 )                                                       \
	    : 0 )                                                           \
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x46 ) ?                        \
	    ( ( ( 0xCC == ((const U8*)s)[1] ) && ( 0x87 == ((const U8*)s)[2] ) ) ? 3 : 0 )\
	: ( ( ((const U8*)s)[0] & 0xDF ) == 0x47 ) ? is_DECOMPOSED_REST_safe_part24(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part25(s,e,is_utf8) )\
    : 0 )                                                                   \
: ((e)-(s) > 3) ? is_DECOMPOSED_REST_safe_part32(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part33(s,e,is_utf8) )


/*** GENERATED CODE ***/
#define is_DECOMPOSED_REST_safe(s,e,is_utf8)                                \
( ((e)-(s) > 7) ? is_DECOMPOSED_REST_safe_part34(s,e,is_utf8) : is_DECOMPOSED_REST_safe_part35(s,e,is_utf8) )

#endif /* H_REGCHARCLASS */
/* ex: set ro: */
