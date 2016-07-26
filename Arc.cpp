#include "Arc.hpp"
#include "Node.hpp"

using namespace std;

Arc::Arc(void) : _placeA(nullptr), _placeB(nullptr), _mode(NULL), _length(NULL)
{
}


Arc::Arc(Node* nA, Node* nB, int transportType, float arcLength) :
_placeA(nA), _placeB(nB), _mode(transportType), _length(arcLength)
{
}

Arc::~Arc(void)
{
}


int Arc::getRefA(void) const {
	return(_placeA->getRef());
}

int Arc::getRefB(void) const {
	return(_placeB->getRef());
}

float Arc::getLength(void) const {
	return(_length);
}

int Arc::getMode(void) const {
	return(_mode);
}


