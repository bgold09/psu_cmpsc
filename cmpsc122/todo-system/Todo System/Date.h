// Date.h - Date class declaration
// Date containing month and year only
// Written by Brian Golden

#pragma once
#include <iostream>
#include <string>
using std::string;

class Date
{
public:
	// Default constructor
	Date();

	// Init-constructor
	Date(int month, int day);
	
	// Takes in text, sets Day and Month
	void Set(const string& dateText);

	// Return Day
	int GetDay() const;

	// Return Month
	int GetMonth() const;

	// Returns true if Day and/or Month are NULL
	bool IsEmpty() const;

	// Returns true if *this is earlier than compareTo
	bool operator < (const Date& compareTo) const;

	// Returns true if *this is later than compareTo
	bool operator > (const Date& compareTo) const;

private:
	// Set Day and Month
	void Set(int month, int day);

	// Data members
	int Month, Day;
};

// Stream output operator
std::ostream& operator << (std::ostream& out, const Date& aDate);