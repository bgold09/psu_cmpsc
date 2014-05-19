// TldPart.cpp - email TLD class implementation
// Written by Brian Golden

#include "TldPart.h"
#include "String.h"
#include "Vector.h"

// Constructor
TldPart::TldPart(const String& address, Vector<String> validTlds)
{
	Address = address;
	ValidTlds = validTlds;
}

// Performs TLD validation; check Address against 
// a list of all possible valid TLDs from file
bool TldPart::IsValid()
{
	int i = 0;
	for ( ; i < ValidTlds.GetSize(); i++ )
	{
		String valid_Tld = ValidTlds[i];

		// - Check valid_TLD against topLevelDomain
		// - get length of topLevelDomain
		int tldLength = Address.GetLength();				// length of TldPart
		int valid_TLD_Length = valid_Tld.GetLength();		// length of the current valid tld

		// If Tld from Vector is not the same length as TldPart, then
		// they cannot be the same; move on through the other valid Tlds
		if (valid_TLD_Length != tldLength)
			continue;

		if (valid_Tld == Address)
			return true;
		else 
			continue;
	}
	return false;
}