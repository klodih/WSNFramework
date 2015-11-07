#include "stdafx.h"
#include "message.h"

//-----  CONSTRUCTORS  -----
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