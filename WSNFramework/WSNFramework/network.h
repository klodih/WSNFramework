#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include "node.h"
#include "message.h"
#include "lookup.h"

//Allowing only positive coordinate nodes. i.e. The network is "projected" onto the positive quadrant.
//So, all the geometry should still hold and generalization of theorems shouldn't be lost by this assumption.
class Network {
public:
	NodeSet	Nodes;
	NodeSet VirtualNodes;

	//Constructor & Init
	Network();

	//Getters
	bool   GetNodeByName(const string &name, Node *n);
	uint   GetNodeCount() const { return Nodes.size(); };

	//Modifiers
	void   AddNode(Node &n);
	void   AddVirtualNode(const Node &n);
	bool   RemoveNode(Node &n);
	bool   RemoveNodeByName(const string &n);
	void   ComputeNetworkDiameter();
	void   KeepLargestConnectedSegment();

	//Input/Output
	void   PrintNetwork(string &netStr) const;

	//Routing Interface
	bool   SendMessage(Message &m, RoutingAlg alg);



private:
	double    NetworkDiameter;
	Lookup	  LookupService;
	ComCenter COM;
		
	void ComputeNodeNeighborhood(Node &node);
	bool RemoveNodeAsNeighbor(Node &n);
	void RemoveIsolatedNodes();
	void RemoveIsolatedSegments();
	void GetAllNodesConnectedToNode(Node &n, vector<string> &connectednodes);

	//Routing
	//void   ComputeBoundaryNodes();
	//void   FindSpheresOfRaidusRHingedOnPoints(const Point_3 &, const Point_3 &, const Point_3 &, double, SK::Sphere_3 &, SK::Sphere_3 &);
	//bool   IsNodeOnBoundary(Node<Point_3> &, Node<Point_3> &, Node<Point_3> &);
	//bool   IsSphereHingedOnNodesEmpty(Node<Point_3> &, Node<Point_3> &, Node<Point_3> &, const SK::Sphere_3 &);
};

#endif //NETWORK_H