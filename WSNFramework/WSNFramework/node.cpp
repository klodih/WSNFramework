#include "stdafx.h"
#include "node.h"

NetworkNode::NetworkNode():
  Name(),
  Location(),
  SquaredRange(0),
  Properties(0),
  Speed(0),
  Neighbors() {
  }


NetworkNode::NetworkNode(const string& name, const Point_3& loc, double r, double s) :
    Name(name),
	Properties(0),
	Neighbors() { 
		SetLocation(loc);
		SetSquaredRange(r);
		SetSpeed(s);
	}

Point_3 NetworkNode::GetLocation() const{ return Location; }

double NetworkNode::GetSquaredRange() const{ return SquaredRange; }

uint NetworkNode::GetProp() const { return Properties; }

double NetworkNode::GetSpeed() const { return Speed; }

bool NetworkNode::IsProp(NodeInfo ni) const{
	return (Properties & ni);
}

bool NetworkNode::IsBoundary() { 
	return IsProp(NodeInfo::BOUNDARY); 
}

double NetworkNode::GetDistanceToNetworkNode(const NetworkNode& n) const {
	return CGAL::squared_distance(Location, n.GetLocation());
}

double NetworkNode::GetDistanceToLocation(const Point_3& loc) const {
	return CGAL::squared_distance(Location, loc);
}

void NetworkNode::SetLocation(const Point_3& loc) {
  Location = loc;
}

bool NetworkNode::SetLocationAndSquaredRange(const string &nodeLocation) {
	vector<string>	tokens = General::TokenizeString(nodeLocation, C_COMMA);
	if(tokens.size() == 4){
		Point_3 p(stod(tokens[0]), stod(tokens[1]), stod(tokens[2]));
		Location = p;
	    SetSquaredRange(stod(tokens[3]));
		return true;
	}
	else {
		cout << "WARNING: Wrong network node input: " + nodeLocation << " Defaulting to (0, 0, 0)." << endl;
		Point_3 p(0, 0, 0);
	    Location = p;
		SetSquaredRange(0);
		return false;
	}
}

bool NetworkNode::SetLocationFromGEXFStr(const string &gexfLoc) {
	vector<string> tokens = General::TokenizeString(gexfLoc, C_QUOTE);
	if(tokens.size() == 7) {
		Point_3 p(stod(tokens[1]), stod(tokens[3]), stod(tokens[5]));
		Location = p;
		return true;
	}
	else {
		cout << "WARNING: Wrong network node input: " + gexfLoc << " Defaulting to (0, 0, 0)." << endl;
		Point_3 p(0, 0, 0);
	    Location = p;
		return false;
	}
}

void NetworkNode::SetSquaredRange(double r) {
	(r > 0) ? SquaredRange = r : SquaredRange = 0;
}

void NetworkNode::SetLookup(Lookup *lup) { MyLookup = lup; }

void NetworkNode::SetSpeed(double s) {
	s >= 0 ? Speed = s : s = 0;
}

void NetworkNode::AddProp(NodeInfo ni) {
	Properties |= ni;
}

bool NetworkNode::AddNeighbor(const Neighbor &n){
	bool exists = false;
	for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
		if(it->Node == n.Node)
			exists = true;
	}

	if(!exists) {
		Neighbors.push_back(n);
		return true;
	}
	return false;
}

bool NetworkNode::RemoveNeighbor(const Neighbor &n){
	try {
		for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
			if(it->Node == n.Node) {
				Neighbors.erase(it);
				return true;
			}
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

bool NetworkNode::HasNeighbor(const Neighbor &n){
	for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
		if(it->Node == n.Node)
			return true;
	}
	return false;
}

bool NetworkNode::HasNeighbor(const string& neighborName) const{
	for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
		if((it->Node)->Name.compare(neighborName) == 0)
			return true;
	}
	return false;
}

string NetworkNode::GetClosestNeighborToNode(const string& destName) const {	
	string  closestNeighborName = Name; //if no closer neighbor is found to destination, then set closest neighbor to this
	Point_3 destLoc;
				  
	if(Neighbors.size() > 0 && MyLookup != nullptr && MyLookup->GetLocationOfNode(destName, destLoc)) {
		double minDist = GetDistanceToLocation(destLoc),
			   dist;

	   for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
		   if(it->Node != nullptr) {
			   dist = CGAL::squared_distance(it->Node->GetLocation(), destLoc);
				if(dist < minDist) {
					minDist = dist;
					closestNeighborName = it->Node->Name;
				}	
		   }	
		}
	}
	return closestNeighborName;
}

string NetworkNode::GetClosestNeighbor() const {
	string closestNghb;
	if(Neighbors.size() > 0) {
		auto it = Neighbors.begin();
		double minDist = it->Distance;
		closestNghb =it->Node->Name;
		while(it != Neighbors.end()) {
			if(it->Distance < minDist)
				closestNghb = it->Node->Name;
			++it;
		}
	}
	else
		closestNghb = Name;
	return closestNghb;
}

bool NetworkNode::operator==(const NetworkNode& n1) const{
	return Location == n1.Location && SquaredRange == n1.SquaredRange;
}

bool NetworkNode::operator!=(const NetworkNode& n1) const{
	return !operator==(n1);
}

bool NetworkNode::operator>(const NetworkNode& n1) const{
	return Location > n1.GetLocation();
}

bool NetworkNode::operator<(const NetworkNode& n1) const{
	return Location > n1.GetLocation();
}

bool NetworkNode::GetAllNodesConnectedToNode(vector<string> &connectednodes) {
	for(auto n_it = Neighbors.begin(); n_it != Neighbors.end(); ++n_it) {
		bool neighborExists = false;
		for(auto v_it = connectednodes.begin(); v_it != connectednodes.end(); ++v_it) {
			if(n_it->Node->Name.compare(*v_it) == 0) {
				neighborExists = true;
				break;
			}
		}
		if(!neighborExists) {
			connectednodes.push_back(n_it->Node->Name);
			return n_it->Node->GetAllNodesConnectedToNode(connectednodes);
		}
	}
	return true;
}