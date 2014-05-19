// DomainPart.h - email domain class declaration
// Written by Brian Golden

#pragma once

#include "String.h"
#include "Vector.h"
#include "SubdomainPart.h"

class DomainPart
{
public:
	// Default constructor, does nothing
	DomainPart() {}

	// Destructor
	~DomainPart();

	// Sets the address
	void Set(const String& address, Vector<String> validTlds);

	// Performs domain validation
	bool IsValid();

	// Check for valid characters
	bool CheckForValidCharacters();

	// Searches for aChar, returns true if found in text
	bool characterFound(char* text, char aChar);

	// Returns false when emailPart violates the Dot Rule 
	// or Dash Rule, whichever is specified by aChar
	bool CheckRule(char aChar);

private:
	// Parses Address into an array of Subdomains and Tld
	bool Parse();

	bool SubdomainsAreValid();

	// Domain address
	String Address;
	Vector<SubdomainPart*> Subdomains;
	Vector<String> ValidTlds;
};