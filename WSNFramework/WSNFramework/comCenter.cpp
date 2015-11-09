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
bool ComCenter::RunDijkstra(Message &message) {
	map<string, double> dist;
	map<string, bool> visited;
	map<string, string> parentInfo;

	//init distance vector to infinite distances
	for(auto itNet = Nodes->begin(); itNet != Nodes->end(); ++itNet) {
		dist.insert(pair<string, double>(itNet->first, DBL_MAX));
		visited.insert(pair<string, bool>(itNet->first, false));
		parentInfo.insert(pair<string, string>(itNet->first, ""));
	}

	dist[message.GetStartNode()] = 0;
	Node *minDistNode;

	for(auto itNet1 = Nodes->begin(); itNet1 != Nodes->end(); ++itNet1) {
		double min = DBL_MAX;
		for(auto itNet2 = Nodes->begin(); itNet2 != Nodes->end(); ++itNet2) {
			if(!visited[itNet2->first] && dist[itNet2->first] < min)
				min = dist[itNet2->first], minDistNode = &itNet2->second;
		}
		if(minDistNode->Name == message.GetEndNode())
			return BuildPathFromDijkstra(parentInfo, message);

		visited[minDistNode->Name] = true;
		if(minDistNode->Neighbors.size() == 0)
			return false;

		for(auto itNeigh = minDistNode->Neighbors.begin(); itNeigh != minDistNode->Neighbors.end(); ++itNeigh) {
			if(!visited[itNeigh->first] && min != DBL_MAX && (min + 1 < dist[itNeigh->first])) {
				dist[itNeigh->first] = min + 1;
				parentInfo[itNeigh->first] = minDistNode->Name;
			}
		}
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
			message.Header_PrecomputedPath.push_back(it->first);
			for(int i = temp.size() - 1; i >= 0; --i)
				message.Header_PrecomputedPath.push_back(temp[i]);

			return true;
		}
	}
	  
	return false;
}