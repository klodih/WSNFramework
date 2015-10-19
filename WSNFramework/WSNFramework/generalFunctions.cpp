#include "stdafx.h"

vector<string> General::TokenizeString(const string &str, const char delimiter) {
	vector<string>	tokens;
	string			token,
					inStr = str;
	unsigned		pos = 0;
	while((pos = inStr.find(delimiter)) != string::npos) {
		token = inStr.substr(0, pos);
		tokens.push_back(token);
		inStr.erase(0, pos + 1);
	}
	tokens.push_back(inStr);
	return tokens;
}
