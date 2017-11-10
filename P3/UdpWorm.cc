/*
	Author: Zeyu Chen
*/
#include "UdpWorm.h"
#include "ns3/seq-ts-header.h"
#include <iostream>

using namespace ns3;


uint32_t UdpWorm::totalinfected = 0;    // keeps count of total infected instances
uint32_t UdpWorm::scanrate;
uint32_t UdpWorm::infectionport;
string 	UdpWorm::scanpattern;
Ptr<UniformRandomVariable> UdpWorm::x = CreateObject<UniformRandomVariable>();
vector<Ipv4Address> UdpWorm::GlobalIpList;

void UdpWorm::SetScanRate(uint32_t rate) {
	scanrate = rate;
}
void UdpWorm::SetInfectionPort(uint32_t port) {
	infectionport = port;
}
void UdpWorm::SetScanPattern(string patternname) {
	scanpattern = patternname;
}

void UdpWorm::SetIpList(vector<Ipv4Address> GlobalipList, vector<Ipv4Address> LocalipList) {
	GlobalIpList = GlobalipList;
	LocalIpList = LocalipList;
}
void UdpWorm::SetUp(std::string Protocol, uint32_t infectionPort, double m_interval) {
	protocol = Protocol;
	infectionport = infectionPort;
	interval = Seconds(m_interval);
	typeId = TypeId::LookupByName(protocol);

	// Sink socket
	sinkSocket = Socket::CreateSocket (GetNode (), typeId); 
	//std::cout << CurrentIP() <<"eee" << std::endl;
	InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny(), infectionport);
	sinkSocket->Bind (local);
	sinkSocket->Listen ();
	sinkSocket->SetRecvCallback(MakeCallback(&UdpWorm::Listen, this));

  	// Initialize number of simultaneous connections
	//m_onoffSocket = std::vector< ns3::Ptr<ns3::Socket> > (m_numConn);
}

void UdpWorm::StartApplication () {
	//std::cout << "IP:" << CurrentIP() << " Initialized..." << std::endl;

	if (infected) {
		std::cout << "IP:" << CurrentIP() << " is the origin..." << std::endl;
        Sendworm();
	}
}
void UdpWorm::StopApplication() {

}
void UdpWorm::Infect() {
	if (!infected) {
		totalinfected++;
		infected = true;
		started = true;
		if(totalinfected > 1) {
			std::cout << "IP:" << CurrentIP() << " is infected!"<< " || ";
			std::cout << Simulator::Now().GetSeconds() <<"  SystemID:" << Simulator::GetSystemId()<< " :Totalinfected:" << totalinfected << std::endl;
		}
	}
	
}

void UdpWorm::Receive(Ptr<Packet> p) {
	if (p) {
     	if (!infected) {
     		//std::cout << CurrentIP() << " is attacked.."<<std::endl;
       		Infect();
       		Sendworm();
      	}
    }
}

void UdpWorm::Sendworm() {
	Ipv4Address target;
	target = GenerateNextIP();//Ipv4Address("10.5.1.2");
  	// Specify target ip and port, create properly size packet and send it
  	//std::cout << "IP:" << CurrentIP() << " is attacking "<< target <<std::endl;
  	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  	sendSocket = Socket::CreateSocket (GetNode (), tid);
  	InetSocketAddress inetAddress(target, infectionport);
  	Ptr<Packet> packet = Create<Packet> (20);
  	sendSocket->Bind ();
    sendSocket->Connect (inetAddress);
  	sendSocket->Send(packet);
  	sendEvent = Simulator::Schedule (interval, &UdpWorm::Sendworm, this);
}
bool UdpWorm::IsInfected() {
	return infected;
}

void UdpWorm::Listen(Ptr<Socket> socket) {
	Address from;
    Ptr<Packet> p;
	while (p = socket->RecvFrom (from)) {
    	if (p -> GetSize() > 0) {
    		Receive(p);
     		
     		//std::cout << CurrentIP() <<":r" << std::endl;
    	}    	
        //std::cerr << "Number of Infected:" << totalinfected << std::endl;
	}
}
Ipv4Address UdpWorm::CurrentIP() {
	Ptr<Ipv4> ipv4 = GetNode()->GetObject<Ipv4>();
   	Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
	Ipv4Address addri = iaddr.GetLocal ();
	return addri;
}
Ipv4Address UdpWorm::GenerateNextIP() {
	if (scanpattern == "Uniform")
		//std::cout << 1 << std::endl;
		return UniGenerateNextIP(GlobalIpList);
	if (scanpattern == "Local")
		return LocGenerateNextIP(LocalIpList, GlobalIpList);
	if (scanpattern == "Sequential")
		return SeqGenerateNextIP(GlobalIpList);
	std::cout << "Error: ScanPattern not Found" << std::endl;
	return NULL;
}

Ipv4Address UdpWorm::UniGenerateNextIP(vector<Ipv4Address> GlobalipList) {
	uint32_t size = GlobalipList.size();
	return GlobalipList[uint32_t(x -> GetValue(0, size))];
}

Ipv4Address UdpWorm::LocGenerateNextIP(vector<Ipv4Address> LocalipList, vector<Ipv4Address> GlobalipList) {// Scan ip in the same logical process
	uint32_t size = GlobalipList.size();
	uint32_t lsize = LocalipList.size();
	if(x ->GetValue(0, 1) <= 0.65) {
		//std::cout << 1 << std::endl;
		return LocalIpList[uint32_t(x -> GetValue(0, lsize))];
	}
	else {
		//std::cout << 2 << std::endl;
		return GlobalipList[uint32_t(x -> GetValue(0, size))];
	}
}

Ipv4Address UdpWorm::SeqGenerateNextIP(vector<Ipv4Address> GlobalipList) {
	if(seq_index < GlobalIpList.size())
		return GlobalipList[seq_index++];
	else {
		seq_index = 0;
		return GlobalipList[seq_index++];
	}
}