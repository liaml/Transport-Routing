#ifndef ARC_HPP
#define ARC_HPP

#pragma once

// C/C++ Standard Libraries
#include <cstdio>
#include <vector>
#include <string>

class Node;

class Arc {


public:

	Arc(void);
	~Arc(void);
	Arc(Node* nA, Node* nB, int transportType, float arcLength);

	int getRefA(void) const;
	int getRefB(void) const;
	int getMode(void) const;

	float getLength(void) const;

private:

	Node* _placeA;
	Node* _placeB;
	int _mode;
	float _length;
};

#endif /* Arc_hpp */
