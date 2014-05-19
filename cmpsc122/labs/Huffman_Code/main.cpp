// main.cpp - Tree test application
// Written by Brian Golden

#include <iostream>
#include <string>
#include "PriorityQueue.h"
#include "TreeNode.h"
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

bool characterExists (vector<SymbolPriority> myVector, char aChar)
{
	for (int i = 0; i < int(myVector.size()); i++)
		if (myVector[i].Symbol == aChar)
			return true;
	return false;
}

string HuffmanCode(char symbol, TreeNode<SymbolPriority>* root, string code)
{
	if ( root->Data.Symbol == symbol ) 
		return code; 
	else 
		return "";


	string code = "";
	string temp = HuffmanCode(huffman->Data.Symbol, huffman->Left, code + "0"); 
	if ( temp == "" ) 
		return HuffmanCode(huffman->Data.Symbol, huffman->Right, code+"1");
	else 
		return code += temp;


}

string Encode(const string& message, const TreeNode<SymbolPriority>* huffman)
{
	string code = "";
	string temp = HuffmanCode(huffman->Data.Symbol, huffman->Left, code + "0"); 
	if ( temp == "" ) 
		return HuffmanCode(huffman->Data.Symbol, huffman->Right, code+"1");
	else 
		return code += temp;
}


string Decode(const string& code, const TreeNode<SymbolPriority>* huffman)
{
	if (huffman->IsLeaf())
		return huffman->Data;
	else 
	{


	}

}


// Builds the tree
TreeNode<SymbolPriority>* MakeTree(const string& message)
{
	char										currentChar;
	vector<SymbolPriority>						temp;
	PriorityQueue<TreeNode<SymbolPriority>*>	myPriorityQueue;

	for (int i = 0; i < int(message.size()); i++)
	{
		currentChar = message[i];

		if ( temp.empty() )
			temp.push_back( SymbolPriority(currentChar, 1) );
		else if  ( characterExists(temp, currentChar) )
		{
			for (int c = 0; c < int (temp.size()); i++)
				if (currentChar == temp[i].Symbol)
					temp[i].Priority++;
		}
		else
			temp.push_back( SymbolPriority(currentChar, 1) );
	}


	for (int i = 0; i < int(temp.size()); i++)
	{
		if (myPriorityQueue.GetSize() <= 1)
			myPriorityQueue.Push( new TreeNode<SymbolPriority>( temp[i]) );

		else
		{
			char aChar; 
			TreeNode<SymbolPriority>* tempNode;
			// create a new TreeNode<SymbolPriority>* and
			// make the first popped element its left child
			// make the second popped element its right child
			// set its value to the sum of its left and right child priorities

			tempNode->Left = myPriorityQueue.Top();
			aChar = myPriorityQueue.Top()->Data.Priority;
			myPriorityQueue.Pop();

			tempNode->Right = myPriorityQueue.Top(); 
			aChar += myPriorityQueue.Top()->Data.Priority;
			myPriorityQueue.Pop();

			myPriorityQueue.Push( tempNode );
		}
	}

	return myPriorityQueue.Top();
}

void main()
{
	string										message;
	char										aChar;
	vector<SymbolPriority>						temp;
	PriorityQueue<TreeNode<SymbolPriority>*>	myPriorityQueue;

	cout << "Enter sentence: ";
	getline(cin, message);

	// Build a Huffman Tree for the message	
	TreeNode<SymbolPriority>* root = MakeTree(message);

	// Encode sentence into Huffman Code (bit string)

	// Decode Huffman code back to original sentence


	system("pause");
}