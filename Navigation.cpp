/// Clang 3.8 C++ 11 Compliant  
#include "Navigation.h"
#include "Node.hpp"
#include "Arc.hpp"

using namespace std;

Navigation::Navigation() : m_outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

map<string, int> TMode{

	{ "Foot", 1 },
	{ "Bike", 2 },
	{ "Car", 3 },
	{ "Bus", 4 },
	{ "Rail", 5 },
	{ "Ship", 6 }
};

map<int, string> rTMode{

	{ 1, "Foot" },
	{ 2, "Bike" },
	{ 3, "Car" },
	{ 4, "Bus" },
	{ 5, "Rail" },
	{ 6, "Ship" }
}; // C++ 11 does not provide a bi-directional map, I neither have the time nor patiences to implement one, so this will have to do

bool Navigation::ProcessCommand(const string& commandString) {

	vector<string> strings;

	split(commandString, ' ', strings);

	m_outFile << fixed << setprecision(3);

	try	
	{
		if (strings[0].compare("MaxDist") == 0)
			MaxDist();

		if (strings[0].compare("MaxLink") == 0)
			MaxLink();
		
		if (strings[0].compare("FindDist") == 0)
			FindDist(stoi(strings[1]), stoi(strings[2]));

		if (strings[0].compare("FindNeighbour") == 0)
			FindNeighbour(stoi(strings[1]));

		if (strings[0].compare("Check") == 0) {

			vector<string> subS(strings.begin() + 2, strings.end());
			vector<int> subI;

			transform(subS.begin(), subS.end(), back_inserter(subI), [](const string& str) {return stoi(str); });
			Check(TMode[strings[1]], subI);
		}

		if (strings[0].compare("FindRoute") == 0)
			FindRoute(TMode[strings[1]], stoi(strings[2]), stoi(strings[3]));

	} catch (exception &e) {
		return(false);
	}
	return(true);
}

vector<string>& Navigation::split(const string& s, char delim, vector<string>& elems) const {

	stringstream ss(s);
	string item;

	while (getline(ss, item, delim))
		elems.push_back(item);

	return(elems);
}

bool Navigation::BuildNetwork(const string& fileNamePlaces, const string& fileNameLinks) {

	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);

	if (finPlaces.fail() || finLinks.fail())
		return(false);

	/// BUG - CSV input must have a blank new line at the eof

	for (CSVIterator loop(finPlaces); loop != CSVIterator(); ++loop) {
		_nodesMap[stoi((*loop)[1])] = new Node((*loop)[0], stoi((*loop)[1]), stof((*loop)[2]), stof((*loop)[3]));
	}


	for (CSVIterator loop(finLinks); loop != CSVIterator(); ++loop) {

		if (validRef(stoi((*loop)[0]), stoi((*loop)[1])))
			continue;

		Node* nodeA = _nodesMap[stoi((*loop)[0])];
		Node* nodeB = _nodesMap[stoi((*loop)[1])];

		Arc* a = new Arc(nodeA, nodeB, TMode[(*loop)[2]], NtoL(*nodeA, *nodeB));

		_arcs.push_back(*a);

		nodeA->addArc(a);
		nodeB->addArc(a);
	}
	return(true);
}

bool Navigation::MaxDist(void) {

	Node* N1(nullptr);
	Node* N2(nullptr);

	float tDist; // Test Distance
	float mxDist = 0.0f; // Max Distance

	for (auto nA = _nodesMap.begin(); nA != _nodesMap.end(); ++nA) {

		for (auto nB = nA; nB != _nodesMap.end(); ++nB) {

			tDist = NtoL(*(nA->second), *(nB->second));

			if (tDist > mxDist) {

				N1 = nA->second;
				N2 = nB->second;

				mxDist = tDist;
			}
		}
	}

	m_outFile << "MaxDist" << endl << N1->getLocation() << ',' << N2->getLocation() << ',' << mxDist << endl << endl;

	return(true);
}

bool Navigation::MaxLink(void) {

	Arc aMax;

	float fMax = 0.0f;

	for (auto a = _arcs.begin(); a != _arcs.end(); ++a) {

		if (fMax < a->getLength()) {

			aMax = *a;
			fMax = a->getLength();
		}
	}

	// TODO: Spec. Error - Wrong Order
	m_outFile << "MaxLink" << endl;
	m_outFile << aMax.getRefA() << ',' << aMax.getRefB() << ',' << fMax << endl;
	m_outFile << endl;

	return(true);
}

bool Navigation::FindDist(int refA, int refB) {

	m_outFile << "FindDist" << ' ' << refA << ' ' << refB << endl;

	if (validRef(refA, refB)) {

		m_outFile << "FAIL" << endl << endl;
		return(false);
	}

	Node* N1(_nodesMap[refA]);
	Node* N2(_nodesMap[refB]);

	m_outFile << N1->getLocation() << ',' << N2->getLocation() << ',' << NtoL(*N1, *N2) << endl;
	m_outFile << endl;

	return(true);
}

