#!/usr/bin/env perl
# safeclib - Safe C library extensions.
# Copyright 2026 Reini Urban
# SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
#
# Generates the indirect Unicode normalization tables used by safeclib:
#   u8   -> src/extu8/un8ifc*.h
#   w16  -> src/extwchar/unw16if*.h
#   w32  -> src/extwchar/unwif*.h
#
# This is an extended standalone port of Unicode::Normalize's mkheader
# script and libu8ident's mknorm.pl.  It does not depend on Perl's
# bundled unicore tables.  Canonical_Combining_Class,
# Decomposition_Mapping and Full_Composition_Exclusion are parsed from:
#   UnicodeData.txt
#   DerivedNormalizationProps.txt
# (https://www.unicode.org/Public/UNIDATA/).
#
# Usage: perl mknorm.pl [--type u8|w16|w32|all]
#                       [--ucd-dir DIR] [--output-root DIR]
#                       [--download|--no-download]

use 5.006;
use strict;
use warnings;
no warnings 'redefine';
use Carp;
use File::Path qw(mkpath);
use File::Spec;
use FindBin;
use Getopt::Long qw(GetOptions);

our $PACKAGE = 'safeclib, mknorm.pl';
our $VERSION = '0.5';

BEGIN {
    unless ('A' eq pack('U', 0x41)) {
        die "mknorm.pl cannot stringify a Unicode code point\n";
    }
    unless (0x41 == unpack('U', 'A')) {
        die "mknorm.pl cannot get a Unicode code point\n";
    }
}

sub usage {
    my ($status) = @_;
    my $fh = $status ? *STDERR : *STDOUT;
    print $fh <<"USAGE";
Usage: $0 [options]
  --type u8|w16|w32|all  Header family to generate; repeatable (default: all)
  --ucd-dir DIR          Directory containing the Unicode data files
  --output-root DIR      safeclib source root (default: this script's directory)
  --[no-]download        Refresh Unicode data with wget (default: download)
  --help                 Show this help
USAGE
    exit $status;
}

my @types;
my $ucd_dir = $FindBin::Bin;
my $output_root = $FindBin::Bin;
my $download = 1;
my $help;
GetOptions(
    'type=s@'       => \@types,
    'ucd-dir=s'     => \$ucd_dir,
    'output-root=s' => \$output_root,
    'download!'     => \$download,
    'help'           => \$help,
) or usage(2);
usage(0) if $help;

@types = ('all') unless @types;
@types = map { split /,/, $_ } @types;
my @expanded_types;
for my $type (@types) {
    push @expanded_types, $type eq 'all' ? qw(u8 w16 w32) : $type;
}
@types = @expanded_types;
my %valid_type = map { $_ => 1 } qw(u8 w16 w32);
for my $type (@types) {
    usage(2) unless $valid_type{$type};
}
my %seen;
@types = grep { !$seen{$_}++ } @types;

$ucd_dir = File::Spec->rel2abs($ucd_dir);
$output_root = File::Spec->rel2abs($output_root);
mkpath($ucd_dir) unless -d $ucd_dir;

my @ucd_names = qw(UnicodeData.txt DerivedNormalizationProps.txt);
for my $name (@ucd_names) {
    my $path = File::Spec->catfile($ucd_dir, $name);
    if ($download) {
        my $url = "https://www.unicode.org/Public/UNIDATA/$name";
        system 'wget', '-N', '-P', $ucd_dir, $url;
    }
    -e $path or die "Cannot download or find $path\n";
}

# Generate multiple families in fresh subprocesses.  The table writer retains
# the selected encoding in package globals, so one process emits one family.
if (@types > 1) {
    my $script = File::Spec->catfile($FindBin::Bin, $FindBin::Script);
    for my $type (@types) {
        my @cmd = (
            $^X, $script, "--type=$type", "--ucd-dir=$ucd_dir",
            "--output-root=$output_root", '--no-download'
        );
        system @cmd;
        die "$PACKAGE: failed to generate $type headers\n" if $?;
    }
    exit 0;
}

my $type = $types[0];
my ($is_uni, $is_utf16, $relative_prefix, $write_exc);
if ($type eq 'u8') {
    ($is_uni, $is_utf16, $relative_prefix, $write_exc) =
        (0, 0, [qw(src extu8 un8if)], 0);
} elsif ($type eq 'w16') {
    ($is_uni, $is_utf16, $relative_prefix, $write_exc) =
        (1, 1, [qw(src extwchar unw16if)], 1);
} else {
    ($is_uni, $is_utf16, $relative_prefix, $write_exc) =
        (1, 0, [qw(src extwchar unwif)], 1);
}

my $ucd = File::Spec->catfile($ucd_dir, $ucd_names[0]);
my $dnp = File::Spec->catfile($ucd_dir, $ucd_names[1]);
my @prefix_parts = @$relative_prefix;
my $file_prefix = pop @prefix_parts;
my $output_dir = File::Spec->catdir($output_root, @prefix_parts);
mkpath($output_dir) unless -d $output_dir;

# All safeclib tables use indirect storage and stdint types.
our ($ind, $std) = (1, 1);
our $uni = $is_uni;
our $utf16 = $is_utf16;
our $w8 = $uni ? "W" : "8";
our $prefix = "UN$w8" . ($ind ? "IF_" : "F_");
our $fprefix = File::Spec->catfile($output_dir, $file_prefix);
our $structname = "${prefix}complist";

# Starting in v5.20, the tables in lib/unicore are built using the platform's
# native character set for code points 0-255.
*pack_U = ($] ge 5.020)
          ? sub { return pack('W*', @_).pack('U*'); } # The empty pack returns
                                                      # an empty UTF-8 string,
                                                      # so the effect is to
                                                      # force the return into
                                                      # being UTF-8.
          : sub { return pack('U*', @_); };

