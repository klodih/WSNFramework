#include "stdafx.h"
#include "message.h"

//-----  CONSTRUCTORS  -----
Message::Message(const string& id, const string& start, const string& end, RoutingAlg alg) :
    MessageID(id),
	StartNode(start),
	EndNode(end),
	RouteMode(alg) {}

string Message::PrintMessage() {
	stringstream s;
	s << "MESSAGE " << MessageID << endl;
	s << "\tStart = " << StartNode << "\n\tDestination = " << EndNode << endl;
	if(Path.size() > 0) {
		s << "\tMessage Path = ";
		for(auto it = Path.begin(); it != Path.end(); ++it) 
			s << *it << ", ";
		s << endl;
	}
	return s.str();
}