// main.cpp - Tree test application
// Written by Brian Golden

#include <iostream>
#include <string>
#include "PriorityQueue.h"
using namespace std;

struct SymbolPriority
{
	SymbolPriority(char symbol, int priority)
	{
		Symbol = symbol;
		Priority = priority;
	}

	char Symbol;
	int Priority;

	bool operator >  (const SymbolPriority& compareTo)
	{
		return Priority > compareTo.Priority;
	}

	bool operator <  (const SymbolPriority& compareTo)
	{
		return Priority < compareTo.Priority;
	}

	bool operator == (const SymbolPriority& compareTo)
	{
		return Priority == compareTo.Priority;
	}
};

ostream& operator << (ostream& out, const SymbolPriority& aSymPri)
{
	return out << "'" << aSymPri.Symbol << "' - " << aSymPri.Priority;

}

bool characterExists (vector<SymbolPriority> myVector, char aChar)
{
	for (int i = 0; i < int(myVector.size()); i++)
		if (myVector[i].Symbol == aChar)
			return true;
	return false;
}

void main()
{
	string							sentence;
	vector<SymbolPriority>			temp;
	PriorityQueue<SymbolPriority>	myPriorityQueue;

	// Build a priority queue for the sentence
	cout << "Enter sentence: ";
	getline(cin, sentence);

	for(int i = 0; i < int(temp.size()); i++)
	{
		char currentChar = sentence[i];

		if(!(characterExists(temp, currentChar)) || sentence.empty())
			temp.push_back(SymbolPriority(currentChar, 1));
		else
			for(int c = 0; c < int(temp.size()); c++)
				if(temp[c].Symbol == currentChar)
					temp[c].Priority++;
	}

	for (int i = 0; i < int(temp.size()); i++)
		myPriorityQueue.Push(temp[i]);

	// Print vector before popping


	// Print vector after popping

	system("pause");
}