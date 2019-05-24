#!/usr/bin/perl

=head1 DESCRIPTION

parse the stderr output of gcov for each src file:

    File 'str/sprintf_s.c'
    Lines executed:90.32% of 31

and update each test file with the new coverage,
printing changes from previous runs.

=cut

use File::Basename qw(dirname basename);

my $file = shift || die "missing input file";
open my $f, "<", $file || die "cannot read input file: $!";
my ($src, $test, $gcov, %warned);
while (<$f>) {
  if (/^File '(.*)'/) {
    $src = $1;
    if ($src =~ /test_/) {
      $src = undef;
    }
  }
  elsif (/^Lines executed:(.*)\n/) {
    $gcov = $1;
    $test = basename $src;
    $test = "tests/test_$test";
    if (-e $test && $src && $gcov) {
      upd_test($test, $src, $gcov);
    } elsif (!exists $warned{$test}) {
      if (!-e $test) {
        warn "Missing $test for $src $gcov\n";
      } elsif (!$src) {
        warn "Missing src for $test $gcov\n";
      } elsif ($gcov) {
        warn "Missing gcov for $test $src\n";
      }
      $warned{$test}++;
    }
  }
}
close $f;

sub upd_test {
  my ($file, $src, $gcov) = @_; # 2 lines, like
#str/sprintf_s.c
#90.32% of 31
  my $t;
  if (!open $t, "<", $file) {
    warn "cannot read test file $file\n";
    return;
  }
  my $base = basename $file;
  $base =~ s/\.c$//;
  my $out = $file . ".tmp";
  open my $o, ">", $out || die "cannot write tmp file $file\n";
  my $found;
  $. = 0;
  while (<$t>) {
    print $o $_;
    if (!$found) {
      if ($. < 3 && /^\s*\*\s*\Q$base\E/) {
        # read next line to see if its already there
        $_ = <$t>;
        if (/^ \* File/) {
          print $o $_;
          $_ = <$t>; # read cov
          my ($old) = $_ =~ /Lines executed:(.*)\n/;
          if ($old ne $gcov) {
            printf STDERR "%-24s\t%s => %s\n", $src, $old, $gcov;
            if ($gcov =~ /^0.00/) {
              $gcov = $old;
            } else {
              $changed++;
            }
          } else {
            printf STDERR "%-24s\t%s\n", $src, $gcov;
          }
          print $o " * Lines executed:$gcov\n";
        } else {
          $changed++;
          print $o " * File '$src'\n";
          print $o " * Lines executed:$gcov\n";
          print $o $_;
          printf STDERR "%-24s\t%s\n", $src, $gcov;
        }
        $found++;
        next;
      } elsif ($. >= 3) {
        $changed++;
        if (/^\s*\*/) {
          print $o " * File '$src'\n";
          print $o " * Lines executed:$gcov\n";
          printf STDERR "%-24s\t%s\n", $src, $gcov;
          $found++;
          next;
        } else {
          print $o "/*------------------------------------------------------------------\n";
          print $o " * $base\n";
          print $o " * File '$src'\n";
          print $o " * Lines executed:$gcov\n";
          print $o " *\n *------------------------------------------------------------------\n*/\n";
          printf STDERR "%-24s\t%s\n", $src, $gcov;
          $found++;
          next;
        }
      }
    }
  }
  close $t;
  close $o;
  if ($changed) {
    rename $file, $file."~";
    rename $out, $file;
  } else {
    unlink $out;
  }
}
