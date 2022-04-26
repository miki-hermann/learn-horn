#!/usr/bin/perl

system 'unxz --keep --force digit.csv.xz';
print 'Which digit (0-9) do you want to recognize? ';
my $digit = <>;
chomp $digit;
print "Recognizing digit $digit\n";

my $filename = 'digit' . $digit . '.csv';

open(fh_read, '<', 'digit.csv') or die "$!\n";
open(fh_write, '>', $filename) or die "$!\n";

while (my $line = <fh_read>) {
    chomp $line;
    my @line = split /,/, $line;
    $line[0] = $line[0] == $digit ? 1 : 0;
    $line = join ',', @line;
    print fh_write $line, "\n";
}

close(fh_read);
close(fh_write);
system 'rm -f digit.csv';
print "CSV file $filename generated\n";
