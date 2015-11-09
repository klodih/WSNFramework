// WSNFramework.cpp : main project file.

#include "stdafx.h"
#include "simulator.h"
using namespace std;

int main()
{
	RoutingSimulator simulation1;
	simulation1.InitNetwork(GEXF_PATH + "52nodes_1Seg_R20.gexf", 20.0);
	simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node49");
    simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node77");
	simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node12");
	simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node59");
	simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node57");
	simulation1.RunRoutingSimulation(DIJKSTRA, "Node56", "Node32");

	//srand((uint)time(NULL));
	//simulation1.InitNetwork(100, 100, 100, 20, 100, true);
	//simulation1.SaveNetworkToGexf("testcolor.gexf", "edgeColor");

	//string netStr;
	//simulation1.PrintNetwork(netStr);
	//cout<<netStr<<endl;
	uint i;
	cin>>i;
    return 0;
}