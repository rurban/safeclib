#!/usr/bin/perl -n
=pod
arg1: tests-bos.log
check every log -Wuser-defined-warnings against the test src
if one TEST_BOS warning is missing.

  CC       test_memcpy_s.o
test_memcpy_s.c:42:39: warning: dest overflow or empty [-Wuser-defined-warnings]
    rc = memcpy_s(NULL, LEN, mem2, LEN);
=cut

if (/^  CC       test_(.*_s).o/) {
  push @sources, $1;
  $tests{$1} = [];
} elsif (/^test_(.*_s).c:(\d+):\d+: warning: (.+) \[-Wuser-defined-warnings\]/) {
  push @{$tests{$1}}, ([$2, $3]);
}

END {
  # match result against found warnings
  # print join(' ', sort @sources), "\n";
  my $errs;
  for my $t (sort keys %tests) {
    my $got = "";
    for my $w (@{$tests{$t}}) {
      $got .= "test_$t.c:$w->[0]: error:\t$w->[1]\n"; # [line, warning]
    }
    my $expected = `./t_$t | grep EXPECT_BOS`;
    my ($todo) = $expected =~ /EXPECT_BOS_TODO (test_$t\.c:\d+\t.*?)\n/g;
    $expected =~ s/EXPECT_BOS (test_$t\.c:\d+)\t/$1: error:\t/gs;
    if ($todo) {
      $expected =~ s/EXPECT_BOS_TODO test_$t\.c:\d+\t.*?\n//gs;
    }
    if (($got ne $expected)) {
      if ($todo) {
        print "PASS $t #TODO $todo\n";
      } else {
        print "FAIL $t\n";
       $errs++;
     }
      if ( eval { require Text::Diff } ) {
        import Text::Diff;
        print diff(\$expected, \$got, { STYLE => "MyUni" });
      } else {
        print "expected:\n$expected";
        print "got:\n$got\n";
      }
    } else {
      if ($todo) {
        print "PASS $t #TODO $todo\n";
      } else {
        print "PASS $t\n";
      }
    }
  }
  exit($errs);
}

package MyUni;
@MyUni::ISA = ('Text::Diff::Unified');
sub MyUni::hunk {
    shift; ## No instance data
    pop; ## Ignore options
    my $ops = pop;
    my $prefixes = { "+" => "  ", " " => "# ", "-" => "< " };
    return join "", map Text::Diff::_op_to_line( \@_, $_, undef, $prefixes ), @$ops
}
