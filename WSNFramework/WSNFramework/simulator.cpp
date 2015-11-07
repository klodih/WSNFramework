#include "stdafx.h"
#include "simulator.h"
#include "generalFunctions.h"

RoutingSimulator::RoutingSimulator() :
	SimNetwork() {}

bool RoutingSimulator::InitNetwork(double netLength, double netWidth, double netHeight, double nodeRange, uint nodeCount, bool keepConnectedSegmentsOnly) {
	GenerateRandomNetwork(netLength, netWidth, netHeight, nodeRange, nodeCount);
	if(keepConnectedSegmentsOnly)
		SimNetwork.KeepLargestConnectedSegment();
	return (SimNetwork.GetNodeCount() >= 0.5 * nodeCount);
}

bool RoutingSimulator::InitNetwork(const string &networkDefFile, bool keepConnectedSegmentsOnly) {
	if(ReadNetworkFromFile(networkDefFile)) {
		if(keepConnectedSegmentsOnly)
			SimNetwork.KeepLargestConnectedSegment();
		return true;
	}
	return false;
}

bool RoutingSimulator::InitNetwork(const string &networkDefFile, double squaredRange, bool keepConnectedSegmentsOnly) {
	if(ReadNetworkFromGEFX(networkDefFile, squaredRange)) {
		if(keepConnectedSegmentsOnly)
			SimNetwork.KeepLargestConnectedSegment();
		return true;
	}
	return false;
}

void RoutingSimulator::GenerateRandomNetwork(double netLength, double netWidth, double netHeight, double nodeRange, uint nodeCount) {
	for(uint i=0; i < nodeCount; ++i){
		stringstream nodeName;
		nodeName<<"Node"<<i;
		Node net(nodeName.str(), Point_3(netLength * ((double)rand()/RAND_MAX), netWidth * ((double)rand()/RAND_MAX), netHeight * ((double)rand()/RAND_MAX)), nodeRange * nodeRange, 0);
		SimNetwork.AddNode(net);
	}
}

bool RoutingSimulator::ReadNetworkFromFile(const string &fileName) {
	ifstream in_net;
	string	 line;
	char	 removeChar[] = "()r= ";
	try {
		in_net.open(fileName);
		if(in_net.is_open()){
			int nodeCount = 1;
			while(!in_net.eof()){
				getline(in_net,line);
				Node nd("Node" + nodeCount);			  
				for(uint i = 0; i < strlen(removeChar); ++i)
					line.erase (remove(line.begin(), line.end(), removeChar[i]), line.end());
				if(nd.SetLocationAndSquaredRangeFromStr(line)) {
					SimNetwork.AddNode(nd);
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

bool RoutingSimulator::ReadNetworkFromGEFX(const string &gexfFileName, double range){
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
					Node nd;
					nd.Name = nodeName;
					if(nd.SetLocationFromGEXFStr(line)) {
						nd.SetSquaredRange(range * range);
						SimNetwork.AddNode(nd);
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

void RoutingSimulator::PrintNetwork(string &netStr) const {
	SimNetwork.PrintNetwork(netStr);
}

void RoutingSimulator::SaveNetworkToGexf(bool printBoundary, bool path) {
	Gephi::Gexf_SaveNetwork(GEXF_PATH + "test2.gexf", "Graph1", SimNetwork, printBoundary, path);
}