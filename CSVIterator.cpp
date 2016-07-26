#include "CSVIterator.hpp"

using namespace std;

CSVIterator::CSVIterator(istream& str) : _str(str.good() ? &str : NULL)
{
	++(*this);
}

CSVIterator::CSVIterator() : _str(nullptr)
{
}

CSVIterator& CSVIterator::operator++() {

	if (_str) {
		(*_str) >> _row;
		_str = (_row.size() != 0) ? _str : NULL;
	}
	return *this;
}

const CSVRow& CSVIterator::operator*(void) const {
	
	const CSVRow& rowREF = _row;
	return (rowREF);
	
	//return(_row);
}

bool CSVIterator::operator==(CSVIterator const& rhs) const {
	return ((this == &rhs) || ((this->_str == nullptr) && (rhs._str == nullptr)));
}

bool CSVIterator::operator!=(CSVIterator const& rhs) const {
	return !((*this) == rhs);
}