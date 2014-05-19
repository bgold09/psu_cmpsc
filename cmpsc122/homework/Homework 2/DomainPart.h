// DomainPart.h - email domain class declaration
// Written by Brian Golden

#pragma once

#include <string>
#include <vector>
#include "SubdomainPart.h"
#include "TldPart.h"

using namespace std;

class DomainPart
{
public:
	// Default constructor, does nothing
	DomainPart() {}

	// Sets the address
	void Set(const string& address);

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
	string Address;
	TldPart Tld;
	vector<SubdomainPart> Subdomains;
};