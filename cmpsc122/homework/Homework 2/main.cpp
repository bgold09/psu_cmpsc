// main.cpp - Email checker main program entry point
// Written by Brian Golden
// Final Version 03 March 2011

#include <iostream>
#include <fstream>
#include <string>
#include "Email.h"

using namespace std;

void main()
{
	// 1. Open input file, open output file
	ifstream input("Email.txt");
	ofstream output("Result.txt");

	// 2. While we are not at the end of input file
	while ( !input.eof() )
	{
		// Get emailAddress from file
		string emailAddress;
		getline(input, emailAddress);

		Email email(emailAddress);
		output << email.IsValid() << endl;
	}
	system("pause");
}