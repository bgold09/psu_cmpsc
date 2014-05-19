// SubdomainPart.h - email subdomain class declaration
// Written by Brian Golden

#pragma once

#include "String.h"

class SubdomainPart
{
public:
	// Default constructor, does nothing
	SubdomainPart() {}

	// Sets the address
	SubdomainPart(const String& address);

	// Performs subdomain validation
	virtual bool IsValid();

	// Check for valid characters
	bool CheckForValidCharacters();

	// Searches for aChar, returns true if found in text
	bool characterFound(char* text, char aChar);

	// Returns false when emailPart violates the Dot Rule 
	// or Dash Rule, whichever is specified by aChar
	bool CheckRule(char aChar);

protected:
	// SubdomainPart address
	String Address;
};