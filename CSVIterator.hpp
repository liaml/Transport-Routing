#ifndef CSVITERATOR_HPP
#define CSVITERATOR_HPP

#pragma once

// C/C++ Standard Libraries
#include <stdio.h>

// Internal Dependancies
#include "CSVParser.hpp"

class CSVIterator
{

public:

	explicit CSVIterator(std::istream& str);
	CSVIterator(void);

	CSVIterator& operator++(void);

	CSVRow const& operator*(void) const;

	bool operator==(CSVIterator const& rhs) const;
	bool operator!=(CSVIterator const& rhs) const;

private:
	
	CSVRow _row;
	std::istream* _str;
};

#endif /* CSVIterator_hpp */
