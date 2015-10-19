#pragma once
#ifndef LOOKUP_H
#define LOOKUP_H

class Lookup {
public:
	Lookup();
	bool	AddNode(const string&, const Point_3&);
	bool	DeleteNode(const string&);
	bool	GetNameOfNode(const Point_3&, string&);
	bool    GetLocationOfNode(const string&, Point_3&);
	
private:
	map<string, Point_3> LookupTable;
};
#endif //LOOKUP_H