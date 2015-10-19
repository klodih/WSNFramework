#include "stdafx.h"
#include "network.h"

Network::Network() :
  NetworkNodes(),
  VirtualNodes(),
  NetworkDiameter(0){ }

bool Network::InitNetwork(double NetworkX, double NetworkY, double NetworkZ, double nodeSquaredRange, uint count) {
	GenerateRandomNetwork(NetworkX, NetworkY, NetworkZ, nodeSquaredRange, count);
	InitializeLookupService();
	return ( (count - 0.1*count) <= NetworkNodes.size()); //return false if we have more than 10% node collisions
}

bool Network::InitConnectedNetwork(double NetworkX, double NetworkY, double NetworkZ, double nodeSquaredRange, uint count) {
	if(InitNetwork(NetworkX, NetworkY, NetworkZ, nodeSquaredRange, count)) {
		RemoveIsolatedNodes();
		RemoveIsolatedSegments2();
	}
	return false;
}

bool Network::InitNetwork(const string &networkDefFile, bool gexf, double squaredRange) {
	bool success = false;
	gexf ? success = ReadNetworkFromGEFX(networkDefFile, squaredRange) : success = ReadNetworkFromFile(networkDefFile);
	InitializeLookupService();
	return success;
}

bool Network::InitConnectedNetwork(const string &networkDefFile, bool gexf, double squaredRange) {
	if(InitNetwork(networkDefFile, gexf, squaredRange)) {
		RemoveIsolatedNodes();
		RemoveIsolatedSegments2();
	}
	return false;
}

int	Network::GetIndexOfNode(const NetworkNode &node) const {
	for(size_t i = 0 ; i < GetNodeCount(); ++i) {
		if(NetworkNodes[i] == node)
			return i;
	}
	return -1;
}

bool Network::GetNodeByName(const string& nodeNm, NetworkNode &node) const {
	for(auto iter = NetworkNodes.begin(); iter != NetworkNodes.end(); ++iter) {
		if(iter->Name.compare(nodeNm) == 0) {
			node = *iter;
			return true;
		}
	}
	return false;
}

uint Network::GetNodeCount() const{ 
	return NetworkNodes.size(); 
}

void Network::AddNode(NetworkNode &n) {
	bool isNode = false;
	for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
		if(*it == n) {
			isNode = true;
			break;
		}
	}

	if(!isNode && n.GetLocation().x() >= 0 && n.GetLocation().y() >= 0 && n.GetLocation().z() >= 0) {
		ComputeNodeNeighborhood(n);
		NetworkNodes.push_back(n);
	}
}

void Network::AddVirtualNode(const NetworkNode &n) {
	bool isNode = false;
	for(auto it = VirtualNodes.begin(); it != VirtualNodes.end(); ++it) {
		if(*it == n) {
			isNode = true;
			break;
		}
	}
	if(!isNode)
		VirtualNodes.push_back(n);
}

bool Network::RemoveNode(NetworkNode &n){
	try {
		for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
		    if(*it == n) {
				NetworkNodes.erase(it);
				return true;
			}
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}

	return false;
}

