// main.cpp - Tree test application
// Written by Brian Golden

#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
using namespace std;

#define NUM_OF_STRINGS	5

void main ()
{
	set<string> set1, set2, unionSet, intersectionSet, differenceSet;
	string input;

	cout << "Enter " << NUM_OF_STRINGS << " strings for set 1: ";
	for (int i = 0; i < NUM_OF_STRINGS; i++)
	{
		getline(cin, input);
		set1.insert(input);
	}
	
	cout << "Enter " << NUM_OF_STRINGS << " strings for set 2: ";
	for (int i = 0; i < NUM_OF_STRINGS; i++)
	{
		getline(cin, input);
		set2.insert(input);
	}
	cout << endl;

	// Calculates union, intersection, and difference of set1 and set2 and outputs the results;	
	set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(unionSet, unionSet.begin()));

	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersectionSet, intersectionSet.begin()));

	set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(differenceSet, differenceSet.begin()));
	
	// print result of each (union, intersection, difference)
	cout << "Union set: ";
	for (set<string>::iterator pos = unionSet.begin(); pos != unionSet.end(); pos++)
		cout << *pos << " ";
	cout << endl;

	cout << "Intersection set: ";
	for (set<string>::iterator pos = intersectionSet.begin(); pos != intersectionSet.end(); pos++)
		cout << *pos << " ";
	cout << endl;

	cout << "Difference set: ";
	for (set<string>::iterator pos = differenceSet.begin(); pos != differenceSet.end(); pos++)
		cout << *pos << " ";
	cout << endl;

	// Inputs a search string from cin and determines if it is present in the union of set1 and set2
	cout << "Enter a search query to see if it is present in the union of the sets: ";
	getline(cin, input);
	
	// Search union for a string
	set<string>::iterator itemLocation = unionSet.find(input);
	if (itemLocation != unionSet.end())
		cout << "The string " << input << " was found in the union set." << endl;
	else
		cout << "The string " << input << " was not found in the union set." << endl;
	
	system("pause");
}