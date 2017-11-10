/*
	Author: Zeyu Chen

	The p2pCampusHelper allows users to create a 2-level network.
	The network maintain lists to indicate the current node is valid or not. The lists are used when install application.
	The example of ip address is:
	If given 10.1.0.0 as base and 255.255.0.0 as mask
	The upper level varies at the third, the lower leverl varies at the fourth. 
	i.e. The inner:10.1.2.2, 10.1.3.2;The outer:10.1.2.4, 10.1.2.6
*/

#pragma once

#include <map>
#include <iostream>
#include <string>
#include "ns3/point-to-point-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv6-address-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/random-variable-stream.h"

using namespace ns3;
using namespace std;


class PointToPointCampusHelper {
private:
	NodeContainer m_hub;               // Hub node  
	NetDeviceContainer m_hubDevices;   // Hub node NetDevices
	NodeContainer m_inner;             // Inner nodes  
	NetDeviceContainer m_innerDevices; // Inner nodes NetDevices
	NetDeviceContainer m_ioDevices;    // Inner nodes NetDevices 2 outer
	NodeContainer m_outer;             // Outer nodes (all) 
	NetDeviceContainer m_outerDevices; // Outer nodes NetDevices

	Ipv4InterfaceContainer m_hubInterfaces;     //IPv4 hub interfaces
  	Ipv4InterfaceContainer m_innerInterfaces;   //IPv4 inner nodes interfaces
  	Ipv4InterfaceContainer m_ioInterfaces;      //IPv4 inner nodes interfaces 2 outers
  	Ipv4InterfaceContainer m_outerInterfaces;   //IPv4 outer nodes interfaces

  	multimap<uint32_t, uint32_t> io_pair;          // <innernode, outernode>
  	multimap<uint32_t,uint32_t>::iterator it;

  	vector<uint32_t> innervalid;
  	vector<uint32_t>::iterator inner_it;
  	vector<uint32_t> outervalid;
  	vector<uint32_t>::iterator outer_it;

  	vector<Ipv4Address> LocalList;
  	vector<Ipv4Address>::iterator LocalList_it;

public:
	// Constructor
	PointToPointCampusHelper(uint32_t maxInner, uint32_t maxOuter, PointToPointHelper inner, PointToPointHelper outer, Ptr<UniformRandomVariable> rnd, uint32_t systemId);
	// Destructor
	~PointToPointCampusHelper();

	// Get Hub
	Ptr<Node> GetHub() const;

	// Get inner Nodes
	Ptr<Node> GetInner(uint32_t i) const;

	// Get outer Nodes
	Ptr<Node> GetOuter(uint32_t i) const;

	// Get Ipv4Address of Hub
	Ipv4Address GetHubIpv4Address (uint32_t i) const;

	// Get Ipv4Address of Inner
	Ipv4Address GetInnerIpv4Address (uint32_t i) const;

	// Get Ipv4Address of Outer
	Ipv4Address GetOuterIpv4Address (uint32_t i) const;

	// Return number of Inner
	uint32_t InnerCount () const;

	// Return number of Outer
	uint32_t OuterCount () const;

	// Install stack on every node
	void InstallStack (InternetStackHelper stack);

	// Install Address
	void AssignIpv4Addresses (Ipv4AddressHelper address);

	// Show all available ip address
	void ShowIpv4Addresses () const;

	// Check the node is available
	bool innerisValid(uint32_t id);

	bool outerisValid(uint32_t id);

	vector<Ipv4Address> BaseIpv4List();
};
