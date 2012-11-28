#!/usr/bin/perl

=head1 NAME

  check_for_unsafe_apis - Finds those unsafe APIs.

=head1 SYNOPSIS

  check_for_unsafe_apis [OPTIONS] File [files...]

  Options:
    -h              full documentation

  File - may be a file or directory.

=head1 OPTIONS

=over 8

=item B<-h>, B<--help>, B<--usage>

Prints out this manual page.

=item B<-r>, B<-R>, B<--recursive>

Follows child directories.

=item B<-v>, B<--verbose>

Prints out additional information when searching files.

=item B<-p>, B<--perfile>

Tells the program to keep per file statistics.

=item B<-f> I<output_format>, B<--format> I<output_format>

Request the program to output the results in a different format.

=back

=head1 EXAMPLE

=over 4

=item B<prompt$ check_for_unsafe_apis file.c srcdir>

This will cause the program to check I<file.c> and all *.c sources files
under I<srcdir> for unsafe API calls.

=item B<prompt$ check_for_unsafe_apis -R file.c srcdir>

This will find all unsafe API calls in I<file.c> and any *.c files found under the
I<srcdir> directory tree.

=head1 DESCRIPTION

  Traverses specified directory trees and/or files (cwd by default)
  searching for C source files (*.c), rooting out unsafe API calls.

=head1 AUTHOR

  Created: October 2008, Bo Berry
  Last Modified: April 2009, Jonathan Toppins

=head1 COPYRIGHT

  Copyright (c) 2008, 2009 by Cisco Systems, Inc.
  All rights reserved.

=cut


no lib "/opt/perl/packages/5.004_04/lib/site_perl/";

use strict;
use FileHandle;
use DirHandle;
use Getopt::Long;
use Pod::Usage;


# Global Variables:
my $gFileNamesToCheck = qr/\.((c)|(h))$/;

my $gVerbose         = 0;
my $gRecursive       = 0;
my $gOptPerFileStats = 0;
my $gOptOutputFormat = "std";

my $gNumFilesChecked = 0;

my %gPerFileStats   = ();  # "func_name" => number_encountered
my %gFuncConvertMap = (
    # C11 std APIs (Character Handeling)
    #    - Case Mapping
    "tolower"  => {"convert" => "??",         "num_found" => 0},
    "toupper"  => {"convert" => "??",         "num_found" => 0},

    # C11 std APIs (String handeling)
    #    - Copying
    "memcpy"   => {"convert" => "memcpy_s",   "num_found" => 0},
    "memmove"  => {"convert" => "memmove_s",  "num_found" => 0},
    "strcpy"   => {"convert" => "strcpy_s",   "num_found" => 0},
    "strncpy"  => {"convert" => "strncpy_s",  "num_found" => 0},

    #    - Concatenation
    "strcat"   => {"convert" => "strcat_s",   "num_found" => 0},
    "strncat"  => {"convert" => "strncat_s",  "num_found" => 0},

    #    - Comparison
    "memcmp"   => {"convert" => "memcmp_s",   "num_found" => 0},
    "strcmp"    => {"convert" => "??", "num_found" => 0},
    "strncmp"    => {"convert" => "??", "num_found" => 0},
    "strcoll"    => {"convert" => "??", "num_found" => 0},
    "strxfrm"    => {"convert" => "??", "num_found" => 0},

    #    - Search
    "memchr"    => {"convert" => "??", "num_found" => 0},
    "strchr"    => {"convert" => "??", "num_found" => 0},
    "strcspn"    => {"convert" => "??", "num_found" => 0},
    "strpbrk"    => {"convert" => "??", "num_found" => 0},
    "strrchr"    => {"convert" => "??", "num_found" => 0},
    "strspn"    => {"convert" => "??", "num_found" => 0},
    "strstr"    => {"convert" => "??", "num_found" => 0},
    "strtok"   => {"convert" => "strtok_s",   "num_found" => 0},

    #    - Miscellaneous
    "memset"   => {"convert" => "memset_s",   "num_found" => 0},
    "strerror"    => {"convert" => "??", "num_found" => 0},
    "strlen"   => {"convert" => "strnlen_s",  "num_found" => 0},

    # C11 std APIs (Input/Output)
    "sprintf"  => {"convert" => "NO REPLACEMENT", "num_found" => 0},
    "snprintf" => {"convert" => "NO REPLACEMENT", "num_found" => 0},

    # Extensions
    #    - Posix extensions
    "strnlen"  => {"convert" => "??",         "num_found" => 0},
    "strcasecmp"    => {"convert" => "??", "num_found" => 0},
    "strncasecmp"    => {"convert" => "??", "num_found" => 0},

    #    - BSD extensions
    "strnstr"    => {"convert" => "??", "num_found" => 0},
    "strlcat"    => {"convert" => "??", "num_found" => 0},
    "strlcpy"    => {"convert" => "??", "num_found" => 0},
    "strsep"    => {"convert" => "??", "num_found" => 0},
    "bcopy"    => {"convert" => "memcpy_s",   "num_found" => 0},
    "bzero"    => {"convert" => "memset_s",   "num_found" => 0},

    # Kernel Specific APIs
    #    Linux specific APIs
    "strnicmp"    => {"convert" => "??", "num_found" => 0},
    "strnchr"    => {"convert" => "??", "num_found" => 0},
    "memscan"    => {"convert" => "??", "num_found" => 0},
    "skip_spaces"    => {"convert" => "??", "num_found" => 0},
    "strim"    => {"convert" => "??", "num_found" => 0},
    "memchr_inv"    => {"convert" => "??", "num_found" => 0},
    "kstrdup"    => {"convert" => "??", "num_found" => 0},
    "kstrndup"    => {"convert" => "??", "num_found" => 0},
    "kmemdup"    => {"convert" => "??", "num_found" => 0},
    "argv_split"    => {"convert" => "??", "num_found" => 0},
    "argv_free"    => {"convert" => "??", "num_found" => 0},
    "sysfs_streq"    => {"convert" => "??", "num_found" => 0},
    "strtobool"    => {"convert" => "??", "num_found" => 0},
    "vbin_printf"    => {"convert" => "??", "num_found" => 0},
    "bstr_printf"    => {"convert" => "??", "num_found" => 0},
    "bprintf"    => {"convert" => "??", "num_found" => 0},
    "memory_read_from_buffer"    => {"convert" => "??", "num_found" => 0},
    "strstarts"    => {"convert" => "??", "num_found" => 0},

    #    IOS specific APIs
    "strdiff"  => {"convert" => "??",         "num_found" => 0},
    "memsetl"  => {"convert" => "??",         "num_found" => 0},
    "memchk"   => {"convert" => "??",         "num_found" => 0},
    "memrchk"  => {"convert" => "??",         "num_found" => 0},
    "memchkl"  => {"convert" => "??",         "num_found" => 0},
    "memdiff"  => {"convert" => "??",         "num_found" => 0},
    "sstrncat" => {"convert" => "??",         "num_found" => 0},
    "sstrncpy" => {"convert" => "??",         "num_found" => 0}
);

