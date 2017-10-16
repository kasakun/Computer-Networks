/** Network topology
 *
 *   100Mb/s, 0.5ms                        100Mb/s, 0.5ms
 * n0--------------|                    |---------------n4
 *                 |    45Mbps/s, 2ms   |
 *                nG1------------------nG2
 *    100Mb/s, 1ms |                    |  100Mb/s, 1ms
 * n1--------------|                    |---------------n5
 *                 |                    |
 *    100Mb/s, 3ms |                    |  100Mb/s, 5ms
 * n2--------------|                    |---------------n6
 *                 |                    |
 *                 |                    |
 *    100Mb/s, 5ms |                    |  100Mb/s, 2ms
 * n3--------------|                    |---------------n7
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("p2c");

uint32_t checkTimes;
double avgQueueSize;

uint32_t packetSize = 1000 -42;
// The times
double global_start_time;
double global_stop_time;
double sink_start_time;
double sink_stop_time;
double client_start_time;
double client_stop_time;

NodeContainer n0nG1;
NodeContainer n1nG1;
NodeContainer n2nG1;
NodeContainer n3nG1;

NodeContainer nG1nG2;

NodeContainer nG2n4;
NodeContainer nG2n5;
NodeContainer nG2n6;
NodeContainer nG2n7;

Ipv4InterfaceContainer i0iG1;
Ipv4InterfaceContainer i1iG1;
Ipv4InterfaceContainer i2iG1;
Ipv4InterfaceContainer i3iG1;

Ipv4InterfaceContainer iG1iG2;

Ipv4InterfaceContainer iG2i4;
Ipv4InterfaceContainer iG2i5;
Ipv4InterfaceContainer iG2i6;
Ipv4InterfaceContainer iG2i7;

std::stringstream filePlotQueue;
std::stringstream filePlotQueueAvg;

void
CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = StaticCast<RedQueueDisc> (queue)->GetQueueSize ();

  avgQueueSize += qSize;
  checkTimes++;

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize, queue);

  std::ofstream fPlotQueue (filePlotQueue.str ().c_str (), std::ios::out|std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();

  std::ofstream fPlotQueueAvg (filePlotQueueAvg.str ().c_str (), std::ios::out|std::ios::app);
  fPlotQueueAvg << Simulator::Now ().GetSeconds () << " " << avgQueueSize / checkTimes << std::endl;
  fPlotQueueAvg.close ();
}

void
BuildAppsTest ()
{
  // SINK is in the right side
  uint16_t port = 50000;
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp0 = sinkHelper.Install (n0nG1.Get (0));
  sinkApp0.Start (Seconds (sink_start_time));
  sinkApp0.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp1 = sinkHelper.Install (n1nG1.Get (0));
  sinkApp1.Start (Seconds (sink_start_time));
  sinkApp1.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp2 = sinkHelper.Install (n2nG1.Get (0));
  sinkApp2.Start (Seconds (sink_start_time));
  sinkApp2.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp3 = sinkHelper.Install (n3nG1.Get (0));
  sinkApp3.Start (Seconds (sink_start_time));
  sinkApp3.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp4 = sinkHelper.Install (nG2n4.Get (1));
  sinkApp4.Start (Seconds (sink_start_time));
  sinkApp4.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp5 = sinkHelper.Install (nG2n5.Get (1));
  sinkApp5.Start (Seconds (sink_start_time));
  sinkApp5.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp6 = sinkHelper.Install (nG2n6.Get (1));
  sinkApp6.Start (Seconds (sink_start_time));
  sinkApp6.Stop (Seconds (sink_stop_time));

  ApplicationContainer sinkApp7 = sinkHelper.Install (nG2n7.Get (1));
  sinkApp7.Start (Seconds (sink_start_time));
  sinkApp7.Stop (Seconds (sink_stop_time));

  //////////////////////////////////////////////////////////////////////////////////////////////////

  
  OnOffHelper clientHelper1 ("ns3::TcpSocketFactory", Address ());
  clientHelper1.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper1.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.5]"));
  clientHelper1.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("100Mb/s"))); //tcp
  clientHelper1.SetAttribute 
    ("PacketSize", UintegerValue (1000));

  OnOffHelper clientHelper2 ("ns3::TcpSocketFactory", Address ());
  clientHelper2.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper2.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.5]"));
  clientHelper2.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("1Mb/s"))); //telnet
  clientHelper2.SetAttribute 
    ("PacketSize", UintegerValue (40));
  
  ApplicationContainer clientApps1;
  ApplicationContainer clientApps2;


  clientApps1.Start (Seconds (client_start_time));
  clientApps1.Stop (Seconds (client_stop_time));

  clientApps2.Start (Seconds (client_start_time));
  clientApps2.Stop (Seconds (client_stop_time));

  //Node 0
  // C1
  AddressValue remoteAddress
    (InetSocketAddress (iG2i4.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n0nG1.Get (0)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (iG2i5.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n0nG1.Get (0)));

  //C3

  remoteAddress.Set
    (InetSocketAddress (iG2i6.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n0nG1.Get (0)));

  //C4  Telnet
  clientApps1.Start (Seconds (client_start_time + 0.1 ));
  clientApps1.Stop (Seconds (client_start_time + 0.7));
  remoteAddress.Set
    (InetSocketAddress (iG2i7.GetAddress (1), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n0nG1.Get (0)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n0nG1.Get (0)));

  // Node 1

  // C1
  remoteAddress.Set
    (InetSocketAddress (iG2i4.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n1nG1.Get (0)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (iG2i5.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n1nG1.Get (0)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (iG2i6.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n1nG1.Get (0)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (iG2i7.GetAddress (1), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n1nG1.Get (0)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n1nG1.Get (0)));

  // Node 2
  // C1
  remoteAddress.Set
    (InetSocketAddress (iG2i4.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n2nG1.Get (0)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (iG2i5.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n2nG1.Get (0)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (iG2i6.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n2nG1.Get (0)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (iG2i7.GetAddress (1), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n2nG1.Get (0)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n2nG1.Get (0)));

  // Node 3
  // C1
  remoteAddress.Set
    (InetSocketAddress (iG2i4.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n3nG1.Get (0)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (iG2i5.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n3nG1.Get (0)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (iG2i6.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n3nG1.Get (0)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (iG2i7.GetAddress (1), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n3nG1.Get (0)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n3nG1.Get (0)));

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //G1 -- G2
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (1), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG1nG2.Get (0)));
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  // Node 4
  // C1
  remoteAddress.Set
    (InetSocketAddress (i0iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n4.Get (1)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (i1iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n4.Get (1)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (i2iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n4.Get (1)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (i3iG1.GetAddress (0), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (nG2n4.Get (1)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n4.Get (1)));

  // Node 5
  // C1
  remoteAddress.Set
    (InetSocketAddress (i0iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n5.Get (1)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (i1iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n5.Get (1)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (i2iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n5.Get (1)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (i3iG1.GetAddress (0), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (nG2n5.Get (1)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n5.Get (1)));


  // Node 6
  // C1
  remoteAddress.Set
    (InetSocketAddress (i0iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n6.Get (1)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (i1iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n6.Get (1)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (i2iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n6.Get (1)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (i3iG1.GetAddress (0), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (nG2n6.Get (1)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n6.Get (1)));

  // Node 7
  // C1
  remoteAddress.Set
    (InetSocketAddress (i0iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n7.Get (1)));

  //C2
  remoteAddress.Set
    (InetSocketAddress (i1iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n7.Get (1)));

  //C3
  remoteAddress.Set
    (InetSocketAddress (i2iG1.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n7.Get (1)));

  //C4  Telnet
  remoteAddress.Set
    (InetSocketAddress (i3iG1.GetAddress (0), port));
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (nG2n7.Get (1)));

  //C5
  remoteAddress.Set
    (InetSocketAddress (iG1iG2.GetAddress (0), port));
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (nG2n7.Get (1)));
}

int
main (int argc, char *argv[])
{
  LogComponentEnable ("RedQueueDisc", LOG_LEVEL_INFO);

  uint32_t redTest;
  std::string redLinkDataRate = "45Mbps";
  std::string redLinkDelay = "2ms";

  std::string pathOut;
  bool writeForPlot = false;
  bool writePcap = false;
  bool flowMonitor = false;

  bool printRedStats = true;

  RngSeedManager::SetSeed(4);
  RngSeedManager::SetRun(1);

  Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
 
  global_start_time = 0.0;
  global_stop_time = 1.0; 
  sink_start_time = 0.0;
  sink_stop_time = 1.0;
  client_start_time = x->GetValue(0.0, 0.5);
  client_stop_time = 1.0;

  // Configuration and command line parameter parsing
  redTest = 1;
  // Will only save in the directory if enable opts below
  pathOut = "."; // Current directory
  CommandLine cmd;
  cmd.AddValue ("testNumber", "Run test 1, 3, 4 or 5", redTest);
  cmd.AddValue ("pathOut", "Path to save results from --writeForPlot/--writePcap/--writeFlowMonitor", pathOut);
  cmd.AddValue ("writeForPlot", "<0/1> to write results for plot (gnuplot)", writeForPlot);
  cmd.AddValue ("writePcap", "<0/1> to write results in pcapfile", writePcap);
  cmd.AddValue ("writeFlowMonitor", "<0/1> to enable Flow Monitor and write their results", flowMonitor);

  cmd.Parse (argc, argv);
  if ( (redTest != 1) && (redTest != 3) && (redTest != 4) && (redTest != 5) )
    {
      NS_ABORT_MSG ("Invalid test number. Supported tests are 1, 3, 4 or 5");
    }

  NS_LOG_INFO ("Create nodes");
  NodeContainer c;
  c.Create (10);
  Names::Add ( "N0", c.Get (0));
  Names::Add ( "N1", c.Get (1));
  Names::Add ( "N2", c.Get (2));
  Names::Add ( "N3", c.Get (3));
  Names::Add ( "NG1", c.Get (4));
  Names::Add ( "NG2", c.Get (5));
  Names::Add ( "N4", c.Get (6));
  Names::Add ( "N5", c.Get (7));
  Names::Add ( "N6", c.Get (8));
  Names::Add ( "N7", c.Get (9));

  n0nG1 = NodeContainer (c.Get (0), c.Get (4));
  n1nG1 = NodeContainer (c.Get (1), c.Get (4));
  n2nG1 = NodeContainer (c.Get (2), c.Get (4));
  n3nG1 = NodeContainer (c.Get (3), c.Get (4));
  nG1nG2 = NodeContainer (c.Get (4), c.Get (5));
  nG2n4 = NodeContainer (c.Get (5), c.Get (6));
  nG2n5 = NodeContainer (c.Get (5), c.Get (7));
  nG2n6 = NodeContainer (c.Get (5), c.Get (8));
  nG2n7 = NodeContainer (c.Get (5), c.Get (9));

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
  // 42 = headers size
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000 - 42));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  uint32_t meanPktSize = 500;


  // RED params
  NS_LOG_INFO ("Set RED params");
  Config::SetDefault ("ns3::RedQueueDisc::Mode", StringValue ("QUEUE_MODE_PACKETS"));
  Config::SetDefault ("ns3::RedQueueDisc::MeanPktSize", UintegerValue (meanPktSize));
  Config::SetDefault ("ns3::RedQueueDisc::Wait", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::Gentle", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue (0.002));
  Config::SetDefault ("ns3::RedQueueDisc::MinTh", DoubleValue (5));
  Config::SetDefault ("ns3::RedQueueDisc::MaxTh", DoubleValue (15));
  Config::SetDefault ("ns3::RedQueueDisc::QueueLimit", UintegerValue (100));
  Config::SetDefault ("ns3::Queue::MaxPackets", UintegerValue (1));


  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  internet.Install (c);

  TrafficControlHelper tchPfifo;
  uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));

  TrafficControlHelper tchRed;
  tchRed.SetRootQueueDisc ("ns3::RedQueueDisc", "LinkBandwidth", StringValue (redLinkDataRate),
                           "LinkDelay", StringValue (redLinkDelay));

  NS_LOG_INFO ("Create channels");
  // RED Channel
  PointToPointHelper p2p;
   p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("45Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devnG1nG2 = p2p.Install (nG1nG2);
  // only backbone link has RED queue disc
  QueueDiscContainer queueDiscs = tchRed.Install (devnG1nG2);
  ///////////////////////////////////////////////////////////////////////////
  // DropTail Left
  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0.5ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devn0nG1 = p2p.Install (n0nG1);
  tchPfifo.Install (devn0nG1);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devn1nG1 = p2p.Install (n1nG1);
  tchPfifo.Install (devn1nG1);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("3ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devn2nG1 = p2p.Install (n2nG1);
  tchPfifo.Install (devn2nG1);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devn3nG1 = p2p.Install (n3nG1);
  tchPfifo.Install (devn3nG1);
  ////////////////////////////////////////////////////////////////
  // DropTail Right
  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0.5ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devnG2n4 = p2p.Install (nG2n4);
  tchPfifo.Install (devnG2n4);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devnG2n5 = p2p.Install (nG2n5);
  tchPfifo.Install (devnG2n5);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devnG2n6 = p2p.Install (nG2n6);
  tchPfifo.Install (devnG2n6);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(16));
  NetDeviceContainer devnG2n7 = p2p.Install (nG2n7);
  tchPfifo.Install (devnG2n7);



  NS_LOG_INFO ("Assign IP Addresses");
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  i0iG1 = ipv4.Assign (devn0nG1);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  i1iG1 = ipv4.Assign (devn1nG1);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  i2iG1 = ipv4.Assign (devn2nG1);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  i3iG1 = ipv4.Assign (devn3nG1);

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  iG1iG2 = ipv4.Assign (devnG1nG2);

  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  iG2i4 = ipv4.Assign (devnG2n4);

  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  iG2i5 = ipv4.Assign (devnG2n5);

  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  iG2i6 = ipv4.Assign (devnG2n6);

  ipv4.SetBase ("10.1.9.0", "255.255.255.0");
  iG2i7 = ipv4.Assign (devnG2n7);

  // Set up the routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  
  // Get Gate A
  //Ptr<QueueDisc> queue = queueDiscs.Get (0);
  // Get Gate B
  Ptr<QueueDisc> queue = queueDiscs.Get (1);

  StaticCast<RedQueueDisc> (queue)->SetMode (Queue::QUEUE_MODE_PACKETS);
  StaticCast<RedQueueDisc> (queue)->SetTh (5, 15);
  StaticCast<RedQueueDisc> (queue)->SetQueueLimit (1000);

  BuildAppsTest ();
  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll(ascii.CreateFileStream("p2c.tr"));

  if (writePcap)
    {
      PointToPointHelper ptp;
      std::stringstream stmp;
      stmp << pathOut << "/red";
      ptp.EnablePcapAll (stmp.str ().c_str ());
    }

  Ptr<FlowMonitor> flowmon;
  if (flowMonitor)
    {
      FlowMonitorHelper flowmonHelper;
      flowmon = flowmonHelper.InstallAll ();
    }

  if (true)
    {
      filePlotQueue << pathOut << "/" << "red-queue.plotme";
      filePlotQueueAvg << pathOut << "/" << "red-queue_avg.plotme";

      remove (filePlotQueue.str ().c_str ());
      remove (filePlotQueueAvg.str ().c_str ());
      Ptr<QueueDisc> queue = queueDiscs.Get (0);
      Simulator::ScheduleNow (&CheckQueueSize, queue);
    }

  Simulator::Stop (Seconds (sink_stop_time));
  Simulator::Run ();

  if (flowMonitor)
    {
      std::stringstream stmp;
      stmp << pathOut << "/red.flowmon";

      flowmon->SerializeToXmlFile (stmp.str ().c_str (), false, false);
    }

  if (printRedStats)
    {
      RedQueueDisc::Stats st = StaticCast<RedQueueDisc> (queueDiscs.Get (0))->GetStats ();
      std::cout << "*** RED stats from Node 2 queue ***" << std::endl;
      std::cout << "\t " << st.unforcedDrop << " drops due prob mark" << std::endl;
      std::cout << "\t " << st.forcedDrop << " drops due hard mark" << std::endl;
      std::cout << "\t " << st.qLimDrop << " drops due queue full" << std::endl;

      st = StaticCast<RedQueueDisc> (queueDiscs.Get (1))->GetStats ();
      std::cout << "*** RED stats from Node 3 queue ***" << std::endl;
      std::cout << "\t " << st.unforcedDrop << " drops due prob mark" << std::endl;
      std::cout << "\t " << st.forcedDrop << " drops due hard mark" << std::endl;
      std::cout << "\t " << st.qLimDrop << " drops due queue full" << std::endl;
    }

  Simulator::Destroy ();

  return 0;
}