bool Navigation::FindNeighbour(int ref) {

	if (validRef(ref, NULL)) {
		 
		m_outFile << "FAIL" << endl << endl;
		return(false);
	}

	Node* N(_nodesMap[ref]);

	m_outFile << "FindNeighbour" << ' ' << N->getRef() << endl;

	vector<Arc*> nbrs(N->getArcs());

	for (auto a = nbrs.begin(); a != nbrs.end(); ++a) {

		if ((*a)->getRefA() != ref)
			m_outFile << (*a)->getRefA() << endl;
		else
			m_outFile << (*a)->getRefB() << endl;
	}

	m_outFile << endl;

	return(true);

}

bool Navigation::Check(const int mode, vector<int>& aRefs) {

	m_outFile << "Check" << ' ' << rTMode[mode] << ' ';

	for (auto aR = aRefs.begin(); aR != aRefs.end(); aR++)
		m_outFile << *aR << ' ';

	bool vBool;
	bool fBool = false;

	m_outFile << endl;

	for (auto aR = aRefs.begin(); aR != aRefs.end() - 1; aR++) {

		m_outFile << *aR << ',' << *(aR + 1) << ',';

		vBool = validRef(*aR, *(aR + 1));

		if (!vBool) {

			if (_nodesMap[*aR]->arcsTo(*(aR + 1), mode)) {

				m_outFile << "PASS";
				fBool = true;
			}
		} if (vBool || !fBool) {

			m_outFile << "FAIL" << endl;
			break;
		}

		m_outFile << endl;
	}

	m_outFile << endl;

	return(true);
}

bool Navigation::FindRoute(const int mode, const int refA, const int refB) {

	/// A* Pathfinding Algorithm  

	/*
	vector<Node*> openVec; // Set of nodes to be evaluated
	vector<Node*> closedVec; // Set of nodes already evaluate

	openVec.push_back(_nodesMap[refA]);

	while (!(openVec.size() == 0)) {

	Node* current = openVec.back(); // Last element aligned to have the lowest F Cost
	openVec.pop_back();
	closedVec.push_back(current);

	if (current == _nodesMap[refB]) break; // Target met

	for (auto a = current->getArcs().begin(); a != current->getArcs().end(); a++) {

	Node* N2(nullptr);

	// Verify correct B Node of arc
	if (current->getRef() != (*a)->getRefB()) N2 = _nodesMap[(*a)->getRefB()]; else N2 = _nodesMap[(*a)->getRefA()];

	N2->setRouteParent(current->getRef());

	if (!current->arcsTo(N2->getRef(), mode) || (find(closedVec.begin(), closedVec.end(), N2) != closedVec.end()))
	continue;

	float G(current->getG() + NtoL(*(current), *(N2)));
	float H(NtoL(*(N2), *(_nodesMap[refB])));

	N2->setG(G); N2->setH(H); N2->setF(G + H);

	if (find(openVec.begin(), openVec.end(), N2) != openVec.end())

	if ((NtoL(*(N2), *(_nodesMap[refB])) < H) || !(find(openVec.begin(), openVec.end(), N2) != openVec.end())) {

	if (!(find(openVec.begin(), openVec.end(), N2) != openVec.end()))
	openVec.push_back(N2);
	}
	}

	sort(openVec.begin(), openVec.end(), [](Node* nA, Node* nB) {
	return(nA->getF() < nB->getF());
	}); // Elements ranked based on a lowest F cost
	}
	*/

	/// Depth-First Binary-Tree Traversial

	bool fBool = false;
	bool vBool;

	m_outFile << "FindRoute" << ' ' << rTMode[mode] << ' ' << refA << ' ' << refB << endl;
	
	vBool = validRef(refA, refB);

	if (validRef(refA, refB)) {
		
		m_outFile << "FAIL" << endl << endl;
		return(false);
	}

	Node* N1(_nodesMap[refA]); // start node
	Node* N2(_nodesMap[refB]); // end node

	map<Node*, bool> visited;
	stack<Node*> nStack;
	nStack.push(N1);

	for (auto n = _nodesMap.begin(); n != _nodesMap.end(); ++n)
		visited[n->second] = false;

	while (!nStack.empty()) {

		Node* cN(nStack.top());
		nStack.pop();

		visited[cN] = true;

		if (cN == N2) {

			fBool = true;

			const vector<int> route(buildRoute(cN, refA));

			for (auto r = route.rbegin(); r != route.rend(); ++r) // Reverse iteration
				m_outFile << *r << endl;

			m_outFile << endl;
			break;
		}

		vector<int> nbrRefs(cN->getArcsReference(mode)); // Traversable neighbour nodes (refs)

		for (auto nR = nbrRefs.begin(); nR != nbrRefs.end(); ++nR) {

			if (!visited[_nodesMap[*nR]]) {

				_nodesMap[*nR]->setRouteParent(cN->getRef());
				nStack.push(_nodesMap[*nR]);
			}
		}
	}

	for (auto n = _nodesMap.begin(); n != _nodesMap.end(); ++n)
		n->second->clearHeuristics();

	if (!fBool) {
		m_outFile << "FAIL" << endl << endl;
	}
	if (fBool) return(true); else return(false);
}