# %Canon and %Compat will be ($codepoint => \@codepoints) after exhaustive
# decomposition below.
our %Comp1st;	# $codepoint => $listname  : may be composed with a next char.
our %CompList;	# $listname,$2nd  => $codepoint : composite

# http://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
our %Combin;	# $codepoint => $number    : combining class values
our %Canon;	# $codepoint => \@codepoints : canonical decomp.
our %Compat;	# $codepoint => \@codepoints : compat. decomp.
our %Compos;	# $1st,$2nd  => $codepoint : composite
our %Exclus;	# $codepoint => 1          : composition exclusions
our %Single;	# $codepoint => 1          : singletons
our %NonStD;	# $codepoint => 1          : non-starter decompositions
our %Comp2nd;	# $codepoint => 1          : may be composed with a prev char.
our %FullCompEx; # $codepoint => 1        : Full_Composition_Exclusion (DerivedNormalizationProps.txt)

# definition of Hangul constants
use constant SBase  => 0xAC00;
use constant SFinal => 0xD7A3; # SBase -1 + SCount
use constant SCount =>  11172; # LCount * NCount
use constant NCount =>    588; # VCount * TCount
use constant LBase  => 0x1100;
use constant LFinal => 0x1112;
use constant LCount =>     19;
use constant VBase  => 0x1161;
use constant VFinal => 0x1175;
use constant VCount =>     21;
use constant TBase  => 0x11A7;
use constant TFinal => 0x11C2;
use constant TCount =>     28;

sub decomposeHangul {
    my $sindex = $_[0] - SBase;
    my $lindex = int( $sindex / NCount);
    my $vindex = int(($sindex % NCount) / TCount);
    my $tindex =      $sindex % TCount;
    my @ret = (
       LBase + $lindex,
       VBase + $vindex,
      $tindex ? (TBase + $tindex) : (),
    );
    return wantarray ? @ret :
      $uni ? pack("W*",@ret)
           : pack_U(@ret);
}

########## reading UnicodeData.txt and DerivedNormalizationProps.txt ##########

