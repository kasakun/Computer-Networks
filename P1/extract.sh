#!/bin/bash

echo "Start Simulation..."
declare -a Type=("TcpNewReno" "TcpHybla" "TcpHighSpeed" "TcpVegas" "TcpScalable" "TcpHtcp" "TcpVeno" "TcpBic" "TcpYeah" "TcpIllinois" "TcpWestwood" "TcpWestwoodPlus");
echo "Create Stat File..."

echo "nSpokes Protocol TotalRx Throughput">result.txt

echo "Working on nSpokes=1..."
for((i = 0; i < 12; i++))
do
	echo -e "1 ${Type[i]} \c">>result.txt
	cat ${Type[i]}1.txt | awk 'END{printf $2 >> "result.txt"}'
	echo -e " \c">>result.txt
	cat ${Type[i]}1.txt | awk 'END{print $4 >> "result.txt"}'
done
echo "nSpokes=1 finished..."

echo "Working on nSpokes=4..."
for((i = 0; i < 12; i++))
do
	echo -e "4 ${Type[i]} \c">>result.txt
	cat ${Type[i]}4.txt | awk 'END{printf $2 >> "result.txt"}'
	echo -e " \c">>result.txt
	cat ${Type[i]}4.txt | awk 'END{print $4 >> "result.txt"}'
done
echo "nSpokes=4 finished..."

echo "Working on nSpokes=8..."
for((i = 0; i < 12; i++))
do
	echo -e "8 ${Type[i]} \c">>result.txt
	cat ${Type[i]}8.txt | awk 'END{printf $2 >> "result.txt"}'
	echo -e " \c">>result.txt
	cat ${Type[i]}8.txt | awk 'END{print $4 >> "result.txt"}'
done
echo "nSpokes=8 finished..."

echo "Working on nSpokes=16..."
for((i = 0; i < 12; i++))
do
	echo -e "16 ${Type[i]} \c">>result.txt
	cat ${Type[i]}16.txt | awk 'END{printf $2 >> "result.txt"}'
	echo -e " \c">>result.txt
	cat ${Type[i]}16.txt | awk 'END{print $4 >> "result.txt"}'
done
echo "nSpokes=16 finished..."

echo "Working on nSpokes=32..."
for((i = 0; i < 12; i++))
do
	echo -e "32 ${Type[i]} \c">>result.txt
	cat ${Type[i]}32.txt | awk 'END{printf $2 >> "result.txt"}'
	echo -e " \c">>result.txt
	cat ${Type[i]}32.txt | awk 'END{print $4 >> "result.txt"}'
done
echo "nSpokes=32 finished..."

echo "Finish!"

