#include "stdafx.h"
#include "network.h"
#include "generalFunctions.h"

//-----  CONSTRUCTOR -----
Network::Network() :
  Nodes(),
  VirtualNodes(),
  NetworkDiameter(0),
  COM(&Nodes) { 
  }

//-----  GETTERS -----
bool Network::GetNodeByName(const string& nodeNm, Node *node) {
	auto it = Nodes.find(nodeNm);
	if(it != Nodes.end()) {
		node = & it->second;
		return true;
	}
	return false;
}

//-----  MODIFIERS -----
void Network::ComputeNetworkDiameter() {
   double minX = Nodes[0].GetLocation().x(),
		  minY = Nodes[0].GetLocation().y(),
	      minZ = Nodes[0].GetLocation().z(),
		  maxX = 0,
		  maxY = 0,
		  maxZ = 0;
   for(auto iter = Nodes.begin(); iter != Nodes.end(); ++iter) {
	   Point_3 tmpPoint = iter->second.GetLocation();
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

void Network::AddNode(Node &n) {
	auto it = Nodes.find(n.Name);
	if(it == Nodes.end() && n.GetLocation().x() >= 0 && n.GetLocation().y() >= 0 && n.GetLocation().z() >= 0) {
		n.SetLookup(&LookupService);
		n.SetCOM(&COM);
		ComputeNodeNeighborhood(n);
		Nodes.insert(pair<string, Node>(n.Name, n));
		LookupService.AddNode(n.Name, n.GetLocation());
	}
}

void Network::AddVirtualNode(const Node &n) {
	auto it = VirtualNodes.find(n.Name);
	if(it != VirtualNodes.end())
		VirtualNodes.insert(pair<string, Node>(n.Name, n));
}

bool Network::RemoveNode(Node &n){
	try {
		auto it = Nodes.find(n.Name);
		if(it != Nodes.end()) {
			RemoveNodeAsNeighbor(n);
			Nodes.erase(it);
			LookupService.DeleteNode(n.Name);
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}

	return false;
}

bool Network::RemoveNodeByName(const string &name, bool removeAsNeighbor) {
	try {
		auto it = Nodes.find(name);
		if(it != Nodes.end()) {
			if(removeAsNeighbor)
				RemoveNodeAsNeighbor(it->second);
			Nodes.erase(it);
			LookupService.DeleteNode(name);
		}
	}
	catch(int e){
		cout<<"Exception number " << e << " occurred.";
	}
	return false;
}

void Network::ComputeNodeNeighborhood(Node &n) {
	for(auto it = Nodes.begin(); it != Nodes.end(); ++it) {
		if(!it->second.HasNeighborInNeighborTable(n.Name))
			it->second.AddMutualNeighborIfInRange(n);
	}
}

bool Network::RemoveNodeAsNeighbor(Node &n) {
	for(auto it = Nodes.begin(); it != Nodes.end(); ++it) {
		if(it->second.HasNeighborInNeighborTable(n.Name)) {
			if(!it->second.RemoveNeighbor(n.Name))
				return false;
		}
	}
	return true;
}

void Network::KeepLargestConnectedSegment() {
	RemoveIsolatedNodes();
	RemoveIsolatedSegments();
}

void Network::RemoveIsolatedNodes() {
	try {
		for(auto it = Nodes.begin(); it != Nodes.end(); ++it) {
			if(it->second.Neighbors.size() == 0 && RemoveNodeAsNeighbor(it->second))
				it = Nodes.erase(it);
		}
	}
	catch(int e) {
		cout<<"Exception number " << e << " occurred.";
	}
}

void Network::GetAllNodesConnectedToNode(Node &n, vector<string> &connectednodes) {
	for(auto n_it = n.Neighbors.begin(); n_it != n.Neighbors.end(); ++n_it) {
		bool neighborExists = false;
		for(auto v_it = connectednodes.begin(); v_it != connectednodes.end(); ++v_it) {
			if(n_it->first == *v_it) {
				neighborExists = true;
				break;
			}
		}
		if(!neighborExists) {
			connectednodes.push_back(n_it->first);
			auto currentNode = Nodes.find(n_it->first);
			if(currentNode != Nodes.end())
				GetAllNodesConnectedToNode(currentNode->second, connectednodes);
		}
	}
}

void Network::RemoveIsolatedSegments() {
	if(Nodes.size() > 1) {
		vector<vector<string>>  netSegments;
		vector<string>			newseg;
		Node &n = Nodes.begin()->second;
		newseg.push_back(n.Name);
		GetAllNodesConnectedToNode(n, newseg);
	    netSegments.push_back(newseg);

		for(auto iter = Nodes.begin() ; iter != Nodes.end(); ++iter) {
		  bool foundPod = false;
		  for(size_t j=0; j < netSegments.size(); j++) { 
			for(size_t k = 0; k < netSegments[j].size(); k++) {
				if(netSegments[j][k] == iter->second.Name) {
					foundPod = true;
					break;
				}
			}
			if(foundPod)
				break;
		  }
		  if(!foundPod) {
			vector<string> newseg;
			newseg.push_back(iter->second.Name);
			GetAllNodesConnectedToNode(iter->second, newseg);
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

void Network::ClearNodeProps() {
	for(auto iter = Nodes.begin() ; iter != Nodes.end(); ++iter)
		iter->second.ResetProp();

}

//-----  ROUTING -----
bool Network::SendMessage(Message &m, RoutingAlg alg) {
	return COM.SendMessage(m, alg);
}

//-----  INPUT/OUTPUT  -----
void Network::PrintNetwork(string &netStr) const {
	stringstream nStr;
	string nodeStr,
		   neighborStr;
	for(auto it = Nodes.begin(); it != Nodes.end(); ++it) {
		it->second.PrintNode(nodeStr);
		it->second.PrintNeighbors(neighborStr);
		nStr << nodeStr << "\n\t" << neighborStr << endl;
	}
	netStr = nStr.str();
}

/*void Network<Point_3, Polyhedron>::ComputeBoundaryNodes() {
	vector<Node<Point_3>> neighborhood;
	for(size_t i = 0; i < Nodes.size(); ++i) {
		if(!Nodes[i].IsBoundary()) {
			if(Nodes[i].Neighbors.size() <= 2)
				Nodes[i].AddProp(NodeInfo::BOUNDARY);
			else {
				neighborhood.clear();
				for(auto iter = Nodes[i].Neighbors.begin(); iter !=  Nodes[i].Neighbors.end(); ++iter) {
					Node<Point_3> tmp;
					GetNodeByName(iter->first, tmp);
					neighborhood.push_back(tmp);
				}

				for(size_t j = 0; j < neighborhood.size() - 1; ++j) {
					for(size_t k = j + 1; k < neighborhood.size(); ++k) {
						if(IsNodeOnBoundary(Nodes[i], neighborhood[j], neighborhood[k]))
							Nodes[i].AddProp(NodeInfo::BOUNDARY);
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
bool Network<Point_3, Polyhedron>::IsNodeOnBoundary(Node<Point_3> &n1, Node<Point_3> &n2, Node<Point_3> &n3) {
	SK::Sphere_3 sph1,
		         sph2;
	double halfSquaredRange = n1.GetSquaredRange()/2;

	FindSpheresOfRaidusRHingedOnPoints(n1.GetLocation(), n2.GetLocation(), n3.GetLocation(), n1.GetSquaredRange(), sph1, sph2);
	if(sph1.squared_radius() > 0 && IsSphereHingedOnNodesEmpty(n1, n2, n3, sph1)) {
		Point_3 p_tmp(CGAL::to_double(sph1.center().x()), CGAL::to_double(sph1.center().y()), CGAL::to_double(sph1.center().z()));
		Node<Point_3> pSph1("", p_tmp, n1.GetSquaredRange());
		pSph1.AddNeighbor(pair<string, double>(n1.GetName(), n1.GetSquaredRange()));
	    pSph1.AddNeighbor(pair<string, double>(n2.GetName(), n1.GetSquaredRange()));
		pSph1.AddNeighbor(pair<string, double>(n3.GetName(), n1.GetSquaredRange()));
		AddVirtualNode(pSph1);
		return true;
	}
	if(sph2.squared_radius() > 0 && IsSphereHingedOnNodesEmpty(n1, n2, n3, sph2)) {
		Point_3 p_tmp(CGAL::to_double(sph2.center().x()), CGAL::to_double(sph2.center().y()), CGAL::to_double(sph2.center().z()));
		Node<Point_3> pSph2("", p_tmp, n1.GetSquaredRange());
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
bool Network<Point_3, Polyhedron>::IsSphereHingedOnNodesEmpty(Node<Point_3> &n1, Node<Point_3> &n2, Node<Point_3> &n3, const SK::Sphere_3 &sph) {
	std::set<pair<string, double>> allNeighbors;
	allNeighbors.insert(n1.Neighbors.begin(), n1.Neighbors.end());
	allNeighbors.insert(n2.Neighbors.begin(), n2.Neighbors.end());
	allNeighbors.insert(n3.Neighbors.begin(), n3.Neighbors.end());
	Point_3 tmpP;
	for(auto iter = allNeighbors.begin(); iter != allNeighbors.end(); iter++) {
		if(iter->first == n1.GetName() && iter->first == n2.GetName() && iter->first == n3.GetName()) {
			Node<Point_3> node1;
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