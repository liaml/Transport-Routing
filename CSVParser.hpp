#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

#pragma once

// C/C++ Standard Libraries
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{

public:

	CSVRow(void);
	~CSVRow(void);

	std::string const& operator[](std::size_t index) const;
	std::size_t size(void) const;

	void nextRow(std::istream& str);
	friend std::istream& operator>>(std::istream& str, CSVRow& data);

private:

	std::vector<std::string> _data;
};

#endif /* CSVParser_hpp */
