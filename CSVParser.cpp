#include "CSVParser.hpp"

using namespace std;

CSVRow::CSVRow(void)
{
}

CSVRow::~CSVRow(void)
{
}

string const& CSVRow::operator[](size_t index) const{
	return _data[index];
}

size_t CSVRow::size(void) const {
	return _data.size();
}

void CSVRow::nextRow(istream& str) {

	string row;
	getline(str, row);

	stringstream lineStream(row);
	string col;

	_data.clear();
	while (getline(lineStream, col, ','))
	{
		_data.push_back(col);
	}
}

istream& operator>>(istream& str, CSVRow& data) {

	data.nextRow(str);
	return str;
}