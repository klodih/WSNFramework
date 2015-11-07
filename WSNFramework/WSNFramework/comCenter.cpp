#include "stdafx.h"
#include "comCenter.h"

ComCenter::ComCenter(NodeSet *n) {  Nodes = n; }
	
bool ComCenter::SendMessage(Message &message, RoutingAlg routingType) {
	if(routingType == DIJKSTRA && !RunDijkstra(message)) {
		cout<<"ERROR: Dijkstra Algorithm failed to run!"<<endl;
		return false;
	}

	string nextNode;
	auto startNode = Nodes->find(message.GetStartNode());
	auto destinationNode = Nodes->find(message.GetEndNode());

	if(startNode != Nodes->end() && destinationNode != Nodes->end()) {
		auto it = startNode;
		while(it != Nodes->end() && it != destinationNode) {
			it->second.RouteMessage(message, routingType, nextNode);
			it = Nodes->find(nextNode);
		}
		if(it == destinationNode)
			return it->second.ReceiveMessage(message);
		else if(it == Nodes->end())
			cout<<"No wroute found. Message dropped." <<endl;
	}
	else
		cout<<"ERROR: StartNode or DestinationNode does not exist" << endl;
	return false;
}

//Dijsktra is a special case in this project. It is the only algorithm that is allowed to have a global view of the network topology
//As such, I could have implemented it so that the neighbor tables of the nodes are the whole network, but this requires total change in architecture
//Instead, I will track the algorithm here, one step removed from the nodes. Each node then will just follow the pre-computed path indicated in the message
bool ComCenter::RunDijkstra(Message &message){
	map<string, double> dist;
	map<string, bool> visited;
	map<string, string> parentInfo;

	//init distance vector to infinite distances
	for(auto it = Nodes->begin(); it != Nodes->end(); ++it) {
		dist.insert(pair<string, double>(it->first, DBL_MAX));
		visited.insert(pair<string, bool>(it->first, false));
		parentInfo.insert(pair<string, string>(it->first, ""));
	}

	dist[message.GetStartNode()] = 0;
	string minDistNode = "";

	for(auto it1 = Nodes->begin(); it1 != Nodes->end(); ++it1) {
		double min = DBL_MAX;
		for(auto it2 = Nodes->begin(); it2 != Nodes->end(); ++it2) {
			if(!visited[it2->first] && dist[it2->first] < min)
				min = dist[it2->first], minDistNode = it2->first;
		}
		if(minDistNode.compare(message.GetEndNode()))
			return BuildPathFromDijkstra(parentInfo, message.GetStartNode(), message.GetEndNode(), message);

		visited[minDistNode] = true;
		NodePtr ndp = &(it1->second);
		if(ndp->Neighbors.size() > 0) {
			for(auto it3 = ndp->Neighbors.begin(); it3 != ndp->Neighbors.end(); ++it3) {
				if(!visited[it3->first] && min != DBL_MAX && (min + 1 < dist[it3->first])) {
					dist[it3->first] = min + 1;
					parentInfo[it3->first] = minDistNode;
				}
			}
		}
		else
			return false;
	}
	return true;
}

bool ComCenter::BuildPathFromDijkstra(const map<string, string> &parentInfo, Message &message) {
	uint nodeCount = Nodes->size();
	auto startIt = parentInfo.find(message.GetStartNode()),
		 it = parentInfo.find(message.GetEndNode());

	vector<string> temp;
	if(startIt != parentInfo.end()) {
		while(it != parentInfo.end() && it != startIt) {
			temp.push_back(it->first);
			it = parentInfo.find(it->second);
		}

		if(it == startIt) {
			message.PrecomputedPath.push_back(it->first);
			for(auto reverseIt = temp.end() - 1; reverseIt != temp.begin() - 1; --reverseIt)
				message.PrecomputedPath.push_back(*reverseIt);

			return true;
		}
	}
	  
	return false;
}