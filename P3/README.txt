Group Member:
Zeyu Chen, chezy@gatech.edu
Ruijia Wang, rwang@gatech.edu
Minghe Zhang, minghe_zhang@gatech.edu

What is in the folder:
code:
	p3.cc                    main
	p2pCampusHelper.h        Basic Topo generator
	p2pCampusHelper.cc
	UdpWorm.h                UdpWorm class
	UdpWorm.cc
Data:
	Graph1.m                 Plot graph1
	Graph2.m                 Plot graph2
	getdata.pl               Extract data for graph3
	stats.xlsx               Store graph1 and graph2 data
	p4_Local_10ms_null.txt   Store graph3 data
	p4_Seq_10ms_null.txt     Store graph3 data
	p4_Uniform_10ms_null.txt Store graph3 data
Graph:
	Graph 1.jpg
	Graph 2.jpg
	Graph 3.png
	
How to run:
Put the p3 folder in scratch. Then run command:
mpirun -np x ./waf --run "p3 --ScanRate=int1 --ScanPattern=str1 --BackboneDelay=str2 --SyncType=str3"

Available number for x:1, 2, 4.
ScanRate is int type.
The other three are all string type. If you use BackboneDelay, please type "xms". i.e. 5ms

Code:
The main file is p3.cc. 
p2pCampusHelper is used to construct the basic topology. Also, the validity of the node is determined by it.
UdpWorm is a child class inherited Application, based on udp protocol.

The method is to install UdpWorm application to every valid nodes. 
We choose 10.1.1.4 as the start node.
When an inactive node receive the worm packet, it will be infected and start sending worm to others.
The routing is based on Nix Vector. And if a packet is sent to the not existing node, it will be dropped at the nearest valid node.


Graph:
The program is tested on the ubuntu 16.04. CPU: 1 core i7-7700HQ @2.80HZ Memory: 3.9GiB
The tool we used to extract data is perl.
The tool we used to plot is MATLAB and gnuplot.

1.WallClock Time - MPI instances
The graph is based on the lookahead time = 5ms, SyncType = null.
Here we only consider the execution time of program.
Show the average time and the longest time of the multi processes.

2.WallClock Time - Lookahead Time
The graph is based on 4 MPI instances.
Here we only consider the execution time of program.
Show the average time of the multi processes.

3.Infection Rate
In this part, we only pick one instance to show the infection rate of differfnt scan pattern.
The graph is based on the configuration: MPI intstances = 4, ScanRate = 10ms, SyncType = null.