// main.cpp - Tree test application
// Written by Brian Golden

#include <iostream>
#include <string>
#include "TreeNode.h"
using namespace std;

template<typename T> TreeNode<T>* TraverseRight (TreeNode<T>*& root)
{
	if ( root->IsLeaf() || root->Right == NULL )
		return root;
	else 
		return TraverseRight(root->Right);
}

template<typename T> void Print(TreeNode<T>* root)
{
	if ( root == NULL )
		return;
	else
	{
		Print(root->Left);
		cout << root->Data << " ";
		Print(root->Right);
	}
}

template<typename T>void Insert(TreeNode<T>*& root, const T& insertData)
{
	if (root == NULL)
		root = new TreeNode<T> (insertData);
	else
	{
		if (root->Data == insertData)
			return;
		else if (root->Data > insertData)
			Insert(root->Left, insertData);
		else
			Insert(root->Right, insertData);
	}
}

template<typename T>void Delete(TreeNode<T>*& root, const T& searchData)
{
	if (root == NULL)
		return;
	if (root->Data > searchData)
		Delete(root->Left, searchData);
	else if (root->Data < searchData)
		Delete(root->Right, searchData);
	else
	{
		// Case 1
		if ( root->IsLeaf() )
		{
			delete root;
			root = NULL;
		}
		// Case 2
		else if ( root->hasOneChild() )
		{
			TreeNode<T>* temp = root;
			delete root;

			if (root->Left != NULL)
				root = temp->Left;
			else if (root->Right != NULL)
				root = temp->Right;			
		}
		// Case 3
		else 
		{
			TreeNode<T>* rightMostNode = TraverseRight(root);
			TreeNode<T>* temp = rightMostNode;
			root->Data = rightMostNode->Data;
			Delete(root->Left, rightMostNode->Data);
		}
	}
}

void main()
{
	string wordToInsert, wordToDelete;

	TreeNode<string>* treeRoot = NULL;
	TreeNode<string>* currentNode = treeRoot;

	// Build a binary tree for the sentence
	cout << "Enter expression: ";
	while ( cin.peek() != '\n' )
	{
		cin >> wordToInsert;
		Insert<string> (treeRoot, wordToInsert);
	}

	Print<string> (treeRoot);

	cout << "Enter a word to delete: ";
	cin >> wordToDelete;

	Delete(treeRoot, wordToDelete);

	Print<string> (treeRoot);

	system("pause");
}