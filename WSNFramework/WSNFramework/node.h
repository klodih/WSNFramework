#pragma once
#ifndef NODE_H
#define NODE_H

#include "lookup.h"
#include "message.h"
#include "comCenter.h"

class ComCenter;
class Node {
public:
	string Name;
	map<string, double> Neighbors;

	Node();
	Node(const string& n);
	Node(const string& n, const Point_3& p, double r, double s);

	//Getters
	Point_3	GetLocation() const { return Location; };
	double	GetSquaredRange() const { return SquaredRange; };
	double	GetSpeed() const { return Speed; };
	uint	GetProp() const { return Properties; };
	string  GetClosestNeighborToNode(const string & n) const;

	//Setters
	void	SetLocation(const Point_3 &p) { Location = p; };
	bool    SetLocationAndSquaredRangeFromStr(const string &nodeLocation);
	bool    SetLocationFromGEXFStr(const string &gexfLoc);
	void	SetSquaredRange(const double r) { (r > 0) ? SquaredRange = r : SquaredRange = 0; };
	void	SetLookup(Lookup *l) { MyLookup = l; };
	void	SetCOM(ComCenter *c) { MyCOM = c; };
	void	SetSpeed(const double s) { s >= 0 ? Speed = s : Speed = 0; };

	//Checkers
	bool	IsProp(NodeInfo p) const { return 0 != (Properties & p); };
	bool	IsBoundary() { return IsProp(BOUNDARY); };
	bool	HasNeighborInNeighborTable(const string &n) const;

	//Modifiers
	void	AddProp(const NodeInfo ni) { Properties |= ni; };
	void    ResetProp() { Properties = 0; };
	void	AddNeighbor(const string &n, const double dist);
	void	AddMutualNeighborIfInRange(Node &n);
	bool	RemoveNeighbor(const string &n);

	//Input/Output
	void	PrintNode(string &) const;
	void 	PrintNeighbors(string &) const;

	//operations
	bool	operator==(const Node &n) const;
	bool	operator!=(const Node &n) const;
	bool    operator>(const Node &n) const;
	bool    operator<(const Node &n) const;

	//Routing
	bool    RouteMessage(Message &h, RoutingAlg routingAlgorithm, string &nextNode);
	bool	ReceiveMessage(Message &h);

private:
	Point_3   Location;
	double	  SquaredRange;
	uint	  Properties;
	double	  Speed;
    Lookup*    MyLookup;
	ComCenter* MyCOM;

	double	DistanceToNode(const Node & n) const { return CGAL::squared_distance(Location, n.GetLocation()); };
	double	DistanceToLocation(const Point_3 & loc) const { return CGAL::squared_distance(Location, loc); };
		
	//Routing
	bool RoutePureGreedy(Message &m, string &nextNode);
	bool RoutePureRandom(Message &m, string &nextNode);
	bool RouteDijkstra(Message &m, string &nextNode);
};

#endif //NODE_H