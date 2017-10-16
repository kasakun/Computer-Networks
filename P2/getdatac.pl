#!/bin/perl
#$infile=$ARGV[0];
open ($in, "<", "p2c.tr") || die File $p2a Not Found;
####################################################################################
open ($out04, ">", "./p2c/flow0.csv") || die "Can`t open file`";
open ($out05, ">", "./p2c/flow1.csv") || die "Can`t open file`";
open ($out06, ">", "./p2c/flow2.csv") || die "Can`t open file`";
open ($out07, ">", "./p2c/flow3.csv") || die "Can`t open file`";
open ($out0G2, ">", "./p2c/flow4.csv") || die "Can`t open file`";

open ($out14, ">", "./p2c/flow5.csv") || die "Can`t open file`";
open ($out15, ">", "./p2c/flow6.csv") || die "Can`t open file`";
open ($out16, ">", "./p2c/flow7.csv") || die "Can`t open file`";
open ($out17, ">", "./p2c/flow8.csv") || die "Can`t open file`";
open ($out1G2, ">", "./p2c/flow9.csv") || die "Can`t open file`";

open ($out24, ">", "./p2c/flow10.csv") || die "Can`t open file`";
open ($out25, ">", "./p2c/flow11.csv") || die "Can`t open file`";
open ($out26, ">", "./p2c/flow12.csv") || die "Can`t open file`";
open ($out27, ">", "./p2c/flow13.csv") || die "Can`t open file`";
open ($out2G2, ">", "./p2c/flow14.csv") || die "Can`t open file`";

open ($out34, ">", "./p2c/flow15.csv") || die "Can`t open file`";
open ($out35, ">", "./p2c/flow16.csv") || die "Can`t open file`";
open ($out36, ">", "./p2c/flow17.csv") || die "Can`t open file`";
open ($out37, ">", "./p2c/flow18.csv") || die "Can`t open file`";
open ($out3G2, ">", "./p2c/flow19.csv") || die "Can`t open file`";

open ($outG1G2, ">", "./p2c/flow20.csv") || die "Can`t open file`";

open ($out40, ">", "./p2c/flow21.csv") || die "Can`t open file`";
open ($out41, ">", "./p2c/flow22.csv") || die "Can`t open file`";
open ($out42, ">", "./p2c/flow23.csv") || die "Can`t open file`";
open ($out43, ">", "./p2c/flow24.csv") || die "Can`t open file`";
open ($out4G1, ">", "./p2c/flow25.csv") || die "Can`t open file`";

open ($out50, ">", "./p2c/flow26.csv") || die "Can`t open file`";
open ($out51, ">", "./p2c/flow27.csv") || die "Can`t open file`";
open ($out52, ">", "./p2c/flow28.csv") || die "Can`t open file`";
open ($out53, ">", "./p2c/flow29.csv") || die "Can`t open file`";
open ($out5G1, ">", "./p2c/flow30.csv") || die "Can`t open file`";

open ($out60, ">", "./p2c/flow31.csv") || die "Can`t open file`";
open ($out61, ">", "./p2c/flow32.csv") || die "Can`t open file`";
open ($out62, ">", "./p2c/flow33.csv") || die "Can`t open file`";
open ($out63, ">", "./p2c/flow34.csv") || die "Can`t open file`";
open ($out6G1, ">", "./p2c/flow35.csv") || die "Can`t open file`";

open ($out70, ">", "./p2c/flow36.csv") || die "Can`t open file`";
open ($out71, ">", "./p2c/flow37.csv") || die "Can`t open file`";
open ($out72, ">", "./p2c/flow38.csv") || die "Can`t open file`";
open ($out73, ">", "./p2c/flow39.csv") || die "Can`t open file`";
open ($out7G1, ">", "./p2c/flow40.csv") || die "Can`t open file`";

############################################################################################


$counter0=0;$counter1=0;$counter2=0;$counter3=0;$counter4=0;$counter5=0;$counter6=0;$counter7=0;$counter8=0;$counter9=0;
$counter10=0;$counter11=0;$counter12=0;$counter13=0;$counter14=0;$counter15=0;$counter16=0;$counter17=0;$counter18=0;$counter19=0;
$counter20=0;$counter21=0;$counter22=0;$counter23=0;$counter24=0;$counter25=0;$counter26=0;$counter27=0;$counter28=0;$counter29=0;
$counter30=0;$counter31=0;$counter32=0;$counter33=0;$counter34=0;$counter35=0;$counter36=0;$counter37=0;$counter38=0;$counter39=0;
$counter40=0;

