#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#pragma once

class Arc;
// C/C++ Standard Libraries
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>
#include <iomanip>

// Internal Dependancies
#include "CSVIterator.hpp"

class Node;

class Navigation
{

public:

	Navigation(void);
	~Navigation(void);

	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(std::string &commandString);

	bool MaxDist(void);
	bool MaxLink(void);
	bool FindDist(int refA, int refB);
	bool FindNeighbour(int ref);
	bool Check(const int mode, std::vector<int>& aRefs);
	bool FindRoute(const int mode, const int refA, const int refB);


	bool ProcessCommand(const std::string& commandString);
	std::vector<std::string>& Navigation::split(const std::string& s, char delim, std::vector<std::string>& elems) const;
	std::vector<int> buildRoute(Node* sN, const int& eRef);

	inline float NtoL(const Node& nA, const Node& nB) const;
	inline float ArcLength(float startNorth, float startEast, float endNorth, float endEast) const;
	inline bool validRef(const int& refA, const int& refB) const;

private:

	/// DARREN MCKIE DEC

	void LLtoUTM(const float Lat, const float Long, float &UTMNorthing, float &UTMEasting) const;
	std::ofstream m_outFile;
	
	/// LIAM C LEES DEC

	std::map<int, Node*> _nodesMap;
	std::vector<class Arc> _arcs;
};

#endif /* Navigation_hpp */