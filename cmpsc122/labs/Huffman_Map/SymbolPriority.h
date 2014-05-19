// SymbolPriority.h - SymbolPriority struct implementation
// Written by Brian Golden

#pragma once
#include<iostream>
using namespace std;

struct SymbolPriority
{
	// Default Constructor
	SymbolPriority() {}

	SymbolPriority(char symbol, int priority)
	{
		Symbol = symbol;
		Priority = priority;
	}

	char Symbol;
	int Priority;

	// Comparison operators
	bool operator > (const SymbolPriority& compareTo)
	{
		return Priority > compareTo.Priority;
	}
	bool operator < (const SymbolPriority& compareTo)
	{
		return Priority < compareTo.Priority;

	}
	bool operator == (const SymbolPriority& compareTo)
	{
		return Priority == compareTo.Priority;
	}
	bool operator <= (const SymbolPriority& compareTo)
	{
		return Priority <= compareTo.Priority;
	}
};

