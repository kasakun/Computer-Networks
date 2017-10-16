#!/bin/bash

echo "Start Simulation..."
declare -a Type=("TcpNewReno" "TcpHybla" "TcpHighSpeed" "TcpVegas" "TcpScalable" "TcpHtcp" "TcpVeno" "TcpBic" "TcpYeah" "TcpIllinois" "TcpWestwood" "TcpWestwoodPlus");
echo "Create Stat File..."

echo "=======Working on nSpokes=1======="
for((i = 0; i < 12; i++))
do
	echo "Simulating ${Type[i]}..."
	./waf --run "p1 --nSpokes=1 --transport_prot=${Type[i]}">./result/${Type[i]}1.txt	
	echo "${Type[i]} finished..."
done
echo "=======nSpokes=1 finished========="

echo "=======Working on nSpokes=4======="
for((i = 0; i < 12; i++))
do
	echo "Simulating ${Type[i]}..."
	./waf --run "p1 --nSpokes=4 --transport_prot=${Type[i]}">./result/${Type[i]}4.txt	
	echo "${Type[i]} finished..."
done
echo "=======nSpokes=4 finished========="

echo "=======Working on nSpokes=8======="
for((i = 0; i < 12; i++))
do
	echo "Simulating ${Type[i]}..."
	./waf --run "p1 --nSpokes=8 --transport_prot=${Type[i]}">./result/${Type[i]}8.txt	
	echo "${Type[i]} finished..."
done
echo "=======nSpokes=8 finished========="

echo "=======Working on nSpokes=16======"
for((i = 0; i < 12; i++))
do
	echo "Simulating ${Type[i]}..."
	./waf --run "p1 --nSpokes=16 --transport_prot=${Type[i]}">./result/${Type[i]}16.txt	
	echo "${Type[i]} finished..."
done
echo "=======nSpokes=16 finished========"

echo "Working on nSpokes=32..."
for((i = 0; i < 12; i++))
do
	echo "Simulating ${Type[i]}..."
	./waf --run "p1 --nSpokes=32 --transport_prot=${Type[i]}">./result/${Type[i]}32.txt	
	echo "${Type[i]} finished..."
done
echo "=======nSpokes=32 finished========"

echo "Finish!"
