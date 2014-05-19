// TldPart.cpp - email TLD class implementation
// Written by Brian Golden

#include "TldPart.h"

#include <string>
#include <fstream>

// Sets the address
void TldPart::Set(const string& address)
{
	Address = address;
}

// Performs TLD validation; check Address against 
// a list of all possible valid TLDs from file
bool TldPart::IsValid()
{
	// - open ICANN list
	ifstream tld_list ("tld_list.txt");

	// - while we are not at end of tld_list file
	while ( !tld_list.eof() )
	{
		string valid_TLD;

		// - Read TLD from tld_list file
		getline(tld_list, valid_TLD);

		// - Check valid_TLD against topLevelDomain
		// - get length of topLevelDomain
		int tldLength = Address.length();
		int valid_TLD_Length = valid_TLD.length();

		// If Tld from file is not the same length as TldPart, 
		// then they cannot be the same; move on through file
		if (valid_TLD_Length != tldLength)
			continue;

		// if a character in topLevelDomain is not the same
		// as the corresponding one in valid_TLD, then
		// topLevelDomain is not a valid TLD
		bool validChar = true;
		int i = 0;
		for ( ; i < tldLength && validChar == true; i++) 
		{
			char aChar = Address[i];
			aChar = toupper(aChar);
			if ( aChar != valid_TLD[i] )
				validChar = false;
		}

		// check to see why program exited loop:
		// If the last char in both the source and 
		// registered TLD are the same, then they are
		// the same. If not, then they are not the same
		// and the source TLD is invalid
		if ( toupper(Address[tldLength-1]) == valid_TLD[tldLength-1] )
		{
			tld_list.close();
			return true;
		}
		else 
			continue;
	}
	return false;
}