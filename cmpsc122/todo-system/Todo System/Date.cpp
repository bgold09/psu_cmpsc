// Date.h - Date class implementation
// Date containing month and year only
// Written by Brian Golden

#include "Date.h"
#include <iostream>
#include <string>
using namespace std;

#define NOT_FOUND		-1

// Default constructor
Date::Date()
{
	Day = NULL;
	Month = NULL;
}

// Init-constructor
Date::Date(int month, int day)
{
	Day = day;
	Month = month;
}

// Set Day and Month
void Date::Set(int month, int day)
{
	Day = day;
	Month = month;
}

// Set day and month by taking in valid text format
void Date::Set(const string& dateText)
{
	int day, month;
	day = month = 0;

	// Look for '/' character
	int slashIndex = NOT_FOUND;
	for (int i = 0; i < int(dateText.size()); i++)
	{
		if (dateText[i] == '/')
			slashIndex = i;				
	}

	// day is from beginning to slash; month from slash to end
	string monthText = dateText.substr(0, slashIndex);
	string dayText = dateText.substr(slashIndex+1);

	if (monthText.size() == 1)
		month = monthText[0] - '0';
	else
	{
		if (monthText[0] == '0')
			// subtract '0' to get int value
			month = monthText[1] - '0';
		else
			// Add ten to first digit if month is 10, 11, 12
			month = 10 + (monthText[1] - '0');
	}

	if (dayText.size() == 1)
		day = dayText[0] - '0';
	else
	{
		if (dayText[0] == '0')
			day = dayText[1] - '0';
		else
			// Add ten to first digit if month > 9
			day = 10*(dayText[0] - '0') + (dayText[1] - '0');		
	}
	Set(month, day);
}

int Date::GetDay() const
{
	return Day;
}

int Date::GetMonth() const
{
	return Month;
}

// Returns true if Day and/or Month are NULL
bool Date::IsEmpty() const
{
	return (Day == NULL || Month == NULL);
}

// Returns true if *this is earlier than compareTo
bool Date::operator < (const Date& compareTo) const
{
	if (Month < compareTo.GetMonth())
		return true;

	if (Month <= compareTo.GetMonth())
		if (Day < compareTo.GetDay())
			return true;

	return false;
}

// Returns true if *this is later than compareTo
bool Date::operator > (const Date& compareTo) const
{
	if (Month > compareTo.GetMonth())
		return true;

	if (Month >= compareTo.GetMonth())
		if (Day > compareTo.GetDay())
			return true;

	return false;
}

// Stream output operator
std::ostream& operator << (std::ostream& out, const Date& aDate)
{
	return out << "[" << aDate.GetMonth() << "/" << aDate.GetDay() << "]";
}