// main.cpp - Email checker main program entry point
// Written by Brian Golden
// Final Version 31 March 2011

#include <iostream>
#include <fstream>
#include <string>
#include "String.h"
#include "Email.h"
using namespace std;

// Preload valid tlds into Vector of Strings
Vector<String> PreloadTlds()
{
	Vector<String> temp;

	// - open ICANN list
	ifstream tld_list ("tld_list.txt");

	// - while we are not at end of tld_list file
	while ( !tld_list.eof() )
	{
		string valid_Tld_string;
		getline(tld_list, valid_Tld_string);	// Read TLD from tld_list file

		String valid_Tld;
		valid_Tld = valid_Tld_string;			// convert from string to String

		temp.Add(valid_Tld);					// Add to list of valid Tlds
	}
	return temp;
}

void main()
{
	Vector<String> validTlds = PreloadTlds();

	// 1. Open input file, open output file
	ifstream input("Email.txt");
	ofstream output("Result.txt");

	// 2. While we are not at the end of input file
	while ( !input.eof() )
	{
		// Get emailAddress from file
		string str_emailAddress;
		getline(input, str_emailAddress);

		// convert from string to String
		String emailAddress;
		emailAddress = str_emailAddress;			

		Email email(emailAddress, validTlds);

		// Validate and output to file
		output << email.IsValid() << "	" << emailAddress << endl;
	}

	system("pause");
}