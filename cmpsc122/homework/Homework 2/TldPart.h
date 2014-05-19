// TldPart.h - email local part class declaration
// Written by Brian Golden

#pragma once

#include <string>
using namespace std;

class TldPart
{
public:
	TldPart() {}

	// Sets the address
	void Set(const string& address);

	// Performs TLD validation
	bool IsValid();
		
	// Returns length of input text; will be TLD from 
	// list or from Address
	int GetLength(string text);


private:
	// TldPart address
	string Address;
};