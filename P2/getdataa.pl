#!/bin/perl
#$infile=$ARGV[0];
open ($in, "<", "p2a.tr") || die File $p2a Not Found;
open ($out0, ">", "p2a_N0.csv") || die "Can`t open file`";
open ($out1, ">", "p2a_N1.csv") || die "Can`t open file`";
open ($out2, ">", "p2a_N2.csv") || die "Can`t open file`";
open ($out3, ">", "p2a_N3.csv") || die "Can`t open file`";
$counter0=0;
$counter1=0;
$counter2=0;
$counter3=0;
while(<$in>) {
    @x = split( );
    if($x[0] eq "+")
    {
    # if($x[2] =~ /NodeList\/2\//)
    # {
    # $counter=$counter+1;
    # }
	if($x[36] =~ /(\d+)/) {
	    $num = $1;
	}
        if($x[2] =~ /NodeList\/0\//)
        {
            
	    $counter0=$counter0+1;
            print $out0 "$x[1] \t $counter0 \t $num \n"
        } 
        elsif($x[2] =~ /NodeList\/1\//)
        {   

            $counter1=$counter1+1;
            print $out1 "$x[1] \t $counter1 \t $num  \n"
        }
        elsif($x[2] =~ /NodeList\/2\//)
        {
            $counter2=$counter2+1;
            print $out2 "$x[1] \t $counter2 \t $num \n"
        }
        elsif($x[2] =~ /NodeList\/3\//)
        {
            $counter3=$counter3+1;
            print $out3 "$x[1] \t $counter3 \t $num \n"
        }
    }

}
# printf \n\tNo of packets received at the destination = %d \n\n, $counter;
# close DATA;
close $in || die "$in: $!";
close $out0 || die "$out0: $!";
close $out1 || die "$out1: $!";
close $out2 || die "$out2: $!";
close $out3 || die "$out3: $!";
exit(0);
