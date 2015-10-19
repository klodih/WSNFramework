#pragma once
#ifndef NODE_H
#define NODE_H

#include "lookup.h"
#include "message.h"

class NetworkNode {
public:
	struct Neighbor {
		NetworkNode *Node;
		double		Distance;

		Neighbor(NetworkNode *n, double d) {
			Node = n;
			Distance = d;
		}
	};

	string Name;
	vector<Neighbor> Neighbors;

	NetworkNode();
	NetworkNode(const string& n);
	NetworkNode(const string& n, const Point_3& p, double r, double s);

	Point_3	GetLocation() const;
	double	GetSquaredRange() const;
	double	GetSpeed() const;
	bool	IsBoundary();
	double	GetDistanceToNetworkNode(const NetworkNode & n) const;

	uint	GetProp() const;
	bool	IsProp(NodeInfo p) const;
	void	SetLocation(const Point_3 &p);
	bool    SetLocationAndSquaredRange(const string &nodeLocation);
	bool    SetLocationFromGEXFStr(const string &gexfLoc);
	void	SetSquaredRange(const double d);
	void	SetLookup(Lookup *l);
	void	SetSpeed(const double d);
	void	AddProp(const NodeInfo);
	bool	AddNeighbor(const Neighbor &n);
	bool	RemoveNeighbor(const Neighbor &n);
    bool	HasNeighbor(const Neighbor &n);
	bool	HasNeighbor(const string &n) const;
	bool	GetAllNodesConnectedToNode(vector<string> &connectednodes);
	double	GetDistanceToLocation(const Point_3 & p) const;
	string	GetClosestNeighborToNode(const string & n) const;
	string	GetClosestNeighbor() const;

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