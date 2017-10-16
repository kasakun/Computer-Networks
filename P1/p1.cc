#include <iostream>
#include <fstream>
#include <string>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

// Topology
//
//        n2 n3 n4            m2 m3 m4       .
//         \ | /_______________\ | /         .
//          \|//             _ \\|/          .
//     n1--- n0---n5       m1--- m0---m5     .
//          /|\                 /|\          .
//         / | \               / | \         .
//        n8 n7 n6            m8 m7 m6       .
//
using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("P1");
int main (int argc, char *argv[])
{
  uint32_t nSpokes = 8;
  uint32_t maxBytes = 0;
  double endTime = 60.0;
  uint16_t port = 5000;
  std::string transport_prot = "TcpWestwood";

  CommandLine cmd;
  cmd.AddValue ("nSpokes", "Number of spokes", nSpokes); 
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno," 
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno,"
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus ", transport_prot);

  cmd.Parse (argc,argv);

  SeedManager::SetSeed(1);

  // Select TCP variant
  if (transport_prot.compare ("TcpNewReno") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHybla") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHybla::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHighSpeed") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHighSpeed::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpVegas") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVegas::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpScalable") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpScalable::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHtcp") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHtcp::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpVeno") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVeno::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpBic") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpBic::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpYeah") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpYeah::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpIllinois") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpIllinois::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpWestwood") == 0){ // the default protocol type in ns3::TcpWestwood is WESTWOOD
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      Config::SetDefault ("ns3::TcpWestwood::FilterType", EnumValue (TcpWestwood::TUSTIN));
    }
  else if (transport_prot.compare ("TcpWestwoodPlus") == 0){
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
      Config::SetDefault ("ns3::TcpWestwood::FilterType", EnumValue (TcpWestwood::TUSTIN));
    }
  else{
      NS_LOG_DEBUG ("Invalid TCP version");
      exit (1);
    }

  Time::SetResolution (Time::NS);

  
  PointToPointHelper hubTohub;
  hubTohub.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  hubTohub.SetChannelAttribute ("Delay", StringValue ("20ms"));


  PointToPointHelper hubTostar;
  hubTostar.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  hubTostar.SetChannelAttribute ("Delay", StringValue ("10ms"));

  PointToPointStarHelper star1 (nSpokes, hubTostar);
  PointToPointStarHelper star2 (nSpokes, hubTostar);
  
  NodeContainer p2pNodes;
  p2pNodes.Add(star1.GetHub());
  p2pNodes.Add(star2.GetHub());

  NetDeviceContainer devices;
  devices = hubTohub.Install (p2pNodes);

  InternetStackHelper stack;
  star1.InstallStack(stack);
  star2.InstallStack(stack);

  Ipv4AddressHelper address;
  
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (devices);

  address.SetBase ("10.2.1.0", "255.255.255.0");
  star1.AssignIpv4Addresses (address);

  address.SetBase ("10.3.1.0", "255.255.255.0");
  star2.AssignIpv4Addresses (address);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  ApplicationContainer sourceApps;
  ApplicationContainer sinkApps;

  BulkSendHelper source ("ns3::TcpSocketFactory", Address ());
  // Set the amount of data to send in bytes.  Zero is unlimited.
  //source.SetAttribute ("SendSize", UintegerValue (1024));
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  
  for (uint32_t i = 0; i < nSpokes ; ++i) {
    AddressValue remoteAddress (InetSocketAddress (star2.GetSpokeIpv4Address (i), port));
    source.SetAttribute ("Remote", remoteAddress);
    sourceApps.Add (source.Install (star1.GetSpokeNode (i)));
    
    // Create a PacketSinkApplication and install it on node 1

    sinkApps.Add (sink.Install (star2.GetSpokeNode (i)));
    
  }

  sinkApps.Start (Seconds (1.0));
  sinkApps.Stop (Seconds (60.0));
  sourceApps.Start (Seconds (2.0));
  //sourceApps.Stop (Seconds (60.0));
  


  Simulator::Stop (Seconds(60.0));
  Simulator::Run ();

  uint64_t totalRx = 0;

  for (uint32_t i = 0; i < nSpokes; ++i) {

    Ptr<PacketSink> sink = DynamicCast<PacketSink> (sinkApps.Get(i));
    uint32_t bytesReceived = sink->GetTotalRx();
    totalRx += bytesReceived;
    std::cout << "Sink" << i << "\tTotalRx: " << bytesReceived * 1e-6 * 8 << "Mb";
    std::cout << "\tGoodput: " << (bytesReceived * 1e-6 * 8) / endTime << "Mbps" << std::endl;

  }

  std::cout << std::endl;
  std::cout << "\tTotalRx: " << totalRx * 1e-6 * 8<< "Mb";
  std::cout << "\tGoodput: " << (totalRx * 1e-6 * 8)/ endTime << "Mbps" << std::endl;

  Simulator::Destroy ();
  return 0;
}
