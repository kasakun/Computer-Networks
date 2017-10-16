Project 2 RED

1.p2aGraph1 is the queue-time.
2.p2aGraph2 is flows(mod)-time. The "X" means the dropped packets and "o" means the retransmission packets.
3.p2cGraph1A is queue-time for Gate A.
3.p2cGraph1B is queue-time for Gate B.
3.p2cGraph1A is flows-time. There are 41 flows in the graph match the 41 connections.

The .tr file is used to trace the enqueue, dequeue, drop and flow information. Use perl(getdataa.perl and getdatac.perl) to grab the information from the .tr files and then use MATLAB GnuPlot to plot and mark the Graph.