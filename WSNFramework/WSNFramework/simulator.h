#pragma once
#ifndef ROUTINGSIMULATOR_H
#define ROUTINGSIMULATOR_H

#include "node.h"
#include "network.h"
#include "gephi.h"

class RoutingSimulator {
public:
	//Constructors
	RoutingSimulator();

	//Network Init
	bool InitNetwork(double netLength, double netWidth, double netHeight, double nodeRange, uint nodeCount, bool keepConnectedSegmentsOnly = true);
	bool InitNetwork(const string &networkDefFile, bool keepConnectedSegmentsOnly = true);
	bool InitNetwork(const string &networkDefFile, double squaredRange, bool keepConnectedSegmentsOnly = true);
    void PrintNetwork(string &netStr) const;
	void SaveNetworkToGexf(const string &fileName, const string& graphName, bool printBoundary = false, bool path = true);
	bool RunRoutingSimulation(RoutingAlg algType);
	bool RunRoutingSimulation(RoutingAlg algType, const string &startNode, const string &endNode);

private:
	Network SimNetwork;
	uint	SimCount;

	//Network Creation
	void GenerateRandomNetwork(double netLength, double netWidth, double netHeight, double nodeRange, uint nodeCount);
	bool ReadNetworkFromFile(const string &fileName);
    bool ReadNetworkFromGEFX(const string &gexfFileName, double range);
};

#endif