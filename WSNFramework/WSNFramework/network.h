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

	Network();

	bool   InitNetwork(double x, double y, double z, double nodeSquaredRange, uint count);
	bool   InitConnectedNetwork(double x, double y, double z, double nodeSquaredRange, uint count);
	bool   InitNetwork(const string &networkDefFile, bool Gexf = false, double squaredRange = 0);
	bool   InitConnectedNetwork(const string &networkDefFile, bool Gexf = false, double squaredRange = 0);
	int	   GetIndexOfNode(const NetworkNode &n) const;
	bool   GetNodeByName(const string &name, NetworkNode &n) const;
	uint   GetNodeCount() const;
	void   AddNode(NetworkNode &n);
	void   AddVirtualNode(const NetworkNode &n);
	bool   RemoveNode(NetworkNode &n);
	bool   RemoveNodeAtIndex(size_t idx);
	bool   RemoveNodeByName(const string &n);
	void   ComputeNetworkDiameter();
	//void   ComputeBoundaryNodes();
	//bool   AreNodesConnected(const string &n1, const string &n2, vector<string> &v);


private:
	double NetworkDiameter;
	Lookup LookupService;

	void   ComputeNodeNeighborhood(NetworkNode &node);
	void   InitializeLookupService();
	void   RemoveIsolatedNodes();
	void   RemoveIsolatedSegments1();
	void   RemoveIsolatedSegments2();
	bool   AreNodesConnected(const string &n1, const string &n2, vector<string> &v);
	void   GenerateRandomNetwork(double x, double y, double z, double nodeSquaredRange, uint count);
	bool   ReadNetworkFromFile(const string &fileName);
    bool   ReadNetworkFromGEFX(const string &gexfFileName, double range);
	//void   FindSpheresOfRaidusRHingedOnPoints(const Point_3 &, const Point_3 &, const Point_3 &, double, SK::Sphere_3 &, SK::Sphere_3 &);
	//bool   IsNodeOnBoundary(NetworkNode<Point_3> &, NetworkNode<Point_3> &, NetworkNode<Point_3> &);
	//bool   IsSphereHingedOnNodesEmpty(NetworkNode<Point_3> &, NetworkNode<Point_3> &, NetworkNode<Point_3> &, const SK::Sphere_3 &);
};

#endif //NETWORK_H