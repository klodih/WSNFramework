// WSNFramework.cpp : main project file.

#include "stdafx.h"
#include "network.h"
using namespace std;

int main()
{
	Network myNetwork;
	myNetwork.InitNetwork(50, 50, 50, 50, 8);
	for(auto it = myNetwork.NetworkNodes.begin(); it != myNetwork.NetworkNodes.end(); ++it)
		cout<< it->Name << " (" << it->GetLocation().x() << "," << it->GetLocation().y() << "," << it->GetLocation().z() << ")" << endl;
	uint i;
	cin>>i;
    return 0;
}
