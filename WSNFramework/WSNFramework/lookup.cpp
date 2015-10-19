#include "stdafx.h"
#include "lookup.h"

Lookup::Lookup() {
}

bool  Lookup::AddNode(const string& name, const Point_3& loc) {
	auto it = LookupTable.find(name);
	if(it == LookupTable.end()) {
		LookupTable.insert(pair<string, Point_3>(name, loc));
		return true;
	}
	else
		return false;
}

bool  Lookup::DeleteNode(const string& nodeName) {
	auto it = LookupTable.find(nodeName);
	if(it != LookupTable.end()) {
		LookupTable.erase(it);
		return true;
	}
	else
		return false;
}

bool  Lookup::GetNameOfNode(const Point_3& loc, string& name) {
	for (auto it=LookupTable.begin(); it != LookupTable.end(); ++it) {
		if(it->second == loc) {
			name = it->first;
			return true;
		}
	}
	return false;
}

bool Lookup::GetLocationOfNode(const string& name, Point_3& loc) {
	auto it = LookupTable.find(name);
	if(it != LookupTable.end()) {
		loc = it->second;
		return true;
	}
	return false;
}