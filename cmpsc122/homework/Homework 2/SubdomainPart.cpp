// SubdomainPart.cpp - email local part class implementation
// Written by Brian Golden

#include "SubdomainPart.h"

// list of valid characters
#define VALID_CHARACTERS \
{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&*+-/=?^_`{}~.'"}

// Sets the address
SubdomainPart::SubdomainPart(const string& address)
{
	Address = address;
}

// Performs subdomain validation
bool SubdomainPart::IsValid()
{
	// 1. Check size --> already checked in SubdomainPart::Parse; no need to 
	// extract subdomains if entire domain is too large

	// 2. Check for valid characters
	if ( !CheckForValidCharacters() )
		return false;

	// 3. Check dash rule
	if ( !CheckRule('-') )
		return false;

	// 4. Check dot rule
	if ( !CheckRule('.') )
		return false;

	return true;
}

// Check for valid characters
bool SubdomainPart::CheckForValidCharacters()
{
	char* validCharacters = VALID_CHARACTERS;
	
	// check each character in Address against the list of valid ones
	for (int i = 0; i < Address.length(); i++)
	{
		char currentCharacter = Address[i];
		// look for currentCharacter within valid characters
		if ( !characterFound(validCharacters, currentCharacter) )
			return false;
	}
	return true;
}

// Searches for aChar, returns true if found in text
bool SubdomainPart::characterFound(char* text, char aChar)
{
	int pos = 0;
	for ( ; text[pos] != '\0' && text[pos] != aChar; pos++);
	if ( text[pos] == aChar)
		return true;
	else 
		return false;
}

// Returns false when emailPart violates the Dot Rule 
// or Dash Rule, whichever is specified by aChar
bool SubdomainPart::CheckRule(char aChar)
{
	int length = Address.length();
	// Check if first character is aChar
	if ( Address[0] == aChar)
		return false;

	// Check if last character is aChar
	if ( Address[length - 1] == aChar)
		return false;

	// Check if aChar is immediately followed by another instance of aChar
	for (int i = 0; i < ( length-1 ); i++)
		if ( Address[i] == aChar)
			if ( Address[i + 1] == aChar)
				return false;

	return true;
}