while(<$in>) {
    @x = split( );
    if($x[0] eq "-")
    {
    # if($x[2] =~ /NodeList\/2\//)
    # {
    # $counter=$counter+1;
    # }
    	if($x[36] =~ /(\d+)/) {
    	    if($1 == 1) {
                next;
            }
            $num = $1;
    	}
        ##################################################
        if($x[28] =~ /10.1.1.1/ && $x[30] =~ /10.1.6.2/)
        {
            
	        $counter0=$counter0+1;
            print $out04 "$x[1] \t $counter0 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.1.1/ && $x[30] =~ /10.1.7.2/)
        {   

            $counter1=$counter1+1;
            print $out05 "$x[1] \t $counter1 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.1.1/ && $x[30] =~ /10.1.8.2/)
        {   

            $counter2=$counter2+1;
            print $out06 "$x[1] \t $counter2 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.1.1/ && $x[30] =~ /10.1.9.2/)
        {   

            $counter3=$counter3+1;
            print $out07 "$x[1] \t $counter3 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.1.1/ && $x[30] =~ /10.1.5.2/)
        {   

            $counter32=$counter32+1;
            print $out0G2 "$x[1] \t $counter32 \t $num  \n"
        }
        ##################################################
        elsif($x[28] =~ /10.1.2.1/ && $x[30] =~ /10.1.6.2/)
        {
            
            $counter4=$counter4+1;
            print $out14 "$x[1] \t $counter4 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.2.1/ && $x[30] =~ /10.1.7.2/)
        {   

            $counter5=$counter5+1;
            print $out15 "$x[1] \t $counter5 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.2.1/ && $x[30] =~ /10.1.8.2/)
        {   

            $counter6=$counter6+1;
            print $out16 "$x[1] \t $counter6 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.2.1/ && $x[30] =~ /10.1.9.2/)
        {   

            $counter7=$counter7+1;
            print $out17 "$x[1] \t $counter7 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.2.1/ && $x[30] =~ /10.1.5.2/)
        {   

            $counter33=$counter33+1;
            print $out1G2 "$x[1] \t $counter32 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.3.1/ && $x[30] =~ /10.1.6.2/)
        {
            
            $counter8=$counter8+1;
            print $out24 "$x[1] \t $counter8 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.3.1/ && $x[30] =~ /10.1.7.2/)
        {   

            $counter9=$counter9+1;
            print $out25 "$x[1] \t $counter9 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.3.1/ && $x[30] =~ /10.1.8.2/)
        {   

            $counter10=$counter10+1;
            print $out26 "$x[1] \t $counter10 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.3.1/ && $x[30] =~ /10.1.9.2/)
        {   

            $counter11=$counter11+1;
            print $out27 "$x[1] \t $counter11 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.3.1/ && $x[30] =~ /10.1.5.2/)
        {   

            $counter34=$counter34+1;
            print $out2G2 "$x[1] \t $counter34 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.4.1/ && $x[30] =~ /10.1.6.2/)
        {
            
            $counter12=$counter12+1;
            print $out34 "$x[1] \t $counter12 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.4.1/ && $x[30] =~ /10.1.7.2/)
        {   

            $counter13=$counter13+1;
            print $out35 "$x[1] \t $counter13 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.4.1/ && $x[30] =~ /10.1.8.2/)
        {   

            $counter14=$counter14+1;
            print $out36 "$x[1] \t $counter14 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.4.1/ && $x[30] =~ /10.1.9.2/)
        {   

            $counter15=$counter15+1;
            print $out37 "$x[1] \t $counter15 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.4.1/ && $x[30] =~ /10.1.5.2/)
        {   

            $counter35=$counter35+1;
            print $out3G2 "$x[1] \t $counter35 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.5.1/ && $x[30] =~ /10.1.5.2/)
        {   

            $counter36=$counter36+1;
            print $outG1G2 "$x[1] \t $counter36 \t $num  \n"
        }
        
        ####################################################
        elsif($x[28] =~ /10.1.6.2/ && $x[30] =~ /10.1.1.1/)
        {
            
            $counter16=$counter16+1;
            print $out40 "$x[1] \t $counter16 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.6.2/ && $x[30] =~ /10.1.2.1/)
        {   

            $counter17=$counter17+1;
            print $out41 "$x[1] \t $counter17 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.6.2/ && $x[30] =~ /10.1.3.1/)
        {   

            $counter18=$counter18+1;
            print $out42 "$x[1] \t $counter18 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.6.2/ && $x[30] =~ /10.1.4.1/)
        {   

            $counter19=$counter19+1;
            print $out43 "$x[1] \t $counter19 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.6.2/ && $x[30] =~ /10.1.5.1/)
        {   

            $counter37=$counter37+1;
            print $out4G1 "$x[1] \t $counter37 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.7.2/ && $x[30] =~ /10.1.1.1/)
        {
            
            $counter20=$counter20+1;
            print $out50 "$x[1] \t $counter20 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.7.2/ && $x[30] =~ /10.1.2.1/)
        {   

            $counter21=$counter21+1;
            print $out51 "$x[1] \t $counter21 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.7.2/ && $x[30] =~ /10.1.3.1/)
        {   

            $counter22=$counter22+1;
            print $out52 "$x[1] \t $counter22 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.7.2/ && $x[30] =~ /10.1.4.1/)
        {   

            $counter23=$counter23+1;
            print $out53 "$x[1] \t $counter23 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.7.2/ && $x[30] =~ /10.1.5.1/)
        {   

            $counter38=$counter38+1;
            print $out5G1 "$x[1] \t $counter38 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.8.2/ && $x[30] =~ /10.1.1.1/)
        {
            
            $counter24=$counter24+1;
            print $out60 "$x[1] \t $counter24 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.8.2/ && $x[30] =~ /10.1.2.1/)
        {   

            $counter25=$counter25+1;
            print $out61 "$x[1] \t $counter25 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.8.2/ && $x[30] =~ /10.1.3.1/)
        {   

            $counter26=$counter26+1;
            print $out62 "$x[1] \t $counter26 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.8.2/ && $x[30] =~ /10.1.4.1/)
        {   

            $counter27=$counter27+1;
            print $out63 "$x[1] \t $counter27 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.8.2/ && $x[30] =~ /10.1.5.1/)
        {   

            $counter39=$counter39+1;
            print $out6G1 "$x[1] \t $counter39 \t $num  \n"
        }
        ####################################################
        elsif($x[28] =~ /10.1.9.2/ && $x[30] =~ /10.1.1.1/)
        {
            
            $counter28=$counter28+1;
            print $out70 "$x[1] \t $counter28 \t $num \n"
        } 
        elsif($x[28] =~ /10.1.9.2/ && $x[30] =~ /10.1.2.1/)
        {   

            $counter29=$counter29+1;
            print $out71 "$x[1] \t $counter29 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.9.2/ && $x[30] =~ /10.1.3.1/)
        {   

            $counter30=$counter30+1;
            print $out72 "$x[1] \t $counter30 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.9.2/ && $x[30] =~ /10.1.4.1/)
        {   

            $counter31=$counter31+1;
            print $out73 "$x[1] \t $counter31 \t $num  \n"
        }
        elsif($x[28] =~ /10.1.9.2/ && $x[30] =~ /10.1.5.1/)
        {   

            $counter40=$counter40+1;
            print $out7G1 "$x[1] \t $counter40 \t $num  \n"
        }
    }

}
# printf \n\tNo of packets received at the destination = %d \n\n, $counter;
# close DATA;
close $in || die "$in: $!";

close $out04 || die "$out04: $!";
close $out05 || die "$out05: $!";
close $out06 || die "$out06: $!";
close $out07 || die "$out07: $!";
close $out0G2 || die "$out0G2: $!";

close $out14 || die "$out14: $!";
close $out15 || die "$out15: $!";
close $out16 || die "$out16: $!";
close $out17 || die "$out17: $!";
close $out1G2 || die "$out1G2: $!";

close $out24 || die "$out24: $!";
close $out25 || die "$out25: $!";
close $out26 || die "$out26: $!";
close $out27 || die "$out207: $!";
close $out2G2 || die "$out0G2: $!";

close $out34 || die "$out34: $!";
close $out35 || die "$out35: $!";
close $out36 || die "$out36: $!";
close $out37 || die "$out37: $!";
close $out3G2 || die "$out3G2: $!";

close $outG1G2 || die "$outG1G2: $!";

close $out40 || die "$out40: $!";
close $out41 || die "$out41: $!";
close $out42 || die "$out42: $!";
close $out43 || die "$out43: $!";
close $out4G1 || die "$out4G1: $!";

close $out50 || die "$out50: $!";
close $out51 || die "$out51: $!";
close $out52 || die "$out52: $!";
close $out53 || die "$out53: $!";
close $out5G1 || die "$out5G1: $!";

close $out60 || die "$out60: $!";
close $out61 || die "$out61: $!";
close $out62 || die "$out62: $!";
close $out63 || die "$out63: $!";
close $out6G1 || die "$out6G1: $!";

close $out70 || die "$out70: $!";
close $out71 || die "$out71: $!";
close $out72 || die "$out72: $!";
close $out73 || die "$out73: $!";
close $out7G1 || die "$out7G1: $!";

exit(0);
