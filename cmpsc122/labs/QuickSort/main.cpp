// main.cpp - quicksort main entry point
// Written by Brian Golden

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define NUMBER_OF_ENTRIES 5

/*
QuickSort Function
QuickSort Recursive Algorithm:
If the array to sort is only 1 element long then return (base case)
Partition the array and return the pivot index such that all elements to the left of pivot are smaller than the pivot, and values to the right of the pivot are larger than the pivot
QuickSort array to the left of pivot (pivot inclusive)
QuickSort array to the right of pivot (pivot exclusive).
*/

template<typename T> void QuickSort(vector<T>& myVector, int start, int size)
{
	int newLocation = Partition(myVector, start, size);

	// base case
	if(start - newLocation == 1)
		return;

	// Left of pivot (pivot inclusive)
	if (start < newLocation )
		QuickSort(myVector, start, newLocation);
	// Right of pivot (pivot exclusive)
	if (newLocation+1 < size)
		QuickSort(myVector, newLocation+1, size);
}

template<typename T> int Partition(vector<T>& myVector, int start, int size)
{
	int upIndex = start;
	int downIndex = size;
	T pivot = myVector[start];
	while (upIndex <= downIndex)
	{
		// Scans up the vector comparing pivot to the value
		// in the vector checking if the value is less than pivot
		// if true, the left index is incremented
		while (myVector[upIndex] < pivot)
			upIndex++;

		// scans down the vector comparing pivot to the value
		// in the vector checking if the value is greater than pivot
		// if true, the right index is incremented
		while (myVector[downIndex] > pivot)
			downIndex--;

		// If up < down then we swap the larger and
		// the smaller values and repeat the process from above
		// we break out of the loop.

		if (upIndex < downIndex)
		{
			swap(myVector[upIndex],myVector[downIndex]);
			upIndex++;
			downIndex--;
		}
		else
			break;
	}

	swap(myVector[start],myVector[downIndex]);
	return downIndex;
}

void main()
{	
	vector<string> myVector;
	string input;

	cout << "Enter " << NUMBER_OF_ENTRIES << " strings to put into a vector: " << endl;

	for(int i = 0; i < NUMBER_OF_ENTRIES; i++)
	{
		getline(cin, input);
		myVector.push_back(input);
	}
	cout << endl;

	QuickSort<string>(myVector, 0, myVector.size()-1);

	cout << "sorted vector: " << endl;
	for(int i = 0; i < NUMBER_OF_ENTRIES; i++)
		cout << myVector[i] << endl;

	system("pause");
}