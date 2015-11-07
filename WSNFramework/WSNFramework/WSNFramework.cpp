// WSNFramework.cpp : main project file.

#include "stdafx.h"
#include "simulator.h"
using namespace std;

int main()
{
	RoutingSimulator simulation1;
	simulation1.InitNetwork(100, 100, 100, 20, 100);

	string netStr;
	simulation1.PrintNetwork(netStr);
	cout<<netStr<<endl;

	uint i;
	cin>>i;
    return 0;
}