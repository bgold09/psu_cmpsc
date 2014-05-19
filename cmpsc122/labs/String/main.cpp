// main.cpp - String class test application
// Written by Brian Golden

#include <iostream>
#include "String.h"
using namespace std;

#define NOT_FOUND	-1

void main ()
{
	try 
	{
		String myString_1(13);
		myString_1 = "String test";
		cout << "Should print 'String test': " << myString_1 << endl << endl;

		//String myString_2(myString_1);
		String myString_2(13);
		myString_2 = "String test";
		cout << "myString_1 = '" << myString_1 << "' and myString_2 = '" << myString_2 << "' and should return as being equal: ";
		if ( myString_1 == myString_2 )
			cout << "Strings are equal." << endl << endl;
		else
			cout << "Strings are not equal." << endl << endl;

		String myString_3(12);
		myString_3 = "string TEST";
		cout << "myString_1 = '" << myString_1 << "' and myString_3 = '" << myString_3 << "' and should return as being not equal: ";
		if ( myString_1 != myString_3 )
			cout << "Strings are not equal." << endl << endl;
		else
			cout << "Strings are equal." << endl << endl;

		String myString_4(7);
		myString_4 = myString_1.Substring(0, 6);
		cout << "Should print 'String': " << myString_4 << endl << endl;

		String myString_5(6);
		myString_5 = myString_1.Substring(7);
		cout << "Should print 'test': " << myString_5 << endl << endl;

		String myString_6(12);
		myString_6 = myString_4 + myString_5;
		cout << "Adding 'String' to 'test': " << myString_6 << endl << endl;

		cout << "Find character 'n' in 'String' from beginning of 'String test': "; 
		if ( myString_1.Find('n') == NOT_FOUND )
			cout << "Character was not found in given range." << endl << endl;
		else 
			cout << myString_1.Find('n') << endl << endl;

		cout << "Find character 'n' in 'String' from 6th element of 'String test': ";
		if ( myString_1.Find('n', 5) == NOT_FOUND )
			cout << "Character was not found in given range." << endl << endl;
		else 
			cout << myString_1.Find('n', 5) << endl << endl;
	}

	catch (string error)
	{
		 cout << "error" << endl;
	}
	
	system("pause");
}