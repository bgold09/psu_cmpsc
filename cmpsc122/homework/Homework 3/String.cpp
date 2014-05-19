// String.cpp - String class implementation
// Written by Brian Golden

#include "String.h"
#include <string>
#include <sstream>
using namespace std;

// Default constructor that doubles as init
// When size = 0 a NULL string is created, else a 
// string of size characters is allocated
String::String (int size/* = 0*/) 
{
	if (size == 0)
		Text = NULL;
	else
		Text = new char[size];
}

// Copy constructor: performs deep copy
String::String(const String& source)
{
	Text = NULL;
	// Rely on assignment operator to perform deep copy
	*this = source;
}

// Assignment operator
String& String::operator = (const String& source)
{
	// Delete old text
	if (Text == NULL)
		delete [] Text;

	// Allocate new C-array
	Text = new char[source.GetLength()];

	// Copy all elements from source to this Text
	int i = 0;
	for ( ; i < source.GetLength(); i++)
		Text[i] = source[i];
	Text[i] = '\0';
	return *this;
}

// Assignment operator
String& String::operator = (char* source)
{	
	// Delete old text
	delete [] Text;

	// Allocate new C-array
	Text = new char[GetLength(source)];

	// Copy all elements from source to this Text
	int i = 0;
	for ( ; source[i] != '\0'; i++)
		Text[i] = source[i];
	Text[i] = '\0';
	return *this;
}

// Assignment operator from string to String
String& String::operator = (string source)
{
	// For performance
	int sourceLength = source.length();

	// Delete old text
	if (Text == NULL)
		delete [] Text;

	// Allocate new C-array
	Text = new char[sourceLength];

	// Copy all elements from source to this Text
	int i = 0;
	for ( ; i < sourceLength; i++)
		Text[i] = source[i];
	Text[i] = '\0';
	return *this;
}

// Destructor
String::~String()
{
	if (Text == NULL)
		delete[] Text;
}

// index operator
char& String::operator [] (int index) const
{
	int size = GetLength();
	if ( (index < 0) || (index >= size) )
	{
		stringstream error;
		error << "Vector::operator[] Index " << index << " out of bounds (0.." << (size-1) << ")";
		throw error.str();
	}
	return this->Text[index];
}

// Equality operator returns true when this string is the same length 
// as compareTo and all characters are the same (case insensitive)
bool String::operator == (const String& compareTo) const
{
	char* text = GetText();
	char* compareToText = compareTo.GetText();
	for (int i = 0; compareToText[i] != '\0'; i++)
	{
		// make both characters the same case to make case insensitive
		char aChar = tolower( text[i] );
		char compareToChar = tolower( compareToText[i] );
		if ( aChar != compareToChar )
			return false;
	}
	return true;
}

bool String::operator != (const String& compareTo) const
{
	if ( compareTo == *this )
		return false;
	return true;
}

// Concatenation operator returns a new string, which is
// a result of concatenation of this string with aString
String String::operator + (const String& aString) const
{
	// For performance sake
	int thisLength = GetLength();
	int aStringLength = aString.GetLength();

	// 1) Create a result string big enough to hold all characters 
	// of this string plus the characters of aString
	String result(thisLength + aStringLength +1);

	// 2) Copy all characters from this string into result
	// starting at index 0
	int index = 0;
	for ( ; index != '\0'; index++ )
		result[index] = this->Text[index];

	// 3) Copy all characters from aString into result starting at index GetLength()
	index++;	// start at next index from last char in this String
	int i = 0;
	for ( ; i != '\0'; i++ )
	{
		result[index] = aString[i];
		index++;
	}	

	// 4) Insert null-terminator
	result[thisLength + aStringLength] = '\0';

	return result;
}

// Returns an index of aChar within this string starting
// the search from startPosition; when aChar is not found the 
// function returns NOT_FOUND

int String::Find(char aChar, int startPosition)
{
	char* text = GetText();
	for ( int i = startPosition; text[i] != '\0'; i++ )
		if (text[i] == aChar)
			return i;
	return NOT_FOUND;
}

// Returns a substring of this string starting at startPosition
// and containing length characters; when length is not specified
// (length = 0) all characters starting from startPosition until 
// end of string are returned
String String::Substring(int startPosition, int length/* = 0*/)
{

	if (length == 0)
	{
		String aSubString( GetLength() + 1 );
		int i = startPosition;
		for ( ; this->Text[i] != '\0'; i++ )
			aSubString[i-startPosition] = this->Text[i];
		aSubString[i-startPosition] = '\0';

		return aSubString;
	}		

	String aSubString(length + 1);
	int i = startPosition;
	for ( i; ( i < (startPosition + length) && this->Text[i] != '\0' ); i++ )
		aSubString[i-startPosition] = this->Text[i];
	aSubString[i-startPosition] = '\0';

	return aSubString;
}

// Returns the count of characters in this string by scanning 
// for null-terminator
int String::GetLength() const
{
	return GetLength(Text);
}

// Returns length of a C-string passed as argument
/*static*/int String::GetLength(char* text)
{
	int count = 0;
	for ( ; text[count] != '\0'; count++);
	return count;
}

// copies length characters from source to destination
/*static*/void String::Copy(char* source, char* destination, int length)
{
	int sourceLength = GetLength(source);
	int destinationLength = GetLength(destination);
	if ( destinationLength < sourceLength + length )
	{
		stringstream error;
		error << "String::Copy member function --> destination string '" << destination << "' is not large enough to hold " << 
			length << " characters from source string '" << source << "'" << endl;
		throw error.str();
	}

	// scan for null terminator in destination
	int nullTerminatorIndex = 0;
	for ( ; source[nullTerminatorIndex] != '\0'; nullTerminatorIndex++);

	int destinationIndex = nullTerminatorIndex;

	// add source to destination
	for (int i = 0; source[i] != '\0'; i++)
	{
		destination[destinationIndex] = source[i];
		destinationIndex++;
	}
	destination[destinationIndex] = '\0';
}

//return encapsulaterd C-string
char* String::GetText() const
{
	return Text;
}

// Stream output operator
std::ostream& operator << (std::ostream& out, const String& aString)
{
	return out << aString.GetText();
}