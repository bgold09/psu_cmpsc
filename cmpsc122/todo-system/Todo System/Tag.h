// Tag.h - Tag class declaration
// Written by Brian Golden

#pragma once
#include <string>
using std::string;

class Tag
{
public:
	// Default constructor, does nothing
	Tag () {}

	// Assignment operator
	Tag& Tag::operator = (const string& source);

	// Returns true if the Tag is valid
	bool IsValid() const;

	// Equality operator returns true when Tags are same length and 
	// all characters are the same
	bool operator == (const Tag& compareTo) const;

	// Inequality operator
	bool operator != (const Tag& compareTo) const;

	// Returns Text
	string GetText() const;

	// True if the Text of tag contains the search text
	bool ContainsText(const string& searchText) const;

private:
	// True if aChar is found in text
	bool CharacterFound(const string& text, char aChar) const;

	// Data member
	string Text;
};

// Stream output operator
std::ostream& operator << (std::ostream& out, const Tag& aTag);