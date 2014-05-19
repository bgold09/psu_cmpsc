// main.cpp - main program entry point
// Written by Brian Golden

#include <list>
#include <string>
#include <iostream>
using namespace std;

void main ()
{
	list<string> myList;
	list<string>::iterator pos = myList.begin();
	string aString;

	// Enter strings into myList
	cout << "Enter 5 strings to put in linked list: ";
	for (int i = 0; i <= 5; i++)
	{	
		cin >> aString;
		// If there are no elements in the string, insert aString
		if ( myList.begin() == myList.end() )
		{
			myList.insert(pos, aString);
			continue;
		}
		
		// Traverse list and find where aString should be inserted
		for ( pos = myList.begin(); pos != myList.end(); pos++ )
		{
		   // Current element
		   string currentElement = *pos;

		   // If string is before currentElement in alphabetic order break
		   // out of loop to insert string
		   if (aString < currentElement)
			   break;  
		}

		myList.insert(pos, aString);
	}

	// Print sorted elements
	for ( list<string>::iterator pos = myList.begin(); pos != myList.end(); ++pos )
		   cout << *pos << " ";
	cout << '\n';

	system("pause");
}