const char C_CARET = '^';
const char C_COMMA = ',';
const char C_QUOTE = '/"';

enum NodeInfo {
	START		 = 0x01,
	LOCALMINIMA  = 0x02,
	BOUNDARY	 = 0x04,
	INPATH		 = 0x08,
	DESTINATION	 = 0x10,
	SP			 = 0x20,
	NORMAL       = 0x40
};

enum RoutingAlg {
	PURE_GREEDY,
	PURE_RANDOM,
	DIJKSTRA,
	NONE
};

const std::string GEXF_PATH = "C:\\Users\\Klodi\\Documents\\Thesis Documents\\Implementation and Analysis\\GEXF_GRAPHS\\";

const static double NODEWIDTH  = 4,
	                NODEHEIGHT = 4,
					NODEDEPTH  = 4;
const static int NODE_END_INDEX = 4;
const static std::string BLUE_RGB = "b=\"250\" g=\"206\" r=\"135\"",     //sky blue
						 DESTBLUE_RGB = "b=\"205\" g=\"0\" r=\"0\"",		//medium blue
						 LAWNGREEN_RGB = "b=\"0\" g=\"252\" r=\"124\"",
						 FORESTGREEN_RGB = "b=\"34\" g=\"139\" r=\"34\"",
						 RED_RGB   = "b=\"0\" g=\"0\" r=\"255\"",
						 BLACK_RGB = "b=\"0\" g=\"0\" r=\"0\"",
						 MAGENTA4_RGB = "b=\"139\" g=\"0\" r=\"139\"",
						 GOLD_RGB = "b=\"215\" g=\"215\" r=\"0\"",
						 ORANGE_RGB = "b=\"255\" g=\"147\" r=\"0\"",
						 GREY_RGB = "b=\"132\" g=\"132\" r=\"132\"";