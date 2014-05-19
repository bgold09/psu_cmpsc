// Email.h - email class declaration
// Written by Brian Golden

#pragma once

#include "String.h"
#include "LocalPart.h"
#include "DomainPart.h"
#include "Vector.h"

class Email
{
public:	
	// Init-Constructor
	Email(const String& address, const Vector<String> validTlds);

	// Performs all email validation functions
	bool IsValid();

	// Preloads valid tlds from file into Vector of Strings
	void PreloadTlds();

private:
	// Parses Address into Local and Domain by searching for '@';
	// returns false if '@' character is not found
	bool Parse();

	// Email address
	String Address;
	LocalPart Local;
	DomainPart Domain;
	Vector<String> ValidTlds;
};