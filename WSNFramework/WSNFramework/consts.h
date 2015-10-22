const char C_CARET = '^';
const char C_COMMA = ',';
const char C_QUOTE = '/"';

enum NodeInfo {
	START		 = 0x01,
	LOCALMINIMA  = 0x02,
	BOUNDARY	 = 0x04,
	ISPATH		 = 0x08,
	DESTINATION	 = 0x10,
	SP			 = 0x20
};

enum RoutingAlg {
	PURE_GREEDY,
	PURE_RANDOM,
	DIJKSTRA
};