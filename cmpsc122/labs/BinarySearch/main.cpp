// main.cpp - main program entry point
// Written by Brian Golden

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define NUMBER_OF_ENTRIES	5
#define EMPTY_VECTOR		0
#define NOT_FOUND			-1

template<class T> int BinarySearch(const vector<T>& aVector, const T& value, int startPos, int length)
{
   // Empty vector?
   if ( length == 0 ) 
	   return EMPTY_VECTOR; 
   // Base case
   if ( length == 1 )
      if ( aVector[startPos] >= value )
         return startPos; // matches or less than string at that pos
      else
         return startPos + 1;

   // Scan lower half
   if ( aVector[startPos + length/2] > value )
      return BinarySearch(aVector, value, startPos, length/2);
   // Else scan upper half
   else
      return BinarySearch(aVector, value, startPos + length/2, length - length/2);
}

void main()
{	
	vector<string>	myVector;
	string			entry;
	int				insertAtIndex;
	
	cout << "Enter " << NUMBER_OF_ENTRIES << " entries: ";
	for (int i = 0; i < NUMBER_OF_ENTRIES; i++)
	{
		// Get entry from user
		getline(cin, entry);

		insertAtIndex = BinarySearch( myVector, entry, 0, myVector.size() );

		// convert startPos to an iterator, required for 
		vector<string>::iterator pos = myVector.begin() + insertAtIndex;

		// Insert string at appropriate index
		myVector.insert(pos, entry);
	}

	// print the sorted vector
	for ( vector<string>::iterator pos = myVector.begin(); pos != myVector.end(); pos++ )
		cout << *pos << " ";
	cout << '\n';


	system("pause");
}

