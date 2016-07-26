#ifndef NODE_HPP
#define NODE_HPP

#pragma once

// C/C++ Standard Libraries
#include <cstdio>
#include <string>
#include <vector>

class Arc;

class Node {

public:
	Node& operator=(const Node& rhs);
	Node(const std::string& place, const int& reference, const float& lattitude, const float& longitude);
	~Node(void);

	std::string getLocation(void) const;
	int getRef(void) const;
	float getLatt(void) const;
	float getLong(void) const;

	void setG(const float& g);
	float getG(void) const;
	void setH(const float& h);
	float getH(void) const;
	void setF(const float& f);
	float getF(void) const;

	void setRouteParent(const int& ref);
	int getRouteParent(void) const;

	void clearHeuristics(void);

	void addArc(Arc* link);
	std::vector<Arc*> getArcs(void) const;
	bool arcsTo(const int pRef, const int& tMode);


	bool arcsTo(const int pRef, const int& tMode) const;
	std::vector<int> getArcsReference(const int& tMode);
	bool validArc(const int& tMode, const Arc& a) const;
private:
	
	std::vector<Arc*> _arcs;
	
	std::string _location;
	
	float _latt;
	float _long;
	float _G, _H, _F;
	
	int _rParent;
	int _ref;
};

#endif /* Node_hpp */
