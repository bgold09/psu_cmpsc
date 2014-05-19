// String.h - String class declaration
// Written by Brian Golden

#pragma once

#include <iostream>
#include <string>
using std::string;

#define NOT_FOUND	-1

class String
{
public:
	// Default constructor that doubles as init
	// When size = 0 a NULL string is created, else a 
	// string of size characters is allocated
	String (int size = 0);

	// Copy constructor: performs deep copy
	String(const String& source);

	// Assignment operator: performs deep copy
	String& operator = (const String& source);

	// Assignment operator
	String& operator = (char* source);

	// Assignment operator from string
	String& operator = (string source);

	// destructor
	~String();

	// Equality op returns truewhen strings are same lenght and 
	// all characters are the same
	bool operator == (const String& compareTo) const;
	bool operator != (const String& compareTo) const;

	// Concatenation operator returns a new string, which is
	//a result o fconcatenation of this string with aString
	String operator + (const String& aString) const;

	// Index operator
	char& operator [] (int index) const;

	// Returns an index of aChar within this string starting
	// the search from startPosition; when aChar is not found the 
	// function returns NOT_FOUND
	int Find(char aChar, int startPosition = 0);

	// Returns a substring of this string starting at startPosition
	// and containing length characters; when length is not specified
	// (length = 0) all characters starting from startPosition until 
	// end of string are returned
	String Substring(int startPosition, int length = 0);

	// Returns the count of characters in this String by scanning 
	// for null-terminator
	int GetLength() const;

	// Returns length of a C-string passed as argument
	static int GetLength(char* text);

	// copies length characters from source to destination
	static void Copy (char* source, char* destination, int length);

	//return encapsulaterd C-string
	char* GetText() const;

private:
	// encapsulated C-string
	char* Text;
};

std::ostream& operator << (std::ostream& out, const String& aString);

// when you implement class, header file contains only declaration of members

// whenever pointer is data member, should provide copy contructor (DEEP COPY), assignment op (DEEP COPY), destructor