#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
	Message(const string& id, const string& start, const string& end, RoutingAlg alg);
	vector<string> Header_PrecomputedPath;

	//Getters
	string GetStartNode() const { return StartNode; };
	string GetEndNode() const { return EndNode; };
	string GetID() const { return MessageID; };
	vector<string> GetPath() { return Path; };
	RoutingAlg  GetRouteMode() { return RouteMode; };

	//Modifiers
	void   AddNodeToPath(const string& node) { Path.push_back(node); };
	void   AddNodeToPrecomputedPath(const string& node) { Header_PrecomputedPath.push_back(node); };

	string PrintMessage();

private:
	string         MessageID,
		           StartNode,
			       EndNode;
    RoutingAlg	   RouteMode;
	vector<string> Path;
};

#endif //MESSAGE_H