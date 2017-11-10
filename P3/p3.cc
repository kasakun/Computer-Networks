/*
	Author: Zeyu Chen

	This is the main file for the worm simulation. 
	Use p2pCampusHelper to create subnet and based on mpi implemention.

	The routing strategy is NixVector, which is more efficient than global.
*/
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <string>
#include <vector>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mpi-interface.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-nix-vector-helper.h"
#include "ns3/ipv4-address-helper.h"

#include "p2pCampusHelper.h"

#include "UdpWorm.h"

using namespace ns3;

typedef struct timeval TIMER_TYPE;
#define TIMER_NOW(_t) gettimeofday (&_t,NULL);
#define TIMER_SECONDS(_t) ((double)(_t).tv_sec + (_t).tv_usec * 1e-6)
#define TIMER_DIFF(_t1, _t2) (TIMER_SECONDS (_t1) - TIMER_SECONDS (_t2))


NS_LOG_COMPONENT_DEFINE ("Worm Simulation");

int main(int argc, char** argv) {

	//std::cout << "Hello" << std::endl; 
	// Random Setting
	TIMER_TYPE t0, t1, t2;
	TIMER_NOW (t0);
	RngSeedManager::SetSeed(4);
  	RngSeedManager::SetRun(1);

  	// Time
  	Time::SetResolution (Time::NS);

  	Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
	// Default param
	uint32_t ScanRate = 5;
	std::string BackboneDelay = "5ms";
	std::string ScanPattern = "Uniform";
	std::string SyncType = "Yawns";
	vector<Ipv4Address> GlobalIpList;
	double sim = 5.0;

	// cmd
	CommandLine cmd;
  	cmd.AddValue ("ScanRate", "Rate to generate worm traffic(5, 10, 20)ms, default = 5", ScanRate); 
  	cmd.AddValue ("ScanPattern", "Scanning pattern(Uniform, Local, Sequential), default = Uniform", ScanPattern);
  	cmd.AddValue ("BackboneDelay", "ms delay for backbone links (lookahead), default = 5ms(Please type in as a string)", BackboneDelay);
  	cmd.AddValue ("SyncType", "Conservative algorithm (Yawns, Null), default  = Yawns", SyncType);
  	cmd.Parse (argc, argv);

  	if (SyncType == "Null") {
  		GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::NullMessageSimulatorImpl"));
  	}
  	else if (SyncType == "Yawns") {
  		GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::DistributedSimulatorImpl"));
  	}

  	// Mpi 
  	MpiInterface::Enable (&argc, &argv);
  	uint32_t systemId = MpiInterface::GetSystemId ();
  	uint32_t systemCount = MpiInterface::GetSize ();

  	if (systemCount != 1 && systemCount != 2 && systemCount != 4) {
  		std::cout << "This simulation requires 1, 2, 4 logical processors." << std::endl;
  		return 1;
  	}

    // P2P inner
    PointToPointHelper hub2hub;
  	hub2hub.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
    hub2hub.SetChannelAttribute ("Delay", StringValue (BackboneDelay));

    PointToPointHelper hub2inner;
  	hub2inner.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
    hub2inner.SetChannelAttribute ("Delay", StringValue ("5ms"));

    PointToPointHelper inner2child;
  	inner2child.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
    inner2child.SetChannelAttribute ("Delay", StringValue ("8ms"));

  	ApplicationContainer sinkApps; // holes
  	ApplicationContainer wormApps;
    
    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), 5000));
    
    if (systemCount == 1) {
    	PointToPointCampusHelper base1(8, 2, hub2inner,inner2child, x, systemId); 
    	PointToPointCampusHelper base2(8, 2, hub2inner,inner2child, x, systemId); 
    	PointToPointCampusHelper base3(8, 2, hub2inner,inner2child, x, systemId); 
    	PointToPointCampusHelper base4(8, 2, hub2inner,inner2child, x, systemId); 

		// Stack
		InternetStackHelper stack;
		// Ipv4 use nix vector
  		Ipv4NixVectorHelper nixRouting;
    	stack.SetRoutingHelper (nixRouting);
		base1.InstallStack(stack);
		base2.InstallStack(stack);
		base3.InstallStack(stack);
		base4.InstallStack(stack);

		// Ipv4
		Ipv4AddressHelper address;
		address.SetBase ("10.1.1.0", "255.255.255.0");
		base1.AssignIpv4Addresses (address);
		address.SetBase ("10.2.1.0", "255.255.255.0");
		base2.AssignIpv4Addresses (address);
		address.SetBase ("10.3.1.0", "255.255.255.0");
		base3.AssignIpv4Addresses (address);
		address.SetBase ("10.4.1.0", "255.255.255.0");
		base4.AssignIpv4Addresses (address);

		//GlobalIpList.reserve(4*base1.BaseIpv4List().size());
		vector<Ipv4Address> baselist1 = base1.BaseIpv4List();
		vector<Ipv4Address> baselist2 = base2.BaseIpv4List();
		vector<Ipv4Address> baselist3 = base3.BaseIpv4List();
		vector<Ipv4Address> baselist4 = base4.BaseIpv4List();
		GlobalIpList.insert(GlobalIpList.end(), baselist1.begin(), baselist1.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist2.begin(), baselist2.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist3.begin(), baselist3.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist4.begin(), baselist4.end());
		//std::cout << GlobalIpList.size()<< std::endl;

		// Hub Connect
		NodeContainer HubNodes;
		HubNodes.Add(base1.GetHub());
		HubNodes.Add(base2.GetHub());
		HubNodes.Add(base3.GetHub());
		HubNodes.Add(base4.GetHub());
		
		NetDeviceContainer HubDevices;
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base3.GetHub(), base4.GetHub()));
		//HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		
		address.SetBase ("10.5.1.0", "255.255.255.0");
		address.Assign (HubDevices);

		// base1
		for (uint32_t i = 0; i < base1.InnerCount(); i++) {
			if (base1.innerisValid(i) == false)            //The Node is supposed to be hole
				continue; 									
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base1.GetInner(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist1); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}
		for (uint32_t i = 0; i < base1.OuterCount(); i++) {
			if (base1.outerisValid(i) == false)            //The Node is supposed to be hole
				continue; 							
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base1.GetOuter(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist1); 
			//Set the origin worm
			if (base1.GetOuter(i)->GetId() == 9)
				wormapp->Infect();
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}

		// base2
		for (uint32_t i = 0; i < base2.InnerCount(); i++) {
			if (base2.innerisValid(i) == false)            //The Node is supposed to be hole
				continue; 									
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base2.GetInner(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist2); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}
		for (uint32_t i = 0; i < base2.OuterCount(); i++) {
			if (base2.outerisValid(i) == false)            //The Node is supposed to be hole
				continue; 		
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base2.GetOuter(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist2); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}

		// base3
		for (uint32_t i = 0; i < base3.InnerCount(); i++) {
			if (base3.innerisValid(i) == false)            //The Node is supposed to be hole
				continue; 
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base3.GetInner(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist3); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}
		for (uint32_t i = 0; i < base3.OuterCount(); i++) {
			if (base3.outerisValid(i) == false)            //The Node is supposed to be hole
				continue; 		
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base3.GetOuter(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist3); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}

		// base4
		for (uint32_t i = 0; i < base4.InnerCount(); i++) {
			if (base4.innerisValid(i) == false)            //The Node is supposed to be hole
				continue; 
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base4.GetInner(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist4); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}
		for (uint32_t i = 0; i < base4.OuterCount(); i++) {
			if (base4.outerisValid(i) == false)            //The Node is supposed to be hole
				continue; 		
			Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
			base4.GetOuter(i) -> AddApplication(wormapp);
			wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
			wormapp->SetScanPattern(ScanPattern);
			wormapp->SetIpList(GlobalIpList, baselist4); 
			wormapp->SetStartTime (Seconds (0.0));
			wormapp->SetStopTime (Seconds (sim));
		}

		AsciiTraceHelper ascii;
  		hub2inner.EnableAsciiAll(ascii.CreateFileStream("p3hub.tr"));
  		PointToPointHelper ptp;
      	std::stringstream stmp;
      	stmp << "./pcap/" << "/red";
      	inner2child.EnablePcapAll (stmp.str ().c_str ());

    }

    if (systemCount == 2) {
    	PointToPointCampusHelper base1(8, 2, hub2inner,inner2child, x, 0); 
    	PointToPointCampusHelper base2(8, 2, hub2inner,inner2child, x, 0); 
    	PointToPointCampusHelper base3(8, 2, hub2inner,inner2child, x, 1); 
    	PointToPointCampusHelper base4(8, 2, hub2inner,inner2child, x, 1); 

		// Stack
		InternetStackHelper stack;
		// Ipv4 use nix vector
  		Ipv4NixVectorHelper nixRouting;
    	stack.SetRoutingHelper (nixRouting);
		base1.InstallStack(stack);
		base2.InstallStack(stack);
		base3.InstallStack(stack);
		base4.InstallStack(stack);

		// Ipv4
		Ipv4AddressHelper address;
		address.SetBase ("10.1.1.0", "255.255.255.0");
		base1.AssignIpv4Addresses (address);
		address.SetBase ("10.2.1.0", "255.255.255.0");
		base2.AssignIpv4Addresses (address);
		address.SetBase ("10.3.1.0", "255.255.255.0");
		base3.AssignIpv4Addresses (address);
		address.SetBase ("10.4.1.0", "255.255.255.0");
		base4.AssignIpv4Addresses (address);

		//GlobalIpList.reserve(4*base1.BaseIpv4List().size());
		vector<Ipv4Address> baselist1 = base1.BaseIpv4List();
		vector<Ipv4Address> baselist2 = base2.BaseIpv4List();
		vector<Ipv4Address> baselist3 = base3.BaseIpv4List();
		vector<Ipv4Address> baselist4 = base4.BaseIpv4List();
		GlobalIpList.insert(GlobalIpList.end(), baselist1.begin(), baselist1.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist2.begin(), baselist2.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist3.begin(), baselist3.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist4.begin(), baselist4.end());
		
		// Hub Connect
		NodeContainer HubNodes;
		HubNodes.Add(base1.GetHub());
		HubNodes.Add(base2.GetHub());
		HubNodes.Add(base3.GetHub());
		HubNodes.Add(base4.GetHub());
		
		NetDeviceContainer HubDevices;
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base3.GetHub(), base4.GetHub()));
		//HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		
		address.SetBase ("10.5.1.0", "255.255.255.0");
		address.Assign (HubDevices);

		if (systemId == 0) {
			// base1
			for (uint32_t i = 0; i < base1.InnerCount(); i++) {
				if (base1.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base1.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist1); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base1.OuterCount(); i++) {
				if (base1.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base1.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist1); 
				//Set the origin worm
				if (base1.GetOuter(i)->GetId() == 9)
					wormapp->Infect();
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}

			// base2
			for (uint32_t i = 0; i < base2.InnerCount(); i++) {
				if (base2.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base2.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist2); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base2.OuterCount(); i++) {
				if (base2.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base2.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist2); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
    	}
		if (systemId == 1) {
			// base3
			for (uint32_t i = 0; i < base3.InnerCount(); i++) {
				if (base3.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base3.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist3); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base3.OuterCount(); i++) {
				if (base3.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base3.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist3); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}

			// base4
			for (uint32_t i = 0; i < base4.InnerCount(); i++) {
				if (base4.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base4.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist4); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base4.OuterCount(); i++) {
				if (base4.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base4.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist4); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			} 	
    	}
    	
    }

    if (systemCount == 4) {
    	PointToPointCampusHelper base1(8, 2, hub2inner,inner2child, x, 0); 
    	PointToPointCampusHelper base2(8, 2, hub2inner,inner2child, x, 1); 
    	PointToPointCampusHelper base3(8, 2, hub2inner,inner2child, x, 2); 
    	PointToPointCampusHelper base4(8, 2, hub2inner,inner2child, x, 3); 

		// Stack
		InternetStackHelper stack;
		// Ipv4 use nix vector
  		Ipv4NixVectorHelper nixRouting;
    	stack.SetRoutingHelper (nixRouting);
		base1.InstallStack(stack);
		base2.InstallStack(stack);
		base3.InstallStack(stack);
		base4.InstallStack(stack);

		// Ipv4
		Ipv4AddressHelper address;
		address.SetBase ("10.1.1.0", "255.255.255.0");
		base1.AssignIpv4Addresses (address);
		address.SetBase ("10.2.1.0", "255.255.255.0");
		base2.AssignIpv4Addresses (address);
		address.SetBase ("10.3.1.0", "255.255.255.0");
		base3.AssignIpv4Addresses (address);
		address.SetBase ("10.4.1.0", "255.255.255.0");
		base4.AssignIpv4Addresses (address);

		//GlobalIpList.reserve(4*base1.BaseIpv4List().size());
		vector<Ipv4Address> baselist1 = base1.BaseIpv4List();
		vector<Ipv4Address> baselist2 = base2.BaseIpv4List();
		vector<Ipv4Address> baselist3 = base3.BaseIpv4List();
		vector<Ipv4Address> baselist4 = base4.BaseIpv4List();
		GlobalIpList.insert(GlobalIpList.end(), baselist1.begin(), baselist1.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist2.begin(), baselist2.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist3.begin(), baselist3.end());
		GlobalIpList.insert(GlobalIpList.end(), baselist4.begin(), baselist4.end());

		// Hub Connect
		NodeContainer HubNodes;
		HubNodes.Add(base1.GetHub());
		HubNodes.Add(base2.GetHub());
		HubNodes.Add(base3.GetHub());
		HubNodes.Add(base4.GetHub());
		
		NetDeviceContainer HubDevices;
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base1.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base3.GetHub()));
		HubDevices.Add(hub2hub.Install(base2.GetHub(), base4.GetHub()));
		HubDevices.Add(hub2hub.Install(base3.GetHub(), base4.GetHub()));
		//HubDevices.Add(hub2hub.Install(base1.GetHub(), base2.GetHub()));
		
		address.SetBase ("10.5.1.0", "255.255.255.0");
		address.Assign (HubDevices);

		if (systemId == 0) {
			// base1
			for (uint32_t i = 0; i < base1.InnerCount(); i++) {
				if (base1.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base1.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist1); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base1.OuterCount(); i++) {
				if (base1.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base1.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist1); 
				//Set the origin worm
				if (base1.GetOuter(i)->GetId() == 9)
					wormapp->Infect();
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
		}
		if (systemId == 1){
			// base2
			for (uint32_t i = 0; i < base2.InnerCount(); i++) {
				if (base2.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base2.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist2); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base2.OuterCount(); i++) {
				if (base2.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base2.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist2); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
    	}

		if (systemId == 2) {
			// base3
			for (uint32_t i = 0; i < base3.InnerCount(); i++) {
				if (base3.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base3.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist3); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base3.OuterCount(); i++) {
				if (base3.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base3.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist3); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
		}
		if (systemId == 3) {
			// base4
			for (uint32_t i = 0; i < base4.InnerCount(); i++) {
				if (base4.innerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base4.GetInner(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist4); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			}
			for (uint32_t i = 0; i < base4.OuterCount(); i++) {
				if (base4.outerisValid(i) == false)            //The Node is supposed to be hole
					continue; 
				Ptr<UdpWorm> wormapp = CreateObject<UdpWorm>();
				base4.GetOuter(i) -> AddApplication(wormapp);
				wormapp->SetUp ("ns3::UdpSocketFactory", 5000, double(ScanRate)/1000);
				wormapp->SetScanPattern(ScanPattern);
				wormapp->SetIpList(GlobalIpList, baselist4); 
				wormapp->SetStartTime (Seconds (0.0));
				wormapp->SetStopTime (Seconds (sim));
			} 	
    	}
    }
	
	
  	TIMER_NOW (t1);


    Simulator::Stop (Seconds (sim));
    
	Simulator::Run ();
	Simulator::Destroy ();
	// Exit the MPI execution environment
	MpiInterface::Disable ();
	TIMER_NOW (t2);
	double d1 = TIMER_DIFF (t1, t0), d2 = TIMER_DIFF (t2, t1);
	std::cout << "-----" << std::endl << "Runtime Stats:" << std::endl;
  	std::cout << "Simulator init time: " << d1 << std::endl;
  	std::cout << "Simulator run time: " << d2 << std::endl;
  	std::cout << "Total elapsed time: " << d1 + d2 << std::endl;
	return 0;
}
