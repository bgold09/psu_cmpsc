// DomainPart.cpp - email domain class implementation
// Written by Brian Golden

#include <vector>
#include "DomainPart.h"

#define NOT_FOUND							-1
#define NUMBER_OF_VALID_CHARACTERS			83

// list of valid characters
#define VALID_CHARACTERS \
{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&*+-/=?^_`{}~.'"}

// Sets the address
void DomainPart::Set(const string& address)
{
	Address = address;
}

// Parses Address into an array of Subdomains and Tld
bool DomainPart::Parse()
{
	// Check size
	int domainPartLength = Address.length();
	if (domainPartLength > 253)
		return false;

	// first and last characters of DomainPart must not be '.' or '-'
	if (Address[0] == '.' || Address[0] == '-' ||Address[domainPartLength-1] == '.' || Address[domainPartLength-1] == '-')
		return false;

	// we need to be looking for '.'
	// the last subdomain is a TLD, needs to go into Tld data member
	// we need to populate vector of Subdomains
	// Subdomains.push_back(Subdomain(substring corresponding to subdomain));
	// Tld.Set(substring corresponding to TLD);
	vector<TldPart> subDomains;

	// Look for last dot
	int lastDotIndex = NOT_FOUND;
	for (int pos = 0; pos < domainPartLength; pos++)
		if (Address[pos] == '.')
			lastDotIndex = pos;

	// Invalid if no '.' is found
	if (lastDotIndex == NOT_FOUND)
		return false;

	// Set TLD; characters from lastDotIndex to end of Domain
	string tld = Address.substr(lastDotIndex + 1);
	Tld.Set(tld);

	// Get subdomains and insert into subDomains
	string aSubDomain;
	int previousDotIndex = NOT_FOUND;
	for (int domainPos = 0; domainPos < lastDotIndex; domainPos += previousDotIndex)
	{
		// Find index of the next '.'
		int nextDotIndex;
		int i = domainPos;
		for ( ; Address[i] != '.'; i++);
		nextDotIndex = i;

		// aSubDomain is substring from previousDotIndex to nextDotIndex,
		// not including the dots

		if (previousDotIndex == NOT_FOUND)
			aSubDomain = Address.substr(0, nextDotIndex);
		else
			aSubDomain = Address.substr(previousDotIndex + 1, nextDotIndex - previousDotIndex - 1);

		// Insert aSubDomain into vector subDomains
		Subdomains.push_back(aSubDomain);

		// Look for next dot after current nextDotIndex
		previousDotIndex = nextDotIndex;
	}

	return true;
}

// Performs domain validation
bool DomainPart::IsValid()
{
	return
		// Parse Address into an array of Subdomains and Tld
		Parse() &&
		SubdomainsAreValid() &&
		Tld.IsValid();
}

// Validates each subdomain
bool DomainPart::SubdomainsAreValid()
{
	for ( int i = 0; i < Subdomains.size(); i++ )
		if ( !Subdomains[i].IsValid() )
			return false;

	return true;
}

bool DomainPart::CheckForValidCharacters()
{
	char* validCharacters = VALID_CHARACTERS;
	
	// check each character against the list of valid ones
	for (int i = 0; i < Address.length(); i++)
	{
		char currentCharacter = Address[i];
		// look for currentCharacter within valid characters
		if ( !characterFound(validCharacters, currentCharacter) )
			return false;
	}
	return true;
}

// Returns false when emailPart violates the Dot Rule 
// or Dash Rule, whichever is specified by aChar
bool DomainPart::CheckRule(char aChar)
{
	int length = Address.length();
	// returns false if first character is aChar
	if ( Address[0] == aChar)
		return false;

	// returns false if last character is aChar
	if ( Address[length - 1] == aChar)
		return false;

	// returns false if aChar is immediately followed by another instance of aChar
	for (int i = 0; i < ( length-1 ); i++)
		if ( Address[i] == aChar)
			if ( Address[i + 1] == aChar)
				return false;

	return true;
}

// Searches for aChar, returns true if found in text
bool DomainPart::characterFound(char* text, char aChar)
{
	int pos = 0;
	for ( ; text[pos] != '\0' && text[pos] != aChar; pos++);
	if ( text[pos] == aChar)
		return true;
	else 
		return false;
}