/*
	Author: Zeyu Chen

	This is the UdpWorm class, inherited from Application class.
	The worm has three scanpattern and can be set at different scanrate.
*/
#pragma once

#include <iostream>
#include <string>

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/event-id.h"
#include "ns3/data-rate.h"
#include "ns3/traced-callback.h"
#include "ns3/core-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv6-address-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/random-variable-stream.h"

#include <vector>

using namespace ns3;
using namespace std;

class UdpWorm : public Application {
private:
	static uint32_t totalinfected;    // keeps count of total infected instances
	static uint32_t scanrate;
	static uint32_t infectionport;
	static string scanpattern;
	static Ptr<UniformRandomVariable> x;
	static vector<Ipv4Address> GlobalIpList;

	//UdpSocketImpl* udp;

	bool infected;       // indicates if this node is in infected stage
	bool started;        // used to know if the application of worm has started
	uint32_t seq_index;
	vector<Ipv4Address> LocalIpList;

	TypeId typeId;
	string protocol;

	//uint32_t count;     //!< Maximum number of packets the application will send
	Time interval;      //!< Packet inter-send time
	//uint32_t size;      //!< Size of the sent packet (including the SeqTsHeader)

	//uint32_t sent;      //!< Counter for sent packets
	Ptr<Socket> sendSocket; //!< Socket
	Ptr<Socket> sinkSocket; //!< Socket
	EventId sendEvent;  //!< Event to send the next packet
	
public:
	UdpWorm() {
		seq_index = 0;
		infected = false;
		started = false;
		interval = Seconds(0.005);};
	
	~UdpWorm() {};

	// Function
	//Application* Copy() const;// Make a copy of the application

	void SetUp(std::string protocol, uint32_t infectionPort, double m_interval);
	void StartApplication();    // Called at time specified by Start
	void StopApplication();     // Called at time specified by Stop
	void Infect();
	void Receive(Ptr<Packet> p);
	void Sendworm();
	void Listen(Ptr<Socket> socket);
	bool IsInfected();


  	
	static void SetScanRate(uint32_t rate);
	static void SetInfectionPort(uint32_t port);
	static void SetScanPattern(string patternname);
	static uint32_t TotalInfected() {return totalinfected;};
	void SetIpList(vector<Ipv4Address> GlobalipList, vector<Ipv4Address> LocalipList);

	// Scanning Pattern
	Ipv4Address CurrentIP();
	Ipv4Address GenerateNextIP();
	Ipv4Address UniGenerateNextIP(vector<Ipv4Address> GlobalipList);
	Ipv4Address LocGenerateNextIP(vector<Ipv4Address> LocalipList, vector<Ipv4Address> GlobalipList);// Scan ip in the same logical process
	Ipv4Address SeqGenerateNextIP(vector<Ipv4Address> GlobalipList);
};
