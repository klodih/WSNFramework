#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
	string StartNode,
		   EndNode;

	Message();
	Message(const string& start, const string& end);

	//Getters
	string GetHeader() const { return Header; };
	vector<string> GetPath() { return Path; };
	string GetAdditionalInfo() const { return AdditionalInfo; };

	//Modifiers
	void   AddToHeader(const string& h);
	void   AddNodeToPath(const string& node) { Path.push_back(node); };
	void   AddAdditionalInfo(const string& info);

private:
	string Header,
		   AdditionalInfo;
	vector<string> Path;
	uint   HeaderMaxSize,
		   AdditionalInfoMaxSize;
};

#endif //MESSAGE_H