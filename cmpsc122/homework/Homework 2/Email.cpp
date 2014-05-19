// Email.cpp - email class implementation
// Written by Brian Golden

#include "Email.h"

#define NOT_FOUND	-1

Email::Email(const string& address)
{
	Address = address;
}

// Parses Address into Local and Domain by searching for '@';
// returns false if '@' character is not found
bool Email::Parse()
{
	// Look for '@' character
	int atIndex = NOT_FOUND;
	int i = 0;
	for ( ; i < Address.length(); i++)
		if (Address[i] == '@')
			atIndex = i;

	// Invalid if '@' was not found, or if first or last character
	if (atIndex == NOT_FOUND || atIndex == 0 || atIndex == Address.length()-1)
		return false;

	// Set Local to characters from beginning of Email
	// up to character before '@'
	string localPart = Address.substr(0, atIndex);	// no -1
	Local.Set(localPart);

	// Set Domain to characters from character after '@'
	// to end of Email
	string domainPart = Address.substr(atIndex+1);
	Domain.Set(domainPart);

	return true;
}

bool Email::IsValid()
{
	return
		// Parses Address into Local and Domain parts
		Parse() &&
		Local.IsValid() &&
		Domain.IsValid();
}