main();


sub main
{
    Getopts();

    if (@ARGV) {
        CheckFilesOnCommandLine();
    } else {
        # check the current working directory
        Log($gVerbose, "Checking source files in . ");
        if ($gRecursive) {
            Log($gVerbose, "(recursively) ");
        }
        Log($gVerbose, "\n");
        CheckDirectory(".", $gRecursive);
    }

    # Print out the summary results
    PrintResults();
    Exit(0);
}

# prototype: void Getopts(void)
sub Getopts ()
{
    my (%optsVector) = ( "usage"       => \&SetOptionFlag,
                         "recurse"     => \&SetOptionFlag,
                         "verbose"     => \&SetOptionFlag,
                         "perfile"     => \&SetOptionFlag,
                         "format"      => \&SetOutputFormat);
    my (%opts) = ( "usage"      => 0,
                   "recurse"    => 0,
                   "verbose"    => 0,
                   "perfile"    => 0,
                   "format"     => "");
    my (%allowedFlags) = ( "h|help|usage"   => \$opts{"usage"},
                           "r|R|recursive"  => \$opts{"recurse"},
                           "v|verbose"      => \$opts{"verbose"},
                           "p|perfile"      => \$opts{"perfile"},
                           "f|format=s"     => \$opts{"format"});
    my ($key);

    # Configure GetOptions
    Getopt::Long::Configure('posix_default');
    Getopt::Long::Configure('bundling');

    GetOptions(%allowedFlags) || pod2usage(2);

    foreach $key (keys %opts) {
        $optsVector{$key}->($key, $opts{$key});
    }
}

# prototype: void SetOptionFlag(string, int)
sub SetOptionFlag ($$)
{
    my ($flag, $val) = @_;

    if ($val != 1) { return; }
    SWITCH: for ($flag) {
        /^verbose$/     && do { $gVerbose      = 1; last; };
        /^recurse$/     && do { $gRecursive    = 1; last; };
        /^perfile$/     && do { $gOptPerFileStats = 1; last; };
        /^usage$/       && do { pod2usage(-verbose => 2); last; };
        pod2usage("$0: Invalid option flag \"$flag\"\n");
    }
}