bool Network::RemoveNodeAtIndex(size_t idx) {
	try {
		if(idx < NetworkNodes.size()) {
			NetworkNodes.erase(NetworkNodes.begin() + idx);
			return true;
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

bool Network::RemoveNodeByName(const string &name) {
	try {
		for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
			if(it->Name.compare(name) == 0) {
				NetworkNodes.erase(it);
				return true;
			}
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

void Network::RemoveIsolatedNodes() {
	try {
		for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
			if(it->Neighbors.size() == 0)
				NetworkNodes.erase(it);
		}
	}
	catch(int e) {
		cout<<"Exception number " << e << " occurred.";
	}
}

//REVIEW: Is there a way to simulate "neighbor finding" in a more efficient way?
void Network::ComputeNodeNeighborhood(NetworkNode &n) {
	double dist;
	for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
		dist = n.GetDistanceToNetworkNode(*it);
		if(dist <= n.GetSquaredRange()) {
			NetworkNode::Neighbor nghb(&(*it), dist);
			n.AddNeighbor(nghb);
		}
		if(dist <= it->GetSquaredRange()) {
			NetworkNode::Neighbor nghb(&(*it), dist);
			n.AddNeighbor(nghb);
		}
	}
}

void Network::InitializeLookupService() {
	for(auto it = NetworkNodes.begin(); it != NetworkNodes.end(); ++it) {
		LookupService.AddNode(it->Name, it->GetLocation());
		it->SetLookup(&LookupService);
	}
}

void Network::ComputeNetworkDiameter() {
   double minX = NetworkNodes[0].GetLocation().x(),
		  minY = NetworkNodes[0].GetLocation().y(),
	      minZ = NetworkNodes[0].GetLocation().z(),
		  maxX = 0,
		  maxY = 0,
		  maxZ = 0;
   for(auto iter = NetworkNodes.begin(); iter != NetworkNodes.end(); ++iter) {
	   Point_3 tmpPoint = iter->GetLocation();
	   if(tmpPoint.x() < minX)
		   minX = tmpPoint.x();
	   else if(tmpPoint.x() > maxX)
		   maxX = tmpPoint.x();
	   if(tmpPoint.y() < minY)
		   minY = tmpPoint.y();
	   else if(tmpPoint.y() > maxY)
		   maxY = tmpPoint.y();
	   if(tmpPoint.z() < minZ)
		   minZ = tmpPoint.z();
	   else if(tmpPoint.z() > maxZ)
		   maxZ = tmpPoint.z();
   }
   double distX = maxX - minX,
	      distY = maxY - minY,
		  distZ = maxZ - minZ;
   if(distX >= distY && distX >= distZ)
	   NetworkDiameter = distX;
   else if(distY >= distX && distY >= distZ)
	   NetworkDiameter = distY;
   else if(distZ >= distX && distZ >= distY)
	   NetworkDiameter = distZ;
}

bool Network::AreNodesConnected(const string &node1, const string &node2, vector<string> &visitedNodes) {
	NetworkNode start;
	GetNodeByName(node1, start);
	if(start.HasNeighbor(node2))
		return true;

	visitedNodes.push_back(node1);
	bool foundPath = false;
	for(auto iter = start.Neighbors.begin(); !foundPath && iter != start.Neighbors.end(); ++iter) {
		bool visited = false;
		for(size_t i = 0; i < visitedNodes.size(); ++i) {
			if(visitedNodes[i].compare(iter->Node->Name) == 0) {
				visited = true;
				break;
			}
		}
		if(!visited)
			foundPath |= AreNodesConnected(iter->Node->Name, node2, visitedNodes);
	}
	return foundPath;
}

void Network::RemoveIsolatedSegments1() {
	if(NetworkNodes.size() > 1) {
		vector<vector<string>>  netSegments;
		vector<string>			newseg;
		newseg.push_back(NetworkNodes[0].Name);
	    netSegments.push_back(newseg);

		for(auto iter = NetworkNodes.begin(); iter != NetworkNodes.end(); ++iter) {
			bool foundPod = false;
			string currentNode = iter->Name;

			//check 1-hop neighborhoods first
			for(size_t j=0; j < netSegments.size(); j++) { 
				for(size_t k = 0; k < netSegments[j].size(); k++) {
					if(netSegments[j][k].compare(iter->Name) == 0)
						foundPod = true;
					else if(!foundPod && iter->HasNeighbor(netSegments[j][k])) {
						netSegments[j].push_back(iter->Name);
						foundPod = true;
					}
					if(foundPod)
						break;
				}
			}

			if(!foundPod) {
				for(size_t n=0; !foundPod && n < netSegments.size(); n++) {
					vector<string> visitedNodes;
					clock_t t_begin = clock();
					if(AreNodesConnected(currentNode, netSegments[n][0], visitedNodes)) {
						netSegments[n].push_back(currentNode);
						foundPod = true;
						break;
					}
					double elapsed_secs = double(clock() - t_begin)/CLOCKS_PER_SEC;
					cout<<"Took "<<elapsed_secs << "to determine if nodes are connected" << endl;
				}
			}

			//Add new pod with new node
			if(!foundPod) {
				vector<string> newseg;
				newseg.push_back(currentNode);
				netSegments.push_back(newseg);
			}
		}

		uint maxSize = 0;
		for(size_t i = 0; i < netSegments.size(); i++) {
			if(maxSize < netSegments[i].size())
				maxSize = netSegments[i].size();
		}

		for(size_t i = 0; i < netSegments.size(); i++) {
			if(netSegments[i].size() != maxSize) {
				for(size_t j=0; j<netSegments[i].size(); j++) {
					RemoveNodeByName(netSegments[i][j]);
				}
			}
		}
	}
}

//SHOULD BE FASTER
void Network::RemoveIsolatedSegments2() {
	if(NetworkNodes.size() > 1) {
		vector<vector<string>>  netSegments;
		vector<string>			newseg;
		newseg.push_back(NetworkNodes[0].Name);
		NetworkNodes[0].GetAllNodesConnectedToNode(newseg);
	    netSegments.push_back(newseg);

		for(auto iter = NetworkNodes.begin() ; iter != NetworkNodes.end(); ++iter) {
		  bool foundPod = false;
		  for(size_t j=0; j < netSegments.size(); j++) { 
			for(size_t k = 0; k < netSegments[j].size(); k++) {
				if(netSegments[j][k].compare(iter->Name) == 0) {
					foundPod = true;
					break;
				}
			}
			if(foundPod)
				break;
		  }
		  if(!foundPod) {
			vector<string> newseg;
			newseg.push_back(iter->Name);
			iter->GetAllNodesConnectedToNode(newseg);
			netSegments.push_back(newseg);
	      }
		}

		uint maxSize = 0;
		for(size_t i = 0; i < netSegments.size(); i++) {
			if(maxSize < netSegments[i].size())
				maxSize = netSegments[i].size();
		}

		for(size_t i = 0; i < netSegments.size(); i++) {
			if(netSegments[i].size() != maxSize) {
				for(size_t j=0; j<netSegments[i].size(); j++) {
					RemoveNodeByName(netSegments[i][j]);
				}
			}
		}
	}
}

void Network::GenerateRandomNetwork(double Max_x, double Max_y, double Max_z, double nodeRange, uint nodeCount) {
	for(uint i=0; i < nodeCount; ++i){
		stringstream nodeName;
		nodeName<<"Node"<<i;
		NetworkNode net(nodeName.str(), Point_3(Max_x * ((double)rand()/RAND_MAX), Max_y * ((double)rand()/RAND_MAX), Max_z * ((double)rand()/RAND_MAX)), nodeRange * nodeRange, 0);
		AddNode(net);
	}
}

bool Network::ReadNetworkFromFile(const string &fileName) {
	ifstream in_net;
	string	 line;
	char	 removeChar[] = "()r= ";
	try {
		in_net.open(fileName);
		if(in_net.is_open()){
			int nodeCount = 1;
			while(!in_net.eof()){
				getline(in_net,line);
				stringstream s;
				s<<"Node"<<nodeCount;
				NetworkNode nd;
				nd.Name = s.str();			  
				for(uint i = 0; i < strlen(removeChar); ++i)
					line.erase (remove(line.begin(), line.end(), removeChar[i]), line.end());
				if(nd.SetLocationAndSquaredRange(line)) {
					AddNode(nd);
					nodeCount++;
				}
			}
			in_net.close();
			return true;
		}
	}
	catch(int e) {
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

bool Network::ReadNetworkFromGEFX(const string &gexfFileName, double range){
	//NOTE: The *.gefx file does not have the radius defined so the MAX_RANGE will be used. Make sure this is as desired.
	ifstream in_net;
	string	 line,
			 nodeName="";
	int		 idx1, idx2;
	try {
		in_net.open(gexfFileName);
		if(in_net.is_open()){
			while(!in_net.eof()){
				getline(in_net,line);
				idx1 = line.find("<node ");
				idx2 = line.find("position");
				if(idx1 >=0) {
					vector<string> nodepieces = General::TokenizeString(line, C_QUOTE);
					if(nodepieces.size() > 2) 
						nodeName = nodepieces[1];
				}
				else if(idx2 >= 0 && !nodeName.empty()) {
					NetworkNode nd;
					nd.Name = nodeName;
					if(nd.SetLocationFromGEXFStr(line)) {
						nd.SetSquaredRange(range * range);
						AddNode(nd);
					}
					
					nodeName = "";
				}
			}
		}
		return true;
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
		return false;
	}
}
/*void Network<Point_3, Polyhedron>::ComputeBoundaryNodes() {
	vector<NetworkNode<Point_3>> neighborhood;
	for(size_t i = 0; i < NetworkNodes.size(); ++i) {
		if(!NetworkNodes[i].IsBoundary()) {
			if(NetworkNodes[i].Neighbors.size() <= 2)
				NetworkNodes[i].AddProp(NodeInfo::BOUNDARY);
			else {
				neighborhood.clear();
				for(auto iter = NetworkNodes[i].Neighbors.begin(); iter !=  NetworkNodes[i].Neighbors.end(); ++iter) {
					NetworkNode<Point_3> tmp;
					GetNodeByName(iter->first, tmp);
					neighborhood.push_back(tmp);
				}

				for(size_t j = 0; j < neighborhood.size() - 1; ++j) {
					for(size_t k = j + 1; k < neighborhood.size(); ++k) {
						if(IsNodeOnBoundary(NetworkNodes[i], neighborhood[j], neighborhood[k]))
							NetworkNodes[i].AddProp(NodeInfo::BOUNDARY);
					}
				}
			}
		}
    }
	for(size_t k = 0; k < VirtualNodes.size(); ++k) {
		stringstream s;
		s<<"Sph"<<k;
		VirtualNodes[k].SetName(s.str());
	}
}*/

/*

template<>
void Network<Point_3, Polyhedron>::FindSpheresOfRaidusRHingedOnPoints(const Point_3 &p1, const Point_3 &p2, const Point_3 &p3, double maxR, SK::Sphere_3 &sph1, SK::Sphere_3 &sph2) {
	SK::Point_3 SK_p1(p1.x(), p1.y(), p1.z()),
	            SK_p2(p2.x(), p2.y(), p2.z()),
		        SK_p3(p3.x(), p3.y(), p3.z()); //convert to spherical coordinates

	//Finding the sphere with raidus R hinged on 3 points can give 3 types of results:
	//1. No Sphere if the points are collinear
	//2. 1 Sphere if the circle that the three points determine has raidus R
	//3. 2 Spheres if the diameter of the circle that the three points determine has raidus < R
	if(!CGAL::collinear(p1, p2, p3) && (p1 != p2) && (p1 != p3) && (p2 != p3)) {
		SK::Circle_3 circumCircle(SK_p1, SK_p2, SK_p3);
		double radius = CGAL::to_double(circumCircle.squared_radius());
		SK::Point_3 center = circumCircle.center();
		Point_3 centerC(CGAL::to_double(center.cartesian(0)), CGAL::to_double(center.cartesian(1)), CGAL::to_double(center.cartesian(2)));
		if(abs(radius - maxR) < 0.001 * maxR)//|radius of circumcircle| <= 1% of desired radius should be good enough
			sph1 = SK::Sphere_3(circumCircle.center(), maxR);
		else { 
			//Find the two points in the line perpendicular to the circumcircle at the center of it, such that they have a distance of R from the center.
			//To find the points in the line, find the intersection points between the line and the sphere of cc center and raidus R.
			SK::Intersect_3 inter;
			SK::Line_3 ln = circumCircle.supporting_plane().perpendicular_line(circumCircle.center());
			SK::Sphere_3 sph(circumCircle.center(), abs(maxR - radius));
			 //create a functor to compare theta-coordinates on sphere s1
			//SK::Compare_theta_z_3 cmp(sph);
		    std::vector< CGAL::Object > intersections;
            inter(sph,ln,std::back_inserter(intersections));
			if(intersections.size() == 2) {
				std::pair<SK::Circular_arc_point_3,unsigned> CA_intersectionP1 = CGAL::object_cast< std::pair<SK::Circular_arc_point_3,unsigned> >(intersections[0]),
															 CA_intersectionP2 = CGAL::object_cast< std::pair<SK::Circular_arc_point_3,unsigned> >(intersections[1]);
				SK::Point_3 SK_intersectionP1(CGAL::to_double(CA_intersectionP1.first.x()), CGAL::to_double(CA_intersectionP1.first.y()), CGAL::to_double(CA_intersectionP1.first.z())),
					        SK_intersectionP2(CGAL::to_double(CA_intersectionP2.first.x()), CGAL::to_double(CA_intersectionP2.first.y()), CGAL::to_double(CA_intersectionP2.first.z()));
				
				sph1 = SK::Sphere_3(SK_intersectionP1, maxR);	
			    sph2 = SK::Sphere_3(SK_intersectionP2, maxR);
				Point_3 t1(CGAL::to_double(SK_intersectionP1.cartesian(0)), CGAL::to_double(SK_intersectionP1.cartesian(1)), CGAL::to_double(SK_intersectionP1.cartesian(2))),
					    t2(CGAL::to_double(SK_intersectionP2.cartesian(0)), CGAL::to_double(SK_intersectionP2.cartesian(1)), CGAL::to_double(SK_intersectionP2.cartesian(2)));
				double radius1 = CGAL::to_double(sph1.squared_radius()),
					radius2 = CGAL::to_double(sph2.squared_radius());

			}
			else
				cout<<"WARNING: Expecting 2 points in the intersection between a line and a sphere, but there are " << intersections.size() << endl;
		}
	}
}

template<>
bool Network<Point_3, Polyhedron>::IsNodeOnBoundary(NetworkNode<Point_3> &n1, NetworkNode<Point_3> &n2, NetworkNode<Point_3> &n3) {
	SK::Sphere_3 sph1,
		         sph2;
	double halfSquaredRange = n1.GetSquaredRange()/2;

	FindSpheresOfRaidusRHingedOnPoints(n1.GetLocation(), n2.GetLocation(), n3.GetLocation(), n1.GetSquaredRange(), sph1, sph2);
	if(sph1.squared_radius() > 0 && IsSphereHingedOnNodesEmpty(n1, n2, n3, sph1)) {
		Point_3 p_tmp(CGAL::to_double(sph1.center().x()), CGAL::to_double(sph1.center().y()), CGAL::to_double(sph1.center().z()));
		NetworkNode<Point_3> pSph1("", p_tmp, n1.GetSquaredRange());
		pSph1.AddNeighbor(pair<string, double>(n1.GetName(), n1.GetSquaredRange()));
	    pSph1.AddNeighbor(pair<string, double>(n2.GetName(), n1.GetSquaredRange()));
		pSph1.AddNeighbor(pair<string, double>(n3.GetName(), n1.GetSquaredRange()));
		AddVirtualNode(pSph1);
		return true;
	}
	if(sph2.squared_radius() > 0 && IsSphereHingedOnNodesEmpty(n1, n2, n3, sph2)) {
		Point_3 p_tmp(CGAL::to_double(sph2.center().x()), CGAL::to_double(sph2.center().y()), CGAL::to_double(sph2.center().z()));
		NetworkNode<Point_3> pSph2("", p_tmp, n1.GetSquaredRange());
		pSph2.AddNeighbor(pair<string, double>(n1.GetName(), halfSquaredRange));
		pSph2.AddNeighbor(pair<string, double>(n2.GetName(), halfSquaredRange));
		pSph2.AddNeighbor(pair<string, double>(n3.GetName(), halfSquaredRange));
		AddVirtualNode(pSph2);
		return true;
	}

	return false;
	----- SK::Sphere_3 sph1,
		         sph2,
				 sph3,
				 sph4;
	double       NetworkRadius = NetworkDiameter/2;

	FindSpheresOfRaidusRHingedOnPoints(n1.GetLocation(), n2.GetLocation(), n3.GetLocation(), n1.GetSquaredRange(), sph1, sph2);
	FindSpheresOfRaidusRHingedOnPoints(n1.GetLocation(), n2.GetLocation(), n3.GetLocation(), NetworkRadius, sph3, sph4);
	double C1C3 = NetworkRadius - CGAL::to_double(CGAL::squared_distance(sph1.center(), sph3.center())),
		   C1C4 = NetworkRadius - CGAL::to_double(CGAL::squared_distance(sph1.center(), sph4.center())),
		   C2C3 = NetworkRadius - CGAL::to_double(CGAL::squared_distance(sph2.center(), sph3.center())),
		   C2C4 = NetworkRadius - CGAL::to_double(CGAL::squared_distance(sph2.center(), sph4.center()));

	bool emptySph3 = IsSphereHingedOnNodesEmpty(n1, n2, n3, sph3);
	bool emptySph4 = IsSphereHingedOnNodesEmpty(n1, n2, n3, sph4);

	if(sph1.squared_radius() > 0) {
		if(IsSphereHingedOnNodesEmpty(n1, n2, n3, sph1) && !((C1C3 < 0 && emptySph3) || (C1C4 < 0 && emptySph4)))
			return true;
	}
	if(sph2.squared_radius() > 0) {
		if(IsSphereHingedOnNodesEmpty(n1, n2, n3, sph2) && !((C2C3 < 0 && emptySph3) || (C2C4 < 0 && emptySph4)))
			return true;
	}

	return false; -------
}

template<>
bool Network<Point_3, Polyhedron>::IsSphereHingedOnNodesEmpty(NetworkNode<Point_3> &n1, NetworkNode<Point_3> &n2, NetworkNode<Point_3> &n3, const SK::Sphere_3 &sph) {
	std::set<pair<string, double>> allNeighbors;
	allNeighbors.insert(n1.Neighbors.begin(), n1.Neighbors.end());
	allNeighbors.insert(n2.Neighbors.begin(), n2.Neighbors.end());
	allNeighbors.insert(n3.Neighbors.begin(), n3.Neighbors.end());
	Point_3 tmpP;
	for(auto iter = allNeighbors.begin(); iter != allNeighbors.end(); iter++) {
		if(iter->first.compare(n1.GetName()) != 0 && iter->first.compare(n2.GetName()) != 0 && iter->first.compare(n3.GetName()) != 0) {
			NetworkNode<Point_3> node1;
			if(GetNodeByName(iter->first, node1)) {
				tmpP = node1.GetLocation();
				SK::Point_3 sphericalPoint(tmpP.x(), tmpP.y(), tmpP.z());
				double radius = CGAL::to_double(sph.squared_radius());
				double distance = CGAL::to_double(CGAL::squared_distance(sph.center(), sphericalPoint));
				if(sph.has_on_bounded_side(sphericalPoint) || sph.has_on_boundary(sphericalPoint))
					return false;
			}
			else
				cout<<"ERROR: Could not find node " << iter->first << endl;
		}
	}
	return true;
}*/