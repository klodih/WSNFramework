#pragma once
#ifndef NODE_H
#define NODE_H

#include "lookup.h"
#include "message.h"

class NetworkNode {
public:
	struct Neighbor {
		NetworkNode *Node;
		double Distance;

		Neighbor(NetworkNode *n, double d):
		  Node(n),
		  Distance(d) {}
	};

	string Name;
	vector<Neighbor> Neighbors;

	NetworkNode();
	NetworkNode(const string& n);
	NetworkNode(const string& n, const Point_3& p, double r, double s);

	//Getters
	Point_3	GetLocation() const { return Location; };
	double	GetSquaredRange() const { return SquaredRange; };
	double	GetSpeed() const { return Speed; };
	uint	GetProp() const { return Properties; };
	double	GetDistanceToNetworkNode(const NetworkNode & n) const { return CGAL::squared_distance(Location, n.GetLocation()); };
	double	GetDistanceToLocation(const Point_3 & loc) const { return CGAL::squared_distance(Location, loc); };
	string	GetClosestNeighbor() const;
	bool    GetAllNodesConnectedToNode(vector<string> &connectednodes);
	Neighbor GetClosestNeighborToNode(const string & n) const;

	//Setters
	void	SetLocation(const Point_3 &p) { Location = p; };
	bool    SetLocationAndSquaredRange(const string &nodeLocation);
	bool    SetLocationFromGEXFStr(const string &gexfLoc);
	void	SetSquaredRange(const double r) { (r > 0) ? SquaredRange = r : SquaredRange = 0; };
	void	SetLookup(Lookup *l) { MyLookup = l; };
	void	SetSpeed(const double s) { s >= 0 ? Speed = s : Speed = 0; };

	//Checkers
	bool	IsProp(NodeInfo p) const { return (Properties & p); };
	bool	IsBoundary() { return IsProp(NodeInfo::BOUNDARY); };
	bool	HasNeighbor(const Neighbor &n);
	bool	HasNeighbor(const string &n) const;

	//Modifiers
	void	AddProp(const NodeInfo ni) { Properties |= ni; };
	bool	AddNeighbor(const Neighbor &n);
	bool	RemoveNeighbor(const Neighbor &n);

	//Input/Output
	void	PrintNode(string &) const;
	void 	PrintNeighbors(string &) const;
	
	//Routing
	NetworkNode* RoutePureGreedy(Message &m);
	NetworkNode* RoutePureRandom(Message &m);

	//operations
	bool	operator==(const NetworkNode &n) const;
	bool	operator!=(const NetworkNode &n) const;
	bool    operator>(const NetworkNode &n) const;
	bool    operator<(const NetworkNode &n) const;

private:
	Point_3 Location;
	double	SquaredRange;
	uint	Properties;
	double	Speed;
    Lookup  *MyLookup;
};
#endif //NODE_H