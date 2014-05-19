// LocalPart.h - email local part class declaration
// Written by Brian Golden

#pragma once

#include <string>
using namespace std;

class LocalPart
{
public:
	LocalPart() {}

	// Sets the address
	void Set(const string& address);

	// Performs local part validation
	bool IsValid();

	// Check for valid characters
	bool CheckForValidCharacters();

	// Searches for aChar, returns true if found in text
	bool characterFound(char* text, char aChar);

	// Returns false when emailPart violates the Dot Rule 
	// or Dash Rule, whichever is specified by aChar
	bool CheckRule(char aChar);

private:
	// LocalPart address
	string Address;
};