# UnicodeData.txt fields (';' separated):
#  0 codepoint  3 Canonical_Combining_Class  5 Decomposition_Type&Mapping
# The <First>/<Last> range pairs (Hangul syllables, CJK/private-use
# blocks, surrogates) never carry a combining class or a decomposition,
# so they need no special-casing here.
open my $UCD, "<", $ucd or croak "$PACKAGE: $ucd not found";
while (<$UCD>) {
    chomp;
    next unless length;
    my @f = split /;/, $_, -1;
    my $cp = hex $f[0];
    $Combin{$cp} = $f[3] if $f[3] != 0;
    my $dm = $f[5];
    next unless length $dm;
    my $compat = ($dm =~ s/<[^>]+>//) ? 1 : 0;
    my @dec = map hex, split ' ', $dm;
    $Compat{$cp} = \@dec;
    $Canon{$cp} = \@dec unless $compat;
}
close $UCD;

# Full_Composition_Exclusion = Singleton_Decomposition
#   \cup Non_Starter_Decomposition \cup Script_Specific_Exclusions.
# The first two are derived algorithmically below from %Canon and
# %Combin already; reading this property spares us the old, manually
# curated CompositionExclusions.txt for the Script_Specific part.
open my $DNP, "<", $dnp or croak "$PACKAGE: $dnp not found";
while (<$DNP>) {
    next unless /^([0-9A-Fa-f]+)(?:\.\.([0-9A-Fa-f]+))?\s*;\s*Full_Composition_Exclusion\b/;
    my ($s, $e) = (hex $1, defined $2 ? hex $2 : hex $1);
    $FullCompEx{$_} = 1 for $s .. $e;
}
close $DNP;

# with $ucd (this) we cannot get Unicode::UCD::UnicodeVersion(); parse
# it out of the DerivedNormalizationProps.txt file header instead.
my $ucd_version = 'unknown';
open my $V, "<", $dnp or croak "$PACKAGE: $dnp not found";
my $first = <$V>;
close $V;
$ucd_version = $1 if $first =~ /-([0-9]+\.[0-9]+\.[0-9]+)\.txt/;

##### classify: Singleton / Non-starter decomp / Composition pairs #####

foreach my $u (keys %Canon) {
    my $dec = $Canon{$u};

    if (@$dec == 2) {
	if ($Combin{ $dec->[0] }) {
	    $NonStD{$u} = 1;
	} else {
	    $Exclus{$u} = 1 if $FullCompEx{$u};
	    $Compos{ $dec->[0] }{ $dec->[1] } = $u;
	    $Comp2nd{ $dec->[1] } = 1 if ! $Exclus{$u};
	}
    } elsif (@$dec == 1) {
	$Single{$u} = 1;
    } else {
	my $h = sprintf '%04X', $u;
	croak("Weird Canonical Decomposition of U+$h");
    }
}

# modern HANGUL JUNGSEONG and HANGUL JONGSEONG jamo
foreach my $j (0x1161..0x1175, 0x11A8..0x11C2) {
    $Comp2nd{$j} = 1;
}

sub getCanonList {
    my @src = @_;
    my @dec = map {
	(SBase <= $_ && $_ <= SFinal) ? decomposeHangul($_)
	    : $Canon{$_} ? @{ $Canon{$_} } : $_
    } @src;
    return join(" ",@src) eq join(" ",@dec) ? @dec : getCanonList(@dec);
    # condition @src == @dec is not ok.
}

sub getCompatList {
    my @src = @_;
    my @dec = map {
	(SBase <= $_ && $_ <= SFinal) ? decomposeHangul($_)
	    : $Compat{$_} ? @{ $Compat{$_} } : $_
		} @src;
    return join(" ",@src) eq join(" ",@dec) ? @dec : getCompatList(@dec);
    # condition @src == @dec is not ok.
}

# exhaustive decomposition
foreach my $key (keys %Canon) {
    $Canon{$key}  = [ getCanonList($key) ];
}

# exhaustive decomposition
foreach my $key (keys %Compat) {
    $Compat{$key} = [ getCompatList($key) ];
}

foreach my $comp1st (keys %Compos) {
    my $listname = sprintf("${structname}_%06x", $comp1st);
		# %04x is bad since it'd place _3046 after _1d157.
    $Comp1st{$comp1st} = $listname;
    my $rh1st = $Compos{$comp1st};

    foreach my $comp2nd (keys %$rh1st) {
	my $uc = $rh1st->{$comp2nd};
	$CompList{$listname}{$comp2nd} = $uc;
    }
}

sub split_into_char {
    use bytes;
    my $uni = shift;
    my $len = length($uni);
    my @ary;
    for(my $i = 0; $i < $len; ++$i) {
	push @ary, ord(substr($uni,$i,1));
    }
    return @ary;
}
sub split_utf16 (@) {
    my @v;
    for (@_) {
        if ($_ < 0x10000) {
            push @v, $_;
        } else {
            my $cp = $_ - 0x10000;
            my $d1 = 0xd800 + (($cp >> 10) & 0x3ff);
            my $d2 = 0xdc00 + ($cp & 0x3ff);
            push @v, $d1, $d2;
        }
    }
    @v
}

# lengths in units: uni/utf16: wchar_t, utf8: byte
sub _utf8_len {
    scalar split_into_char(pack_U(@_));
}
sub _uni_len {
    scalar @_;
}
sub _utf16_len {
    scalar split_utf16 @_;
}
# UTF-8 char* string literal
sub _utf8_stringify {
    sprintf '"%s"', join '',
	map sprintf("\\x%02x", $_), split_into_char(pack_U(@_));
}
# wchar_t* string literal
sub _uni_stringify {
    sprintf 'L"%s"', join '',
	map sprintf($_ > 34 && $_ < 127 && $_ != 92 ? "%c" : "\\x%04x", $_), @_;
}
# wchar_t UCS-16 string literal
sub _utf16_stringify {
    sprintf 'L"%s"', join ',',
	map sprintf($_ > 39 && $_ < 127 && $_ != 92 ? "%c" : "\\x%04x", $_),
        split_utf16 @_;
}
# array of single wchar_t without \0
sub _uni_ind_stringify {
    sprintf '{%s}', join ',',
	map sprintf($_ > 39 && $_ < 127 && $_ != 92 && $_ != 39
                    ? "L'%c'" : "L'\\x%04x'", $_), @_;
}
# array of wchar_t UCS-16 characters without \0
sub _utf16_ind_stringify {
    sprintf '{%s}', join ',',
	map sprintf($_ > 39 && $_ < 127 && $_ != 92
                    ? "L'%c'" : "L'\\x%04x'", $_),
        split_utf16 @_;
}
# array of char without \0
sub _utf8_ind_stringify {
    sprintf '{%s}', join ',',
	map sprintf($_ > 32 && $_ < 127 && $_ != 92 && $_ != 39
                    ? "'%c'" : "'\\x%02x'", $_), split_into_char(pack_U(@_));
}

########## writing header files ##########

my @boolfunc = (
    {
	name => "Exclusion",
        desc => "Composite exclusions",
	type => "bool",
	hash => \%Exclus,
    },
    {
	name => "Singleton",
        desc => "Singletons",
	type => "bool",
	hash => \%Single,
    },
    {
	name => "NonStDecomp",
        desc => "non-starter decompositions",
	type => "bool",
	hash => \%NonStD,
    },
    {
	name => "Comp2nd",
        desc => "may be composed with a prev char",
	type => "bool",
	hash => \%Comp2nd,
    },
);

my @h_args = ($VERSION,
              $uni?" -uni":"", $ind?" -ind":"",
              $utf16?" -utf16":"", $std?" -std":"",
              $ucd_version,
              $utf16 ? 16 : $uni ? 32 : 8);
if ($write_exc) {
my $file = $fprefix . "exc.h";
open FH, ">", $file or croak "$PACKAGE: $file can't be made";
binmode FH; select FH;

printf FH << 'EOF', @h_args;
/* ex: set ro ft=c: -*- buffer-read-only: t -*-
 *
 * !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
 * This file is auto-generated by safeclib mknorm.pl %s
 * mkheader%s%s%s%s
 * for Unicode %s UTF-%d
 * Any changes here will be lost!
 */
EOF

foreach my $tbl (@boolfunc) {
    my @temp = sort {$a <=> $b} keys %{$tbl->{hash}};
    my $type = $tbl->{type};
    my $name = $tbl->{name};
    printf "/* %s */\n", $tbl->{desc};
    # decl
    if ($std) {
        print "$type is$name (uint32_t uv);\n\n";
    } else {
        print "$type is$name (UV uv);\n\n";
    }
    # impl
    if ($std) {
        print "$type is$name (uint32_t uv)\n{\n  return\n\t";
    } else {
        print "$type is$name (UV uv)\n{\n  return\n\t";
    }

    while (@temp) {
	my $cur = shift @temp;
	if (@temp && $cur + 1 == $temp[0]) {
	    print "($cur <= uv && uv <= ";
	    while (@temp && $cur + 1 == $temp[0]) {
		$cur = shift @temp;
	    }
	    print "$cur)";
	    print "\n\t|| " if @temp;
	} else {
	    print "uv == $cur";
	    print "\n\t|| " if @temp;
	}
    }
    if ($uni) {
        print "\n\t? 1 : 0;\n}\n\n";
    } else {
        print "\n\t? TRUE : FALSE;\n}\n\n";
    }
}

close FH;
}

####################################

my $compinit = "typedef struct { U32 nextchar; U32 composite; } $structname;\n";
# wint_t is signed with mingw32 > -Woverflow. use uint16_t instead
$compinit .= "typedef struct { U16 nextchar; U16 composite; }"
             ." ${structname}_s;\n\n";
if ($std) {
    $compinit =~ s/ U32 / uint32_t /g;
    $compinit =~ s/ U16 / uint16_t /g;
}
my $complist = "";
my ($maxn, $maxl, $maxr, $nshort) = (0,0,0,0);
my $flong;

foreach my $i (sort keys %CompList) {
    my $n = 1 + scalar keys %{ $CompList{$i} };
    my @v = sort {$a <=> $b } keys %{ $CompList{$i} };
    my $lastv = $v[$#v];
    if ($flong or $lastv > 0xffff or $CompList{$i}{$lastv} > 0xffff) {
        $nshort++;
        unless ($flong) {
            $flong = $i;
            $flong =~ s/^.*_complist_//;
            $flong = "0x".$flong;
        }
        $complist .= "static const $structname $i [$n] = {\n";
    } else {
        $complist .= "static const ${structname}_s $i [$n] = {\n";
    }
    $complist .= join ",\n",
      map sprintf("\t{ 0x%x, 0x%x }", $_, $CompList{$i}{$_}), @v;
    $complist .= ", { 0, 0 }};\n"; # need the sentinel
    # statistics
    $maxn = $n if $n > $maxn;
    $maxl = $lastv if $lastv > $maxl;
    $maxr = $CompList{$i}{$lastv} if $CompList{$i}{$lastv} > $maxr;
}
# how many > short (0xffff)
$compinit .= sprintf("/* max nextchar: %d/0x%x, max composite: %d/0x%x, max length: %d */\n",
                     $maxl, $maxl, $maxr, $maxr, $maxn);
$compinit .= sprintf("/* %d/%d lists > short (0xffff) */\n\n",
                     $nshort, scalar keys  %CompList);
$compinit .= "#define ${prefix}COMPLIST_FIRST_LONG $flong\n\n";
$compinit .= $complist;
# TODO: create the complist rows as bitmap, not array of char

my @tripletable = (
    {
	file => $fprefix . "cmb.h",
	name => "combin",
        desc => "CombiningClass",
	type => $uni ? 'uint8_t' : "STDCHAR",
	hash => \%Combin,
	null =>  0,
    },
    {
	file => $fprefix . "can.h",
	name => "canon",
        desc => "Canonical Decomposition",
	type => $uni ? "wchar_t*" : "char*",
	hash => \%Canon,
	null => "NULL",
    },
    {
	file => $fprefix . "cpt.h",
	name => "compat",
        desc => "Compat. Decomposition",
	type => $uni ? "wchar_t*" : "char*",
	hash => \%Compat,
	null => "NULL",
    },
    {
	file => $fprefix . "cmp.h",
	name => "compos",
        desc => "Composition",
	type => "${structname}_s *",
	hash => \%Comp1st,
	null => "NULL",
	init => $compinit,
    },
);

sub maxl {
    return undef unless @_;
    my $max = shift;
    for my $e (@_) {
        $max = $e if defined $e and (!defined $max or $e > $max);
    }
    return $max;
}

sub len_list_stringify {
  local $^W; no warnings;
  local $" = ','; # LIST_SEPARATOR
  my @tl = @_;
  shift @tl;
  "(@tl)"
}

# compat -uni: (1664,1190,638,109,16|14,1,1,....1 for 18)
my %compat_exc; # maxlen = 5
my $indtblrx = qr/^(canon|compat)$/;

foreach my $tbl (@tripletable) {
    my $file = $tbl->{file};
    my $name = $tbl->{name};
    my $head = "${prefix}$name";
    my $type = $tbl->{type};
    my $hash = $tbl->{hash};
    my $null = $tbl->{null};
    my $init = $tbl->{init};
    my $doind = $ind && ($name =~ $indtblrx ? 1 : 0);

    open FH, ">", $file or croak "$PACKAGE: $file can't be made";
    binmode FH; select FH;
    my %val;

    printf FH << 'EOF', @h_args;
/* ex: set ro ft=c: -*- buffer-read-only: t -*-
 * !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
 * This file is auto-generated by safeclib mknorm.pl %s
 * mkheader%s%s%s%s
 * for Unicode %s UTF-%d
 * Any changes here will be lost!
 */
EOF
    printf "/* %s */\n", $tbl->{desc};
    print $init if defined $init;

    my ($indtype, %ival, @ival, @l, $shift, $mask);
    my $stringify = $uni
      ? ($utf16 ? \&_utf16_stringify : \&_uni_stringify)
      : \&_utf8_stringify;
    my $lensub = $uni
      ? ($utf16 ? \&_utf16_len : \&_uni_len)
      : \&_utf8_len;
    if ($name =~ $indtblrx) {
        foreach my $uv (keys %$hash) {
            croak sprintf("a Unicode code point 0x%04X over 0x10FFFF.", $uv)
              unless $uv <= 0x10FFFF;
            my @v = $utf16 ? split_utf16(@{ $hash->{$uv} }) : @{ $hash->{$uv} };
            if ($doind) {
                $stringify = $uni 
                  ? ($utf16 ? \&_utf16_ind_stringify : \&_uni_ind_stringify) 
                  : \&_utf8_ind_stringify;
                # length in wchar/byte
                my $n = $lensub->(@v);
                # skip U+FDFA http://www.unicode.org/reports/tr31/#NFKC_Modifications
                # with length 18 on NFKD (compat).
                if ($n > 5) {
                    $compat_exc{$uv} = $uni ? _uni_stringify( @v ) : _utf8_stringify( @v );
                }
                $ival{$n}{ $stringify->( @v ) } = $uv;
                my @c = unpack 'CCCC', pack 'N', $uv;
                $val{ $c[1] }{ $c[2] }{ $c[3] } = [ @v ];
            }
            else {
                my @c = unpack 'CCCC', pack 'N', $uv;
                $val{ $c[1] }{ $c[2] }{ $c[3] } = $stringify->( @v );
            }
        }
        if ($doind) {
            $null = '0';
            for my $n (sort keys %ival) { # 1-4 for canon, 1-6 for compat
                my @v = sort keys %{$ival{$n}};
                $ival[$n] = [ @v ]; # the values
                $l[$n] = scalar @v;
            }
            print "\n/* Using now indirect tables with indices into the unique values */\n";
            print "/* even sparing the final \\0 */\n\n";
            # find unique values with 1-4 entries
            my $size = 16;
            $indtype = $std ? 'uint16_t' : 'U16';
            if ($l[1] < 256 && $l[2] < 256 && $l[3] < 256 && $l[4] < 256) {
                $indtype = $std ? 'uint8_t' : 'U8';
                $size = 8;
            } elsif ($l[1] > 0xffff or $l[2] > 0xffff or $l[3] > 0xffff or $l[4] > 0xffff) {
                die "Too many indirect tables: @l";
            }
            printf "#undef NORMALIZE_IND_TBL\n";
            printf "#define NORMALIZE_IND_TBL\n";
            print "/* tbl sizes: ",len_list_stringify(@l)," */\n";
            # canon: (917,762,227,36)
            # compat: (1664,1190,638,109,16|14,1,1) # special-case the last
            my $l = @l-1;
            $shift = (2**($size-1) >> $l-1); # highest bit rsh for all lengths
            $mask = $shift - 1;
            printf "/* l: 1-%d */\n", $l;
            my $max = maxl @l;
            printf "/* max size: %d 0x%x */\n", $max, $max;
            if ($max > $mask) {
                # if this is only U+FDFA sallallahou alayhe wasallam => 18 upper case letters
                # handle it seperately. also the len=7+8 rows.
                $l = 5; # special-case the rest
                $shift = (2**($size-1) >> $l-1); # highest bit rsh for all lengths
                $mask = $shift - 1;
                warn sprintf "Overlarge $name tables for indirect %s\n",
                  len_list_stringify(@l);
                die "max > mask" if $max > $mask;
                shift @l;
                @l = @l[0..$l];
            }
            printf "#define %s_MAXLEN  %d\n",   $head, $l;
            printf "#define TBL(i)               ((i-1) << %d)\n", 16-$l;
            printf "/* value = (const $type)\&UN%sIF_${name}_tbl[LEN-1][IDX] */\n",
              $w8;
            printf "#define %s_LEN(v)  (((v) >> %d) + 1)\n", $head, 16-$l;
            printf "#define %s_IDX(v)  ((v) & 0x%x)\n", $head, $mask;
            printf "#define %s_PLANE_T %s\n", $head, $indtype;
            printf "\n";
            print "/* the values */\n";
            for my $n (sort keys %ival) { # 1-5
                my $i = 0;
                my @v = @{$ival[$n]}; # the unique values
                my $size = scalar @v;
                $type =~ s/\*$//;
                next if $n > $l;
                printf "static const $type ${head}_tbl_$n [$size][$n] = {";
                for my $v (@v) {
                    printf "\n /* %3d */ ", $i if $i % 8 == 0;
                    print " ",$v;
                    print ','  if $i != $size-1;
                    $i++;
                }
                print "};\n\n";
            }
            if (%compat_exc) {
                my $c = scalar keys %compat_exc;
                print "/* the special-cased overlong entries */\n";
                my $maxcp = maxl(sort {$a <=> $b} keys %compat_exc);
                my $init = $std
                  ? "typedef struct { const wint_t cp; const $type* v; } ${head}_exc_t;\n"
                  : "typedef struct { const U32 cp; const $type* v; } ${head}_exc_t;\n";
                if ($maxcp < 0xffff) {
                    if ($std) {
                        $init =~ s/ wint_t / uint16_t /;
                    } else {
                        $init =~ s/ U32 / U16 /;
                    }
                } elsif ($std) {
                    $init =~ s/ wint_t / uint32_t /;
                }
                print $init;
                print "/* sorted for binary search */\n";
                printf "#define ${head}_exc_size %d\n", $c;
                printf "static const ${head}_exc_t ${head}_exc [%d] = {\n", $c;
                my $i = 0;
                for my $cp (sort {$a <=> $b} keys %compat_exc) {
                    printf "\t{ 0x%x, %s }", $cp, $compat_exc{$cp};
                    printf "," if ++$i < $c;
                    printf "\n";
                }
                print "};\n\n";
            } else {
                printf "/* no exception table */\n";
                printf "#define ${head}_exc_size %d\n\n", 0;
            }
            printf "static const $type* ${head}_tbl [$l] = {\n";
            for my $i (1..$l) {
                if (exists $ival{$i}) {
                    print "\t(const $type*) ${head}_tbl_$i";
                }
                else {
                    print "\tNULL";
                }
                print (($i == $l) ? "\n" : ",\n");
            }
            print "};\n\n";
        }
    } else {
        if ($name eq 'compat') {
            $stringify = $uni ? \&_uni_stringify : \&_utf8_stringify;
        } else {
            $stringify = sub { @_ }; # identity (list of ints or a string)
        }
        foreach my $uv (keys %$hash) {
            croak sprintf("a Unicode code point 0x%04X over 0x10FFFF.", $uv)
              unless $uv <= 0x10FFFF;
            my @c = unpack 'CCCC', pack 'N', $uv;
            my $v = $hash->{$uv};

            if ($name eq 'compat') {
                my @v = $utf16 ? split_utf16 @$v : @$v;
                $val{ $c[1] }{ $c[2] }{ $c[3] } = $stringify->( @v );
            } else {
                $val{ $c[1] }{ $c[2] }{ $c[3] } = $v;
            }
        }
    }

    print "/* the rows */\n";
    foreach my $p (sort { $a <=> $b } keys %val) {
	next if ! $val{ $p };
	for (my $r = 0; $r < 256; $r++) {
            next if ! $val{ $p }{ $r };
            if ($doind) {
                printf "static const $indtype ${head}_%02x_%02x [256] = {\n", $p, $r;
            } else {
                printf "static const $type ${head}_%02x_%02x [256] = {\n", $p, $r;
            }
	    for (my $c = 0; $c < 256; $c++) {
                my $uv = ($p << 16) + ($r << 8) + $c;
                if ($c % 8 == 0) {
                    if ($p) {
                        printf "/* %02x%02x%02x */ ", $p,$r,$c;
                    } else {
                        printf "/*   %02x%02x */ ", $r,$c;
                    }
                }
                if (defined $val{$p}{$r}{$c}) {
                    if ($doind) {
                        my $i = 0;
                        my @s = @{ $val{$p}{$r}{$c} };
                        my $s = $stringify->( @s );
                        my $n = $lensub->(@s);
                        my @v = @{$ival[$n]}; # the unique values
                        # search for $i, the index into @v
                        for my $v (@v) {
                            if ($v eq $s) {
                                if ($n <= 5) {
                                    $val{$p}{$r}{$c} = sprintf("TBL(%d)|%d", $n, $i);
                                } else {
                                    $val{$p}{$r}{$c} = sprintf("(%s)-1 /*TBL(%d)|%d*/",
                                                               $indtype, $n, $i);
                                }
                            }
                            $i++;
                        }
                        if (ref $val{$p}{$r}{$c} eq 'ARRAY') {
                            die "$p $r $c unresolved: uv=$uv $s @{$val{$p}{$r}{$c}}";
                        }
                    } else {
                        if (ref $val{$p}{$r}{$c} eq 'ARRAY') {
                            die "$file $p $r $c unresolved: uv=$uv @{$val{$p}{$r}{$c}}";
                        }
                    }
                    if ($type =~ /(uint|wchar)/ or $doind) {
                        print " ",$val{$p}{$r}{$c};
                    } elsif ($type =~ /complist/) {
                        my $s = $val{$p}{$r}{$c};
                        my ($c) = $s =~ m/_complist_(......)/;
                        my $i = hex $c;
                        if ($i >= hex $flong) {
                            print " (const ${prefix}complist_s*)", $s;
                        } else {
                            print " ", $s;
                        }
                    } else {
                        print " ($type)".$val{$p}{$r}{$c};
                    }
                } else {
		    print " ",$null;
                }
		print ','  if $c != 255;
		print "\n" if $c % 8 == 7;
	    }
	    print "};\n\n";
	}
    }
    print "/* the planes */\n";
    foreach my $p (sort { $a <=> $b } keys %val) {
	next if ! $val{ $p };
        if ($doind) {
            printf "static const $indtype* ${head}_%02x [256] = {\n", $p;
        } else {
            printf "static const $type* ${head}_%02x [256] = {\n", $p;
        }
	for (my $r = 0; $r < 256; $r++) {
	    print $val{ $p }{ $r }
		? sprintf("${head}_%02x_%02x", $p, $r)
		: "NULL";
	    print ','  if $r != 255;
	    print "\n" if $val{ $p }{ $r } || ($r+1) % 8 == 0;
	}
	print "};\n\n";
    }
    print "/* the main plane */\n";
    if ($doind) {
        printf "#undef TBL\n";
        printf "static const $indtype** ${head} [] = {\n";
    } else {
        print "static const $type** $head [] = {\n";
    }
    for (my $p = 0; $p <= 0x10; $p++) {
	print $val{ $p } ? sprintf("${head}_%02x", $p) : "NULL";
	print ','  if $p != 0x10;
	print "\n";
    }
    print "};\n\n";
    close FH;
}

select STDOUT;
1;
__END__
# Local Variables:
# perl-indent-level: 4
# End:
