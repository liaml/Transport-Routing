#include "Node.hpp"
#include "Arc.hpp"
#include <algorithm>

using namespace std;

Node& Node::operator=(const Node& rhs)
{
	if (&rhs != this)
	{
		this->_location = rhs._location;
		this->_ref = rhs._ref;
		this->_latt = rhs._latt;
		this->_long = rhs._long;
		this->_G = rhs._G;
		this->_H = rhs._H;
		this->_F = rhs._F;
		this->_rParent = rhs._rParent;
		this->_arcs = rhs._arcs;
	}
	return(*this);
}

Node::Node(const string& place, const int& reference, const float& lattitude, const float& longitude) :
_arcs(NULL), _location(place), _latt(lattitude), _long(longitude), _G(NULL), _H(NULL), _F(NULL), _rParent(NULL), _ref(reference)
{
}

Node::~Node(void)
{
}

string Node::getLocation(void) const {
	return(_location);
}

int Node::getRef(void) const {
	return(_ref);
}

float Node::getLatt(void) const {
	return(_latt);
}

float Node::getLong(void) const {
	return(_long);
}

void Node::setG(const float& g) {
	this->_G = g;
}

float Node::getG(void) const {
	return(_G);
}

void Node::setH(const float& h) {
	this->_H = h;
}

float Node::getH(void) const {
	return(_H);
}

void Node::setF(const float& f) {
	this->_F = f;
}

float Node::getF(void) const {
	return(_F);
}

void Node::setRouteParent(const int& ref) {
	this->_rParent = ref;
}

int Node::getRouteParent(void) const {
	return(_rParent);
}

void Node::clearHeuristics(void) {

	this->_G = NULL;
	this->_H = NULL;
	this->_F = NULL;
	this->_rParent = NULL;
}

void Node::addArc(Arc* arc) {
	_arcs.push_back(arc);
}

vector<Arc*> Node::getArcs(void) const {

	vector<Arc*> arcs;
	for_each(_arcs.begin(), _arcs.end(), [&](Arc* a){arcs.emplace_back(a); });

	return(arcs);
}

bool Node::arcsTo(const int pRef, const int &tMode = 1) {
	
	for (vector<Arc*>::const_iterator a = _arcs.begin(); a != _arcs.end(); ++a)
		if ((*a)->getRefA() == pRef || (*a)->getRefB() == pRef)
			return(validArc(tMode, **a));
		
	return(false);
}

vector<int> Node::getArcsReference(const int &tMode = 1)  {

	vector<int> refNums;

	for (vector<Arc*>::iterator a = _arcs.begin(); a != _arcs.end(); ++a) {

		if (validArc(tMode, **a)) {

			if ((*a)->getRefA() == this->_ref) { refNums.push_back((*a)->getRefB()); }
			else { refNums.push_back((*a)->getRefA()); }
		}
	}

	return(refNums);
}

bool Node::validArc(const int& tMode, const  Arc& a) const {

	if (1 == tMode && a.getMode() >= tMode) 
		return(true);
	
	if (2 == tMode && a.getMode() >= tMode)
		return(true);
	
	if (3 == tMode && ((a.getMode() >= 3) && (a.getMode() != 5))) 
		return(true);
	
	if (4 == tMode && ((a.getMode() >= 4) && (a.getMode() != 5))) 
		return(true);
	
	if (5 == tMode && a.getMode() >= tMode) 
		return(true);
	
	if (6 == tMode && a.getMode() >= tMode) 
		return(true);

	return false;
}
