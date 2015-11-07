#include "stdafx.h"
#include "gephi.h"

string Gephi::Gexf_NodeColor(const Node &node, bool printBoundary, bool path) {
	//<viz:color b="107" g="72" r="8"/>
    string colorStr = "\t\t\t\t<viz:color ";
	if(path) {
		if(node.IsProp(DESTINATION))
			colorStr += DESTBLUE_RGB;
		else if(node.IsProp(LOCALMINIMA))
			colorStr += RED_RGB;
		else if(node.IsProp(START))
			colorStr += ORANGE_RGB;
		else if(node.IsProp(INPATH))
			colorStr += FORESTGREEN_RGB;
	}

	if(node.IsProp(BOUNDARY) && printBoundary)
		colorStr += MAGENTA4_RGB;
	else if(node.IsProp(SP))
		colorStr += GREY_RGB;
	else
		colorStr += BLACK_RGB;

	colorStr += "/>\n";
	return colorStr;
}

string Gephi::Gexf_EdgeColor(const Node &node1, const Node &node2, bool printBoundary, bool path) {
	string colorStr = "\t\t\t\t<viz:color ";
	if(printBoundary && (node1.IsProp(BOUNDARY) || node2.IsProp(BOUNDARY)))
		colorStr += GREY_RGB;
	else if(path && node1.IsProp(INPATH) && node2.IsProp(INPATH))
		colorStr += LAWNGREEN_RGB;
	else 
		colorStr += BLACK_RGB;

	colorStr += "/>\n";
	return colorStr;
}

string Gephi::Gexf_NodeLabel(const Node &node, bool printBoundary, bool path) {
	string typeStr = "\" label=\"" + node.Name + "[";
	if(path) {
		if(node.IsProp(DESTINATION))
			typeStr += "D,";
		if(node.IsProp(LOCALMINIMA))
			typeStr += "LM,";
		if(node.IsProp(START))
			typeStr += "S,";
		if(node.IsProp(INPATH))
			typeStr += "P,";
	}

	if(node.IsProp(BOUNDARY) && printBoundary)
		typeStr += "B,";

    if(node.IsProp(SP))
		typeStr += "SP,";

	typeStr += "]\">\n";
	return typeStr;
}

string Gephi::Gexf_NodeLocation(const Node &node) {
	stringstream coordStr;
	coordStr << "\t\t\t\t<viz:position x=\"" << node.GetLocation().x() << "\" y=\"" << node.GetLocation().y() << "\" z=\"" << node.GetLocation().z() << "\"/>" << endl;
	return coordStr.str();
}

string Gephi::Gexf_NodeString(const Node &node, bool printBoundary, bool path) {
	string nodeStr = "\t\t\t<node id=\"" + node.Name
					+ Gexf_NodeLabel(node, printBoundary, path)
		            + Gexf_NodeColor(node, printBoundary, path)
					+ "\t\t\t\t<viz:shape value=\"sphere\"/>\n"
					+ Gexf_NodeLocation(node)
					+ "\t\t\t\t<viz:size value =\"2\"/>\n"
					+ "\t\t\t</node>\n";
	return nodeStr;
}

string Gephi::Gexf_EdgeString(const Node &node1, const Node &node2, const string &edgeid, bool printBoundary, bool path) {
	return ("\t\t\t<edge id=\"" + edgeid + "\" source=\"" + node1.Name + "\" target=\"" + node2.Name + "\">\n"
		             + Gexf_EdgeColor(node1, node2, printBoundary, path)
					 + "\t\t\t</edge>\n");
}

bool Gephi::Gexf_SaveNetwork(const string& filePathName, const string& graphName, Network &network, bool printBoundary, bool path) {
	ofstream gexfFile;
	gexfFile.open (filePathName);
	gexfFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	gexfFile << "<gexf xmlns=\"http://www.gephi.org/gexf\" xmlns:viz=\"http://www.gephi.org/gexf/viz\">" << endl;
	gexfFile << "\t<graph mode=\"static\" defaultedgetype=\"undirected\">" <<endl;

	//PRINT NODES
	gexfFile << "\t\t<nodes>"<<endl;

	for(auto it = network.Nodes.begin(); it != network.Nodes.end(); ++it)
		gexfFile << Gexf_NodeString(it->second, printBoundary, path);

	gexfFile << "\t\t</nodes>" << endl;

	//PRINT EDGES
	string printedNodeEdges = "";
	gexfFile << "\t\t<edges>" << endl;
	uint i = 0, j = 0;
	for(auto netIt = network.Nodes.begin(); netIt != network.Nodes.end(); ++netIt) {
		NodePtr currentN = &netIt->second;
		printedNodeEdges += "^" + currentN->Name + "^";
		for(auto neighIt = currentN->Neighbors.begin(); neighIt != currentN->Neighbors.end(); ++ neighIt) {
			auto it = network.Nodes.find(neighIt->first);
			if(it != network.Nodes.end()) {
				int idx = printedNodeEdges.find("^" + it->first + "^");
				if(idx < 0) {
					stringstream edgeId;
					edgeId << i << "-"<<j;
					gexfFile << Gexf_EdgeString(*currentN, it->second, edgeId.str(), printBoundary, path);
				}
			}
			j++;
		}
		i++;
	}

	gexfFile << "\t\t</edges>" << endl << "\t</graph>" << endl << "</gexf>" << endl;
	gexfFile.close();
	return true;
}