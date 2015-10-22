// WSNFramework.cpp : main project file.

#include "stdafx.h"
#include "network.h"
using namespace std;

int main()
{
	Network myNetwork;
	myNetwork.InitNetwork(50, 50, 50, 25, 50);
	string netStr;
	myNetwork.PrintNetwork(netStr);
	cout<<netStr<<endl;
	uint i;
	cin>>i;
    return 0;
}
