#include "node.h"
#include "network.h"

using namespace std;

namespace Gephi {
	string Gexf_NodeColor(const Node &n, bool printBoundary = false, bool path = true);
	string Gexf_EdgeColor(const Node &n1, const Node &n2, bool printBoundary = false, bool path = true);
	string Gexf_NodeLabel(const Node &n, bool printBoundary = false, bool path = true);
	string Gexf_NodeLocation(const Node &node);
	string Gexf_NodeString(const Node &node, bool printBoundary = false, bool path = true);
	string Gexf_EdgeString(const Node &node1, const Node &node2, const string &edgeid, bool printBoundary = false, bool path = true);
	bool   Gexf_SaveNetwork(const string& filePathName, const string& graphName, Network &network, bool printBoundary = false, bool path = true);
}