# Author: Ruijia Wang
#!/bin/perl
#$infile=$ARGV[0];
# open ($in, "<", "p4_Local_10ms_null.txt") || die "Can`t open file";
# open ($out, ">", "Local.txt");
# open ($in, "<", "p4_Seq_10ms_null.txt") || die "Can`t open file";
# open ($out, ">", "Seq.txt");
open ($in, "<", "p4_Uniform_10ms_null.txt") || die "Can`t open file";
open ($out, ">", "Uniform.txt");
$counter=0;

while(<$in>)
{
    @x = split();
    $counter=$counter+1;
    print $out "$x[4] \t$counter\n";
}

close $in;
close $out;
exit(0);