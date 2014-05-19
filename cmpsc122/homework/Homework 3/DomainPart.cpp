// DomainPart.cpp - email domain class implementation
// Written by Brian Golden

#include "Vector.h"
#include "DomainPart.h"
#include "TldPart.h"

#define NOT_FOUND							-1
#define NUMBER_OF_VALID_CHARACTERS			83

// list of valid characters
#define VALID_CHARACTERS \
{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&*+-/=?^_`{}~.'"}

// Destructor
DomainPart::~DomainPart()
{
	Subdomains = NULL;
}

// Sets the address
void DomainPart::Set(const String& address, Vector<String> validTlds)
{
	Address = address;
	ValidTlds = validTlds;
}

// Parses Address into an array of Subdomains and Tld
bool DomainPart::Parse()
{
	// Check size
	int domainPartLength = Address.GetLength();
	if (domainPartLength > 253)
		return false;

	// first and last characters of DomainPart must not be '.' or '-'
	if (Address[0] == '.' || Address[0] == '-' ||Address[domainPartLength-1] == '.' || Address[domainPartLength-1] == '-')
		return false;

	// we need to be looking for '.'
	// the last subdomain is a TLD, needs to go into Tld data member
	// we need to populate Vector of Subdomains
	// Subdomains.Add(Subdomain(substring corresponding to subdomain));
	// Tld.Set(substring corresponding to TLD);

	// Look for last dot
	int lastDotIndex = NOT_FOUND;
	for (int pos = 0; pos < domainPartLength; pos++)
		if (Address[pos] == '.')
			lastDotIndex = pos;

	// Invalid if no '.' is found
	if (lastDotIndex == NOT_FOUND)
		return false;

	// Get TLD; characters from lastDotIndex to end of Domain
	String tld_string = Address.Substring(lastDotIndex + 1);

	// Get subdomains and insert into Subdomains
	String aSubdomain;
	int previousDotIndex = NOT_FOUND;
	for ( int domainPos = 0; domainPos < lastDotIndex; domainPos += (previousDotIndex+1) )
	{
		// Find index of the next '.'
		int nextDotIndex;
		int i = domainPos;
		for ( ; Address[i] != '.'; i++);
		nextDotIndex = i;

		// aSubDomain is substring from previousDotIndex to nextDotIndex,
		// not including the dots

		if (previousDotIndex == NOT_FOUND)
			aSubdomain = Address.Substring(0, nextDotIndex);
		else
			aSubdomain = Address.Substring(previousDotIndex + 1, nextDotIndex - previousDotIndex - 1);

		// Insert aSubDomain into Vector Subdomains
		Subdomains.Add(new SubdomainPart(aSubdomain));

		// Look for next dot after current nextDotIndex
		previousDotIndex = nextDotIndex;
	}

	// add Tld as last Subdomain

	Subdomains.Add(new TldPart(tld_string, ValidTlds));

	return true;
}

// Performs domain validation
bool DomainPart::IsValid()
{
	return
		// Parse Address into an array of Subdomains and Tld
		Parse() &&
		SubdomainsAreValid();
}

// Validates each subdomain
bool DomainPart::SubdomainsAreValid()
{
	for ( int i = 0; i < Subdomains.GetSize(); i++ )
		if ( !Subdomains[i]->IsValid() )
			return false;

	return true;
}

bool DomainPart::CheckForValidCharacters()
{
	char* validCharacters = VALID_CHARACTERS;

	// check each character against the list of valid ones
	for (int i = 0; i < Address.GetLength(); i++)
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
	int length = Address.GetLength();
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