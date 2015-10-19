#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
	string StartNode,
		   EndNode;

	Message();
	Message(const string& start, const string& end);
	string GetHeader() const;
	void   AddToHeader(const string& h);
	vector<string> GetPath();
	void   AddNodeToPath(const string& node);
	string GetAdditionalInfo() const;
	void   AddAdditionalInfo(const string& info);

private:
	string Header,
		   AdditionalInfo;
	vector<string> Path;
	uint   HeaderMaxSize,
		   AdditionalInfoMaxSize;
};

#endif //MESSAGE_H