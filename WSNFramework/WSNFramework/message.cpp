#include "stdafx.h"
#include "message.h"

//-----  CONSTRUCTORS  -----
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

void   Message::AddToHeader(const string& h) {
	Header += C_CARET + h;
	HeaderMaxSize += h.size();
}

void Message::AddAdditionalInfo(const string& info) {
	AdditionalInfo += C_CARET + info;
}