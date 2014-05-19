// main.cpp - C-string test program
// Written by Brian Golden

#include <iostream>
using namespace std;

// Pascal Name
// const int MaxSize = 100;

#define MAX_SIZE 100

// calculate length of a C-string by scanning for the 
// occurance of a null-terminator
// char* means a C-string
int GetLength (char* text)
{
	int pos = 0;
	for ( ; text[pos] != '\0'; pos++);
	return pos;
}

// copies all chars from source to desitination starting at startPosition
void Copy(char* source, char* destination, int startPosition)
{
	// copy chars from source to destination
	int i = 0;
	for ( ; source[i] != '\0'; i++)
		destination[startPosition + i] = source[i];

	// copy null terminator
	destination[startPosition + i] = '\0';
}


void main ()
{
	char word1[MAX_SIZE];
	char word2[MAX_SIZE];
	char sentence[MAX_SIZE];

	//prompt user for word1
	cout << "Enter word1: ";
	cin.getline(word1, MAX_SIZE);

	//promt user for word2
	cout << "Enter word2: ";
	cin.getline(word2, MAX_SIZE);

	// merge words: word1 + ' ' + word2 to form a sentence

	// determine length of word1 (length1)
	int length1 = GetLength(word1);

	// determine length of word2 (length2)
	int length2 = GetLength(word2);

	// safeguard against buffer overrun
	// + 2 accounts for space and null-terminator
	if (length1 + length2 + 2 > MAX_SIZE)
	{
		cout << "sentence is not large enough for word1 + ' ' + word2" << endl;
		system ("pause");
		return;
	}

	// 1) copy all characters from word1 into sentence starting at position 0
	Copy(word1, sentence, 0);
	
	// 2) Put space in sentence at position length1
	sentence[length1] = ' ';

	// 3) Copy all characters from word2 into sentence starting
	//    at position length1 + 1
	Copy(word2, sentence, length1 + 1);
	
	cout << "The sentence is: " << sentence << endl;

	system("pause");
}