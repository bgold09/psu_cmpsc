// Tag.cpp - Tag class implementation
// Written by Brian Golden

#include <string>
#include "Tag.h"
using namespace std;

#define NUMBER_OF_INVALID_CHARACTERS	8
#define INVALID_CHARACTERS				"{}()[]/,"

// Returns Text
string Tag::GetText() const
{
	return Text;
}

// Returns true is the Tag is valid
bool Tag::IsValid() const
{
	string invalidCharacters = INVALID_CHARACTERS;

	for (int i = 0; i < NUMBER_OF_INVALID_CHARACTERS; i++)
	{
		char invalidChar = invalidCharacters[i];
		// look for invalidChar within text
		if ( CharacterFound(Text, invalidChar) )
			return false;
	}

	return true;
}


// Assignment operator
Tag& Tag::operator = (const string& source)
{	
	Text = source;
	return *this;
}

// Equality operator returns true when Tags are same length and 
// all characters are the same
bool Tag::operator == (const Tag& compareTo) const
{
	string text = compareTo.GetText();
	string compareToText = compareTo.GetText();

	return (text == compareToText);
}

// Inequality operator
bool Tag::operator != (const Tag& compareTo) const
{
	// Use equality operator
	if ( compareTo == *this )
		return false;
	return true;
}

bool Tag::ContainsText(const string& searchText) const
{
	int searchTextSize = searchText.size();
	string searchFor = searchText;

	// make searchText all lowercase
	for (int c = 0; c < int(searchText.size()); c++)
		searchFor[c] = tolower(searchText[c]);

	// Search as long as as substring of searchTextSize chars remain
	for (int i = 0; i < int(Text.size()-searchTextSize+1); i++)
	{
		string temp = Text.substr(i, searchTextSize);
		// make temp all lowercase
		for (int c = 0; c < int(temp.size()); c++)
			temp[c] = tolower(temp[c]);

		if (temp == searchFor)
			return true;
	}
	return false;
}

// Searches for aChar, returns true if found in text
bool Tag::CharacterFound(const string& text, char aChar) const
{
	int pos = 0;
	for ( ; text[pos] != '\0' && text[pos] != aChar; pos++);
	if ( text[pos] == aChar)
		return true;
	else 
		return false;
}

// Stream output operator
std::ostream& operator << (std::ostream& out, const Tag& aTag)
{
	return out << aTag.GetText();
}