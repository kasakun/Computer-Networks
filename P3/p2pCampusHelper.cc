/*
	Author: Zeyu Chen
*/
#include "p2pCampusHelper.h"

#include <iostream>

using namespace ns3;

PointToPointCampusHelper::PointToPointCampusHelper(uint32_t maxInner, 
												   uint32_t maxOuter, 
												   PointToPointHelper inner, 
												   PointToPointHelper outer, 
												   Ptr<UniformRandomVariable> rnd, 
												   uint32_t systemId) {
	m_hub.Create (1, systemId);
  	m_inner.Create (maxInner, systemId);  
  	m_outer.Create (maxOuter*maxInner, systemId);

  	uint32_t outer_id = 0; //node id
  	
  	// Use maxinner and maxouter to build the complete topology.
  	// inner_id -> outer_id (node)
  	// 0 -> 0, 1; 1 -> 2, 3;...;7 -> 14, 15
  	for (uint32_t i = 0; i < m_inner.GetN (); ++i) {
     	NetDeviceContainer nd_inner = inner.Install (m_hub.Get (0), m_inner.Get (i));
      	m_hubDevices.Add (nd_inner.Get (0));
      	m_innerDevices.Add (nd_inner.Get (1));
      	for (uint32_t j = 0; j < maxOuter; ++j) { 	
			NetDeviceContainer nd_outer = outer.Install (m_inner.Get (i), m_outer.Get (outer_id));
	    	io_pair.insert(make_pair(i, outer_id));
	    	m_ioDevices.Add (nd_outer.Get (0));
	    	//if (rnd -> GetValue(0, 1) > 0.6) continue;
	    	m_outerDevices.Add (nd_outer.Get (1));
	    	outer_id++;
		}  
	}

	// Use valid vector to determine the inner node is available
	for (uint32_t i = 0; i < m_inner.GetN (); ++i) {
		if (rnd -> GetValue(0, 1) < 0.8)
			innervalid.push_back (1);
		else
			innervalid.push_back (0);
	}

	// Use valid vector to determine the outer node is available
	for (uint32_t i = 0; i < m_outer.GetN (); ++i) {
		if (rnd -> GetValue(0, 1) < 0.6)
			outervalid.push_back (1);
		else
			outervalid.push_back (0);
	}
}

PointToPointCampusHelper::~PointToPointCampusHelper() {
}

Ptr<Node> PointToPointCampusHelper::GetHub () const {
  	return m_hub.Get (0);
}

Ptr<Node> PointToPointCampusHelper::GetInner(uint32_t i) const {
  	return m_inner.Get (i);
}

Ptr<Node> PointToPointCampusHelper::GetOuter(uint32_t i) const {
  	return m_outer.Get (i);
}
Ipv4Address PointToPointCampusHelper::GetHubIpv4Address (uint32_t i) const {
  	return m_hubInterfaces.GetAddress (i);
}
	
Ipv4Address PointToPointCampusHelper::GetInnerIpv4Address (uint32_t i) const {
  	return m_innerInterfaces.GetAddress (i);
}
	
Ipv4Address PointToPointCampusHelper::GetOuterIpv4Address (uint32_t i) const  {
  	return m_outerInterfaces.GetAddress (i);
}
	
uint32_t PointToPointCampusHelper::InnerCount () const {
  	return m_inner.GetN ();
}
	
uint32_t PointToPointCampusHelper::OuterCount () const {
	return m_outer.GetN();
}
	
void PointToPointCampusHelper::InstallStack (InternetStackHelper stack) {
	stack.Install (m_hub);
	stack.Install (m_inner);
	stack.Install (m_outer);
}
	
void PointToPointCampusHelper::AssignIpv4Addresses (Ipv4AddressHelper address) {
	// Subnets
	for (uint32_t i = 0; i < m_inner.GetN(); ++i) { 	
    	m_hubInterfaces.Add (address.Assign (m_hubDevices.Get (i)));
    	m_innerInterfaces.Add (address.Assign (m_innerDevices.Get (i)));
    	for (it = io_pair.lower_bound(i); it != io_pair.upper_bound(i); ++it) {
      		uint32_t  outer_id = it -> second;
      		m_ioInterfaces.Add (address.Assign (m_ioDevices.Get (outer_id)));
      		m_outerInterfaces.Add (address.Assign (m_outerDevices.Get(outer_id)));
    	}
    	address.NewNetwork ();	
    }
    // Construct Local List
    for (uint32_t i = 0; i < m_innerInterfaces.GetN(); ++i)
		LocalList.push_back(m_innerInterfaces.GetAddress(i));
	for (uint32_t i = 0; i < m_outerInterfaces.GetN(); ++i)
		LocalList.push_back(m_outerInterfaces.GetAddress(i));
}

void PointToPointCampusHelper::ShowIpv4Addresses () const {
	// Hub
	for (uint32_t i = 0; i < m_hubInterfaces.GetN(); ++i) 
		std::cout << "Hub:"<< m_hubInterfaces.GetAddress(i) << std::endl;
	for (uint32_t i = 0; i < m_innerInterfaces.GetN(); ++i) {
		Ptr<Ipv4> ipv4 = GetInner(i)->GetObject<Ipv4>();
   		Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
		Ipv4Address addri = iaddr.GetLocal ();
		std::cout << "Inner:"<< addri << std::endl;
	}
	for (uint32_t i = 0; i < m_outerInterfaces.GetN(); ++i) {
		Ptr<Ipv4> ipv4 = GetOuter(i)->GetObject<Ipv4>();
   		Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
		Ipv4Address addri = iaddr.GetLocal ();
		std::cout << "Outer:"<< addri << std::endl;
	}

}

bool PointToPointCampusHelper::innerisValid(uint32_t id) {
	return innervalid[id] == 1;
}

bool PointToPointCampusHelper::outerisValid(uint32_t id) {
	return outervalid[id] == 1 && innervalid[id/2] == 1;     // If the parent inner is invalid, the child is not either.
}

vector<Ipv4Address> PointToPointCampusHelper::BaseIpv4List() {
	return LocalList;
}