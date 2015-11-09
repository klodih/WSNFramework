#include "stdafx.h"
#include "node.h"
#include "generalFunctions.h"

//-----  CONSTRUCTORS  -----
Node::Node():
  Name(),
  Location(),
  SquaredRange(0),
  Properties(0),
  Speed(0),
  Neighbors() {
  }

Node::Node(const string & name):
  Name(name),
  Location(),
  SquaredRange(0),
  Properties(0),
  Speed(0),
  Neighbors() {
  }

Node::Node(const string& name, const Point_3& loc, double r, double s) :
    Name(name),
	Properties(0),
	Neighbors() { 
		SetLocation(loc);
		SetSquaredRange(r);
		SetSpeed(s);
}

//-----  GETTERS  -----
string Node::GetClosestNeighborToNode(const string& destName) const {	
	string n = ""; //if no closer neighbor is found to destination, then set closest neighbor to this
	Point_3 destLoc;
				  
	if(Neighbors.size() > 0 && MyLookup != nullptr && MyLookup->GetLocationOfNode(destName, destLoc)) {
		double minDist = DistanceToLocation(destLoc),
			   dist;

	   for(auto it = Neighbors.begin(); it != Neighbors.end(); ++it) {
		  Point_3 loc;
		  if(MyLookup->GetLocationOfNode(it->first, loc)) {
			   dist = CGAL::squared_distance(loc, destLoc);
				if(dist < minDist) {
					minDist = dist;
					n = it->first;
				}	
		   }	
		}
	}
	return n;
}

//-----  SETTERS  -----
bool Node::SetLocationAndSquaredRangeFromStr(const string &nodeLocation) {
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

bool Node::SetLocationFromGEXFStr(const string &gexfLoc) {
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

//----- CHECKERS  -----

bool Node::HasNeighborInNeighborTable(const string &n) const {
	auto it = Neighbors.find(n);
	return (it != Neighbors.end());
}

//-----  MODIFIERS  -----
void Node::AddNeighbor(const string &n, const double dist){
	Neighbors[n] = dist;
}

void Node::AddMutualNeighborIfInRange(Node &n) {
	double dist = DistanceToNode(n);
	if(dist <= SquaredRange)
		AddNeighbor(n.Name, dist);
	if(dist <= n.SquaredRange)
		n.AddNeighbor(Name, dist);
}

bool Node::RemoveNeighbor(const string &n){
	auto it = Neighbors.find(n);
	try {
		if(it != Neighbors.end()) {
			Neighbors.erase(it);
			return true;
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

//----- OPERATIONS  -----

bool Node::operator==(const Node& n1) const{
	return Location == n1.Location && SquaredRange == n1.SquaredRange;
}

bool Node::operator!=(const Node& n1) const{
	return !operator==(n1);
}

bool Node::operator>(const Node& n1) const{
	return Location > n1.GetLocation();
}

bool Node::operator<(const Node& n1) const{
	return Location > n1.GetLocation();
}

//-----  INPUT/OUTPUT  -----

//eg. Node1=(1, 1, 1, r = 3)
void Node::PrintNode(string &nodeStr) const {
	stringstream nStr;
    nStr << Name << "=( " << Location.x() << ", " << Location.y() << ", " << Location.z() << ", r = " << SquaredRange << " )"; 
	nodeStr = nStr.str();
}

//eg. N(Node1)=[Node2, Node3, Node4]
void Node::PrintNeighbors(string &neighborStr) const {
	if(Neighbors.size() > 0) {
		stringstream nStr;
		auto it = Neighbors.begin();
		nStr << "N(" << Name << ")=[" << it->first;
		auto oneBeforeEnd = Neighbors.end();
		std::advance(oneBeforeEnd, -1);
		while(it != oneBeforeEnd) {
			++it;
			nStr << ", " << it->first;
		}
		nStr << "]" << endl;
			
		neighborStr = nStr.str();
	}
}

//-----  ROUTING -----
bool Node::RouteMessage(Message &message, RoutingAlg routingAlgorithm, string &nextNode) {
	bool success = false;
	switch(routingAlgorithm) {
		case PURE_GREEDY:
			success = RoutePureGreedy(message, nextNode);
			break;
		case PURE_RANDOM:
			success = RoutePureRandom(message, nextNode);
			break;
		case DIJKSTRA:
			success = RouteDijkstra(message, nextNode);
			break;
		default:
			cout<<"ERROR: Unknown Routing Algorithm name."<<endl;
	}
	return success;
}

bool Node::ReceiveMessage(Message &message) {
	if(message.GetEndNode() == Name) {
		message.AddNodeToPath(Name);
		return true;
	}
	else
		cout<<"ERROR: Node " << Name << " is not the destination node of this message..."<<endl;
	return false;
}

bool Node::RoutePureGreedy(Message &m, string &nextNode) {
	if(!IsProp(INPATH))
		AddProp(INPATH);
	m.AddNodeToPath(Name); //Add Current node to path for tracking purposes only
	nextNode = GetClosestNeighborToNode(m.GetEndNode());
	return !nextNode.empty();
}

bool Node::RoutePureRandom(Message &m, string &nextNode) {
	nextNode = "";
	if(!IsProp(INPATH))
		AddProp(INPATH);
	m.AddNodeToPath(Name); //Add Current node to path for tracking purposes only
	string neighbor;
	if(Neighbors.size() > 1) {
		//srand((uint)time(NULL));
		auto it = Neighbors.begin();
		std::advance(it, rand() % Neighbors.size());
		neighbor = it->first;
	}
	else if(Neighbors.size() == 1)
		neighbor = Neighbors.begin()->first;

	return !nextNode.empty();
}

bool Node::RouteDijkstra(Message &m, string &nextNode) {
	nextNode = "";
	m.AddNodeToPath(Name);
	if(m.Header_PrecomputedPath.size() > 0) {
		auto current = m.Header_PrecomputedPath.begin();
		if(*current == Name) {
			m.Header_PrecomputedPath.erase(current);
			nextNode = *m.Header_PrecomputedPath.begin();
		}
	}
	return !nextNode.empty();
}