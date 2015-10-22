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
	vector<NetworkNode>	NetworkNodes;
	vector<NetworkNode> VirtualNodes;

	//Constructor & Init
	Network();
	bool   InitNetwork(double x, double y, double z, double nodeSquaredRange, uint count);
	bool   InitConnectedNetwork(double x, double y, double z, double nodeSquaredRange, uint count);
	bool   InitNetwork(const string &networkDefFile, bool Gexf = false, double squaredRange = 0);
	bool   InitConnectedNetwork(const string &networkDefFile, bool Gexf = false, double squaredRange = 0);

	//Getters
	int	   GetIndexOfNode(const NetworkNode &n) const;
	bool   GetNodeByName(const string &name, NetworkNode *n);
	uint   GetNodeCount() const { return NetworkNodes.size(); };

	//Modifiers
	void   AddNode(NetworkNode &n);
	void   AddVirtualNode(const NetworkNode &n);
	bool   RemoveNode(NetworkNode &n);
	bool   RemoveNodeAtIndex(size_t idx);
	bool   RemoveNodeByName(const string &n);
	void   ComputeNetworkDiameter();

	//Input/Output
	void   PrintNetwork(string &netStr) const;

	//Routing
	bool   RouteMessage(Message &m, RoutingAlg algorithm);	
	//void   ComputeBoundaryNodes();
	//bool   AreNodesConnected(const string &n1, const string &n2, vector<string> &v);


private:
	double NetworkDiameter;
	Lookup LookupService;

	//Compute/Modify
	void   ComputeNodeNeighborhood(NetworkNode *node);
	void   ComputeNeighborhoods();
	void   InitializeLookupService();
	void   RemoveIsolatedNodes();
	void   RemoveIsolatedSegments1();
	void   RemoveIsolatedSegments2();
	bool   AreNodesConnected(const string &n1, const string &n2, vector<string> &v);
	void   GenerateRandomNetwork(double x, double y, double z, double nodeSquaredRange, uint count);

	//Input/Output
	bool   ReadNetworkFromFile(const string &fileName);
    bool   ReadNetworkFromGEFX(const string &gexfFileName, double range);

	//Routing
	bool   RouteGreedy(Message &m, NetworkNode *start, NetworkNode *end);

	//void   FindSpheresOfRaidusRHingedOnPoints(const Point_3 &, const Point_3 &, const Point_3 &, double, SK::Sphere_3 &, SK::Sphere_3 &);
	//bool   IsNodeOnBoundary(NetworkNode<Point_3> &, NetworkNode<Point_3> &, NetworkNode<Point_3> &);
	//bool   IsSphereHingedOnNodesEmpty(NetworkNode<Point_3> &, NetworkNode<Point_3> &, NetworkNode<Point_3> &, const SK::Sphere_3 &);
};

#endif //NETWORK_H