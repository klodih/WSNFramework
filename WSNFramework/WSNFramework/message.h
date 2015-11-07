#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
	Message(const string& start, const string& end);
	vector<string> PrecomputedPath;

	//Getters
	string GetStartNode() const { return StartNode; };
	string GetEndNode() const { return EndNode; };
	string GetHeader() const { return Header; };
	vector<string> GetPath() { return Path; };
	string GetAdditionalInfo() const { return AdditionalInfo; };

	//Modifiers
	void   AddToHeader(const string& h);
	void   AddNodeToPath(const string& node) { Path.push_back(node); };
	void   AddNodeToPrecomputedPath(const string& node) { PrecomputedPath.push_back(node); };
	void   AddAdditionalInfo(const string& info);

private:
	string StartNode,
		   EndNode,
		   Header,
		   AdditionalInfo;
	vector<string> Path;
	uint   HeaderMaxSize,
		   AdditionalInfoMaxSize;
};

#endif //MESSAGE_H