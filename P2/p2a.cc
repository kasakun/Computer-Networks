/** Network topology
 *
 *    100Mb/s, 1ms                           
 * n0--------------|                    
 *                 |     45Mb/s, 2ms            
 *                 nG-----------------n4
 *    100Mb/s, 4ms |                    
 * n1--------------|                    
 *                 |
 *                 |
 *    100Mb/s, 8ms |
 * n2--------------|
 *                 |
 *                 |
 *    100Mb/s, 5ms |
 * n3--------------|
 *
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("p2a");

uint32_t checkTimes;
double avgQueueSize;

// The times
double sink_start_time;
double sink_stop_time;
double client_start_time;
double client_stop_time;
uint32_t nenqueue = 0;
uint32_t ndequeue = 0;
uint32_t ndropatend = 0;
SequenceNumber32 temp;

NodeContainer n0nG;
NodeContainer n1nG;
NodeContainer n2nG;
NodeContainer n3nG;
NodeContainer nGn4;

Ipv4InterfaceContainer i0iG;
Ipv4InterfaceContainer i1iG;
Ipv4InterfaceContainer i2iG;
Ipv4InterfaceContainer i3iG;
Ipv4InterfaceContainer iGi4;


std::stringstream filePlotQueue;
std::stringstream filePlotQueueAvg;
std::stringstream enqueue;
std::stringstream drop;

void EnqueueAtRed(Ptr<const QueueItem> item) {
    TcpHeader tcp;
    Ipv4Header ip;
    PppHeader ppp;
    Ptr<Packet> pkt = item->GetPacket();
    Ptr<Packet> tcopy = pkt->Copy();
    tcopy->PeekHeader(tcp);
    tcopy->RemoveHeader(ppp);
    tcopy->PeekHeader(ip);

    nenqueue++;
    std::ofstream Enqueue (enqueue.str ().c_str (), std::ios::out|std::ios::app);
    Enqueue << Simulator::Now ().GetSeconds () << " " << tcp.GetSequenceNumber() << "  " << tcp.GetSequenceNumber() - temp << " "<<std::endl;
    temp =  tcp.GetSequenceNumber(); 
    Enqueue.close ();
 
}
 
void DequeueAtRed(Ptr<const QueueItem> item) {
    TcpHeader tcp;
    Ptr<Packet> pkt = item->GetPacket();
    pkt->PeekHeader(tcp);
    
    ndequeue++;
 
}
 
void DroppedAtRed(Ptr<const QueueItem> item) {
    TcpHeader tcp;
    Ptr<Packet> pkt = item->GetPacket();
    pkt->PeekHeader(tcp);
    //tcp.Print(std::cout);
    //std::cout<< tcp.GetSequenceNumber() << std::endl;
    ndropatend++;
    std::ofstream Drop (drop.str ().c_str (), std::ios::out|std::ios::app);
    Drop << Simulator::Now ().GetSeconds () << " " << tcp.GetSequenceNumber() << "  " << tcp.GetSequenceNumber() - temp << " "<<std::endl;
    //pkt->Print(std::cout);
    temp =  tcp.GetSequenceNumber(); 
    Drop.close ();
    //
}

void
CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = StaticCast<RedQueueDisc> (queue)->GetQueueSize ();

  avgQueueSize += qSize;
  checkTimes++;

  // check queue size every 1/1000 of a second
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
  ApplicationContainer sinkApp = sinkHelper.Install (nGn4.Get (1));
  sinkApp.Start (Seconds (sink_start_time));
  sinkApp.Stop (Seconds (sink_stop_time));

  // Connection one

  AddressValue remoteAddress
    (InetSocketAddress (iGi4.GetAddress (1), port));

  // 0
  OnOffHelper clientHelper0 ("ns3::TcpSocketFactory", Address ());
  clientHelper0.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.5|Max=0.6]"));
  clientHelper0.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper0.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("100Mbps")));
  clientHelper0.SetAttribute 
    ("PacketSize", UintegerValue (1000));
  
  ApplicationContainer clientApps0;
  clientHelper0.SetAttribute ("Remote", remoteAddress);
  clientApps0.Add (clientHelper0.Install (n0nG.Get (0)));
  clientApps0.Start (Seconds (client_start_time));
  clientApps0.Stop (Seconds (client_stop_time));

  // 1
  OnOffHelper clientHelper1 ("ns3::TcpSocketFactory", Address ());
  clientHelper1.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.5|Max=0.6]"));
  clientHelper1.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper1.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("100Mbps")));
  clientHelper1.SetAttribute 
    ("PacketSize", UintegerValue (1000));

  ApplicationContainer clientApps1;
  clientHelper1.SetAttribute ("Remote", remoteAddress);
  clientApps1.Add (clientHelper1.Install (n1nG.Get (0)));
  clientApps1.Start (Seconds (client_start_time + 0.2));
  clientApps1.Stop (Seconds (client_stop_time));

  // 2
  OnOffHelper clientHelper2 ("ns3::TcpSocketFactory", Address ());
  clientHelper2.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.5|Max=0.6]"));
  clientHelper2.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper2.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("100Mbps")));
  clientHelper2.SetAttribute 
    ("PacketSize", UintegerValue (1000));

  ApplicationContainer clientApps2;
  clientHelper2.SetAttribute ("Remote", remoteAddress);
  clientApps2.Add (clientHelper2.Install (n2nG.Get (0)));
  clientApps2.Start (Seconds (client_start_time + 0.4));
  clientApps2.Stop (Seconds (client_stop_time));

  // 3
  OnOffHelper clientHelper3 ("ns3::TcpSocketFactory", Address ());
  clientHelper3.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.5|Max=0.6]"));
  clientHelper3.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.1]"));
  clientHelper3.SetAttribute 
    ("DataRate", DataRateValue (DataRate ("100Mbps")));
  clientHelper3.SetAttribute 
    ("PacketSize", UintegerValue (1000));

  ApplicationContainer clientApps3;
  clientHelper3.SetAttribute ("Remote", remoteAddress);
  clientApps3.Add (clientHelper3.Install (n3nG.Get (0)));
  clientApps3.Start (Seconds (client_start_time + 0.6));
  clientApps3.Stop (Seconds (client_stop_time));
  
}

int
main (int argc, char *argv[])
{
  LogComponentEnable ("RedQueueDisc", LOG_LEVEL_INFO);

  std::string redLinkDataRate = "45Mbps";
  std::string redLinkDelay = "2ms";
  std::string pathOut = ".";

  bool writeForPlot = true;
  bool writePcap = true;
  bool flowMonitor = false;

  bool printRedStats = true;

  //Time
  sink_start_time = 0.0;
  sink_stop_time = 1.0;
  client_start_time = 0.0;
  client_stop_time = 1.0;


  NS_LOG_INFO ("Create nodes");
  NodeContainer c;
  c.Create (6);
  Names::Add ( "N0", c.Get (0));
  Names::Add ( "N1", c.Get (1));
  Names::Add ( "N2", c.Get (2));
  Names::Add ( "N3", c.Get (3));
  Names::Add ( "N4", c.Get (4));
  Names::Add ( "NG", c.Get (5));
  n0nG = NodeContainer (c.Get (0), c.Get (5));
  n1nG = NodeContainer (c.Get (1), c.Get (5));
  n2nG = NodeContainer (c.Get (2), c.Get (5));
  n3nG = NodeContainer (c.Get (3), c.Get (5));
  nGn4 = NodeContainer (c.Get (5), c.Get (4));

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
  // 42 = headers size
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000 - 42));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));
  Config::SetDefault ("ns3::TcpSocketBase::MaxWindowSize", UintegerValue (50*1000));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (false));

  uint32_t meanPktSize = 1000;

  // RED params
  NS_LOG_INFO ("Set RED params");
  
  Config::SetDefault ("ns3::RedQueueDisc::Mode", StringValue ("QUEUE_MODE_PACKETS"));
  Config::SetDefault ("ns3::RedQueueDisc::MeanPktSize", UintegerValue (meanPktSize));
  Config::SetDefault ("ns3::RedQueueDisc::Wait", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::Gentle", BooleanValue (true));
  Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue (0.002));
  Config::SetDefault ("ns3::RedQueueDisc::MinTh", DoubleValue (5));
  Config::SetDefault ("ns3::RedQueueDisc::MaxTh", DoubleValue (15));
  Config::SetDefault ("ns3::RedQueueDisc::QueueLimit", UintegerValue (120));
  
  SeedManager::SetSeed(4);
  SeedManager::SetRun(1);

  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  internet.Install (c);

  TrafficControlHelper tchPfifo; 
  uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");  //uint16_t handle = 
  tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));
  

  TrafficControlHelper tchRed;
  tchRed.SetRootQueueDisc ("ns3::RedQueueDisc", "LinkBandwidth", StringValue (redLinkDataRate),
                           "LinkDelay", StringValue (redLinkDelay));
  Config::SetDefault ("ns3::Queue::MaxPackets", UintegerValue (8));

  NS_LOG_INFO ("Create channels");
  PointToPointHelper p2p;

  //p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue (redLinkDataRate));
  p2p.SetChannelAttribute ("Delay", StringValue (redLinkDelay));
  //p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(4)); //Change
  NetDeviceContainer devnGn4 = p2p.Install (nGn4);
  // only backbone link has RED queue disc
  QueueDiscContainer queueDiscs = tchRed.Install (devnGn4);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(8));
  NetDeviceContainer devn0nG = p2p.Install (n0nG);
  tchPfifo.Install (devn0nG);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("4ms"));
  //p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(1));
  NetDeviceContainer devn1nG = p2p.Install (n1nG);
  tchPfifo.Install (devn1nG);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("8ms"));
  //p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(8));
  NetDeviceContainer devn2nG = p2p.Install (n2nG);
  tchPfifo.Install (devn2nG);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));
  //p2p.SetQueue("ns3::DropTailQueue",  "MaxPackets",UintegerValue(8));
  NetDeviceContainer devn3nG = p2p.Install (n3nG);
  tchPfifo.Install (devn3nG);

  

  NS_LOG_INFO ("Assign IP Addresses");
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  i0iG = ipv4.Assign (devn0nG);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  i1iG = ipv4.Assign (devn1nG);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  i2iG = ipv4.Assign (devn2nG);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  i3iG = ipv4.Assign (devn3nG);

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  iGi4 = ipv4.Assign (devnGn4);


  // Set up the routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  
    
  // Get Gate
  Ptr<QueueDisc> queue = queueDiscs.Get (0);


  queue->TraceConnectWithoutContext("Enqueue", MakeCallback(&EnqueueAtRed));
  queue->TraceConnectWithoutContext("Dequeue", MakeCallback(&DequeueAtRed));
  queue->TraceConnectWithoutContext("Drop", MakeCallback(&DroppedAtRed));
  //
  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll(ascii.CreateFileStream("p2a.tr"));

  BuildAppsTest ();

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

  if (writeForPlot)
    {
      filePlotQueue << pathOut << "/" << "red-queue.plotme";
      filePlotQueueAvg << pathOut << "/" << "red-queue_avg.plotme";

      enqueue << pathOut << "/" << "enqueue.plotme";
      drop << pathOut << "/" << "drop.plotme";

      remove (filePlotQueue.str ().c_str ());
      remove (filePlotQueueAvg.str ().c_str ());
      remove (enqueue.str ().c_str ());
      remove (drop.str ().c_str ());
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
      std::cout << "*** RED stats from Node G queue ***" << std::endl;
      std::cout << "\t " << st.unforcedDrop << " drops due prob mark" << std::endl;
      std::cout << "\t " << st.forcedDrop << " drops due hard mark" << std::endl;
      std::cout << "\t " << st.qLimDrop << " drops due queue full" << std::endl;

      st = StaticCast<RedQueueDisc> (queueDiscs.Get (1))->GetStats ();
      std::cout << "*** RED stats from Node 4 queue ***" << std::endl;
      std::cout << "\t " << st.unforcedDrop << " drops due prob mark" << std::endl;
      std::cout << "\t " << st.forcedDrop << " drops due hard mark" << std::endl;
      std::cout << "\t " << st.qLimDrop << " drops due queue full" << std::endl;

      std::cout << "*** STATS of queue ***" << std::endl;
      std::cout << "\t " << nenqueue << " enqueue" << std::endl;
      std::cout << "\t " << ndequeue << " dequeue" << std::endl;
      std::cout << "\t " << ndropatend << " dropatend" << std::endl;

    }

  Simulator::Destroy ();

  return 0;
}
