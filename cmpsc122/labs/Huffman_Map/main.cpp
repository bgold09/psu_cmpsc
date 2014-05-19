// main.cpp - Huffman map main entry point
//Written by Brian Golden

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "PriorityQueue.h"
#include "TreeNode.h"
#include "SymbolPriority.h"
using namespace std;

typedef map<char, string> HuffmanMap;

using namespace std;

// MakeTree function
TreeNode<SymbolPriority>* MakeTree(const string& message)
{
	map<char, int> temp;

	// Count occurences of each character and assign priority
	for(int i = 0; i < int(message.length()); i++)
	{
		int priority = 1;
		temp[message[i]]++;
	}
	
	PriorityQueue<SymbolPriority> priorityQueue;
	
	// Push temp into priorityQueue
	map<char, int>::iterator pos = temp.begin();
	for(int i = 0; i < int(temp.size()); i++)
	{
		SymbolPriority tempSymbol;
		tempSymbol.Symbol = pos->first;
		tempSymbol.Priority = pos->second;
		priorityQueue.Push(tempSymbol);
		pos++;
	}

	PriorityQueue<TreeNode<SymbolPriority>*> huffman;
	while(priorityQueue.Size() > 0)
	{
		huffman.Push( new TreeNode<SymbolPriority>(priorityQueue.Top()) );
		priorityQueue.Pop();
	}

	do
	{
		TreeNode<SymbolPriority>* left = huffman.Top();
		huffman.Pop();
		if(!huffman.IsEmpty())
		{
			TreeNode<SymbolPriority>* right = huffman.Top();
			huffman.Pop();
			TreeNode<SymbolPriority>* root = new TreeNode<SymbolPriority>(SymbolPriority('*',left->Value.Priority + right->Value.Priority));
			root->Left = left;
			root->Right = right;
			huffman.Push(root);
		}
		else
		{
			huffman.Push(left);
		}	
	}
	while(huffman.Size()>1);
	
	return huffman.Top(); 
}

void MakeHuffmanMap(HuffmanMap& huffmanMap, TreeNode<SymbolPriority>* root, string code)
{
	// Base case
	if(root->IsLeaf())
	{
		huffmanMap.insert(pair<char,string>(root->Value.Symbol, code));
	}
	else
	{
		MakeHuffmanMap(huffmanMap, root->Left, code+'0');
		MakeHuffmanMap(huffmanMap, root->Right, code+'1');
	}
}

// Takes message and encodes each char based on the huffman tree
string Encode(const string& message, HuffmanMap& huffmanMap, TreeNode<SymbolPriority>* huffman)
{
	string result = "";
	for(int i =0; i < int(message.size()); i++)
	{
		if((huffman->Left == NULL) && (huffman->Right == NULL))
			result += '0';
		else
		{
			MakeHuffmanMap(huffmanMap, huffman, "");
			result += huffmanMap[message[i]];
		}
	}
	return result;
}

// Takes in huffman code and returns key value
string Decode(const string& code, TreeNode<SymbolPriority>* huffman)
{
	string result;
	TreeNode<SymbolPriority>* tTree = huffman;
	for (int i = 0; i < int(code.length()); i++)
	{
		if (code[i] == '0')
			tTree = tTree->Left;
		else
			tTree = tTree->Right;

		if (tTree->IsLeaf())
		{
			result += tTree->Value.Symbol;
			tTree = huffman;
		}
	}
	return result;
}

void main()
{
	string input;
	cout << "Please enter a sentence: " << endl;
	getline(cin, input);

	// Make huffman tree
	TreeNode<SymbolPriority>* lastValue = MakeTree(input);
	
	// Map the codes and encode message
	HuffmanMap huffmanMap;
	string code = Encode(input, huffmanMap, lastValue);
	cout << "The encoded message is: " << code << endl;

	// Decode message
	string decode = Decode(code, lastValue);
	cout << "The decoded message is: " << decode << endl;

	system("pause");
}