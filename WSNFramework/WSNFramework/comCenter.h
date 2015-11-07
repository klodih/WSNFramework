#pragma once
#ifndef COMCENTER_H
#define COMCENTER_H

#include "node.h"

class Node;

typedef Node* NodePtr;
typedef map<string, Node> NodeSet;

class ComCenter {
	public:
		ComCenter(NodeSet *n);

		bool SendMessage(Message &m, RoutingAlg routingType);

	private:
		NodeSet *Nodes;
		bool RunDijkstra(Message &m);
		bool BuildPathFromDijkstra(const map<string, string> &parentInfo, Message &m);
};

#endif