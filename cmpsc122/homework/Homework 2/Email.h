// Email.h - email class declaration
// Written by Brian Golden
#pragma once

#include <string>
#include "LocalPart.h"
#include "DomainPart.h"

using namespace std;

class Email
{
public:
	Email(const string& address);

	// Performs all email validation functions
	bool IsValid();

private:
	// Parses Address into Local and Domain by searching for '@';
	// returns false if '@' character is not found
	bool Parse();

	// Email address
	string Address;
	LocalPart Local;
	DomainPart Domain;
};