#include "stdafx.h"
#include "message.h"

Message::Message() :
	StartNode(),
	EndNode(),
	Header(),
	AdditionalInfo(),
	Path(), 
	HeaderMaxSize(0),
	AdditionalInfoMaxSize(0){}

Message::Message(const string& start, const string& end) :
	StartNode(start),
	EndNode(end) {}

string Message::GetHeader() const { return Header; }

void   Message::AddToHeader(const string& h) {
	Header += C_CARET + h;
	HeaderMaxSize += h.size();
}

vector<string> Message::GetPath() { return Path; }

void Message::AddNodeToPath(const string& node) {
	Path.push_back(node);
}

string Message::GetAdditionalInfo() const { return AdditionalInfo; }

void Message::AddAdditionalInfo(const string& info) {
	AdditionalInfo += C_CARET + info;
}