sub SetOutputFormat ($$)
{
    my ($flag, $val) = @_;

    if ($val eq "") { return; }
    SWITCH: for ($val) {
        /^std$/     && do { $gOptOutputFormat = $val; last; };
#        /^csv$/     && do { $gOptOutputFormat = $val; last; };
#        /^xml$/     && do { $gOptOutputFormat = $val; last; };
        pod2usage("$0: Invalid output format \"$val\"\n");
    }
}

# prototype: void Log(int, ...)
sub Log
{
    my ($v, @args) = @_;
    my ($arg);

    if (! defined($v)) { return; }

    if (scalar(@args)) {
        for $arg (@args) {
            if ($v) {
                print "$arg";
            }
        }
    }
}

# prototype: void Exit(int, ...)
sub Exit
{
    my ($code, @reason) = @_;
    if (scalar(@reason)) {
        Log(1, @reason);
    }

    exit($code);
}


sub CheckFilesOnCommandLine
{
    foreach my $specifiedFiles (@ARGV) {
        my @files = glob($specifiedFiles);

        foreach my $file (@files) {
            if ( -d $file ) {
                CheckDirectory($file, $gRecursive);
            } else {
                CheckFile($file);
            }
        }
    }
}


sub CheckDirectory
{
    my ($dirName, $recursive) = @_;
    my $dirHandle = new DirHandle($dirName);

    if (not defined $dirHandle) {
        Exit(1, "ERROR: Unable to open directory $dirName ($!)");
    }

    while (defined ($_ = $dirHandle->read)) {
        my $path = $dirName . "/" . $_;

        if ($recursive and (-d $path) and ($_ !~ /^\.[\.]?$/)) {
            if ($recursive) {
                Log($gVerbose, "Recursive $path  \n");
            }
            CheckDirectory($path, $recursive);
        } elsif (-f $path) {
            # TODO: check this match, I don't think it really does what is
            # intended.
            if ($path =~ m/\w+[\.]c$/ ) {
                CheckFile($path);
            }
        }
    }
}


sub CheckFile
{
    my $fileName = shift;
    my $line     = 0;
    my $fh       = 0;
    my $lineNumber = 0;
    my $foundfunc = 0;
    my %rec = ();


    return 0 unless ($fileName =~ $gFileNamesToCheck);

    $fh = new FileHandle;
    if (not open ($fh, $fileName)) {
        Log($gVerbose, "Unable to open file $fileName \n");
        return;
    }

    $gNumFilesChecked++;
    Log($gVerbose, "$fileName \n");

    if ($gOptPerFileStats) {
        foreach my $func (keys %gFuncConvertMap) {
            $rec{$func} = 0;
        }
    }

    while ($line = <$fh>) {
        $lineNumber++;
        foreach my $func (keys %gFuncConvertMap) {
            my $matchline = "[^_]$func";

            if ($line =~ m/$matchline/) {
                $foundfunc = 1;
                Log($gVerbose, " [$lineNumber] $line ");
                $gFuncConvertMap{$func}{"num_found"}++;
                if ($gOptPerFileStats) {
                    $rec{$func}++;
                }
            }
        }
    }

    if ($gOptPerFileStats and $foundfunc) {
        $gPerFileStats{$fileName} = {%rec};
    }
    close($fh);
}

sub PrintResults
{
    if ($gOptPerFileStats) {
        PrintFileData();
    }
    PrintSummary();
}

sub PrintFileData
{
    foreach my $file (sort keys %gPerFileStats) {
        if ($gOptOutputFormat =~ m/^std$/) {
            print "\n";
            print "------------------------------------------------ \n";
            print "File: \n";
            print "  ", $file, "\n";
            print "------------------------------------------------ \n";

            foreach my $func (sort keys %{$gPerFileStats{$file}}) {
                print "$func count = " . $gPerFileStats{$file}{$func} . "\n";
            }
        } elsif ($gOptOutputFormat =~ m/^csv$/) {
            Exit(1, "Problem!\n");
        } else {
            Exit(1, "Problem!\n");
        }
    }
}

sub PrintSummary
{
    if ($gOptOutputFormat =~ m/^std$/) {
        print "\n";
        print "------------------------------------------------ \n";
        print "Summary: \n";
        print "  Files checked  = $gNumFilesChecked files\n";
        if ($gOptPerFileStats) {
            print "  Files affected = " . keys(%gPerFileStats) . " files\n";
        }
        print "------------------------------------------------ \n";

        foreach my $func (sort keys %gFuncConvertMap) {
            print "$func count = " . ${$gFuncConvertMap{$func}}{"num_found"} .
                  "  Replace with " . ${$gFuncConvertMap{$func}}{"convert"} . "\n";
        }
    } elsif ($gOptOutputFormat =~ m/^csv$/) {
        Exit(1, "Problem!\n");
    } else {
        Exit(1, "Problem!\n");
    }
}