vector<int> Navigation::buildRoute(Node* sN, const int& eRef) {

	Node* cN(sN); // current node
	vector<int> rt; // route

	rt.push_back(cN->getRef());

	while (cN->getRef() != eRef) {

		cN = _nodesMap[cN->getRouteParent()];
		rt.push_back(cN->getRef());
	}

	return(rt);
}

inline bool Navigation::validRef(const int& refA, const int& refB) const {

	if (refB)
		return(_nodesMap.find(refA) == _nodesMap.end() || _nodesMap.find(refB) == _nodesMap.end());
	else
		return(_nodesMap.find(refA) == _nodesMap.end());
}

inline float Navigation::NtoL(const Node& nA, const Node& nB) const {
	return(ArcLength(nA.getLatt(), nA.getLong(), nB.getLatt(), nB.getLong()));
}

/// ###################################################################################
/// ##                                                                               ##
/// ##  The following methods are used to calculate the distance between two nodes.  ##
/// ##  DO NOT edit either of these methods!                                         ##
/// ##                                                                               ##
/// ###################################################################################

// **** DO NOT EDIT THIS METHOD ****
float Navigation::ArcLength(float startNorth, float startEast, float endNorth, float endEast) const {

	float UTMNorthingStart;
	float UTMEastingStart;
	float UTMNorthingEnd;
	float UTMEastingEnd;

	LLtoUTM(startNorth, startEast, UTMNorthingStart, UTMEastingStart);
	LLtoUTM(endNorth, endEast, UTMNorthingEnd, UTMEastingEnd);

	float dNorth = UTMNorthingEnd - UTMNorthingStart;
	float dEast = UTMEastingEnd - UTMEastingStart;

	return sqrt(dNorth*dNorth + dEast*dEast) * 0.001f;
}

// **** DO NOT EDIT THIS METHOD ****
void Navigation::LLtoUTM(const float Lat, const float Long, float &UTMNorthing, float &UTMEasting) const {
	//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
	//East Longitudes are positive, West longitudes are negative.
	//North latitudes are positive, South latitudes are negative
	//Lat and Long are in decimal degrees
	//Original code by Chuck Gantz- chuck.gantz@globalstar.com (http://www.gpsy.com/gpsinfo/geotoutm/)
	//Adapted by Darren McKie

	const float PI = 3.14159265f, FOURTHPI = PI / 4.0f, DEG2RAD = PI / 180.0f;

	float a = 6378137;
	float eccSquared = 0.00669438f;
	float k0 = 0.9996f;

	float LongOrigin;
	float eccPrimeSquared;
	float N, T, C, A, M;

	//Make sure the longitude is between -180.00 .. 179.9
	float LongTemp = (Long + 180) - int((Long + 180) / 360) * 360 - 180; // -180.00 .. 179.9;

	float LatRad = Lat*DEG2RAD;
	float LongRad = LongTemp*DEG2RAD;
	float LongOriginRad;
	int   ZoneNumber = 30;

	LongOrigin = (ZoneNumber - 1) * 6.0f - 180 + 3;  //+3 puts origin in middle of zone
	LongOriginRad = LongOrigin * DEG2RAD;

	eccPrimeSquared = (eccSquared) / (1 - eccSquared);

	N = a / sqrt(1 - eccSquared * sin(LatRad) * sin(LatRad));
	T = tan(LatRad) * tan(LatRad);
	C = eccPrimeSquared * cos(LatRad) * cos(LatRad);
	A = cos(LatRad) * (LongRad - LongOriginRad);

	M = a*((1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 - 5 * eccSquared * eccSquared * eccSquared / 256) * LatRad
		- (3 * eccSquared / 8 + 3 * eccSquared * eccSquared / 32 + 45 * eccSquared * eccSquared * eccSquared / 1024) * sin(2 * LatRad)
		+ (15 * eccSquared * eccSquared / 256 + 45 * eccSquared * eccSquared * eccSquared / 1024) * sin(4 * LatRad)
		- (35 * eccSquared * eccSquared * eccSquared / 3072) * sin(6 * LatRad));

	UTMEasting = (k0*N*(A + (1 - T + C)*A*A*A / 6
		+ (5 - 18 * T + T*T + 72 * C - 58 * eccPrimeSquared)*A*A*A*A*A / 120)
		+ 500000.0f);

	UTMNorthing = (k0*(M + N*tan(LatRad)*(A*A / 2 + (5 - T + 9 * C + 4 * C*C)*A*A*A*A / 24
		+ (61 - 58 * T + T*T + 600 * C - 330 * eccPrimeSquared)*A*A*A*A*A*A / 720)));
	if (Lat < 0)
		UTMNorthing += 10000000.0f; //10000000 meter offset for southern hemisphere
}
