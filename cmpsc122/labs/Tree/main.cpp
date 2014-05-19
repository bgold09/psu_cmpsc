// main.cpp - Tree test application
// Written by Brian Golden

#include <iostream>
#include <string>
#include "TreeNode.h"
using namespace std;

// Evaluates the tree formed by the mathematical expression
float ValueOf(TreeNode<char>* root)
{
	if ( root->IsLeaf() )
		// convert character stored in value to its integer equivalent
		return float(root->Value - '0');
	else 
	{
		switch (root->Value)
		{	
		case ('+'):
			return ValueOf(root->Left) + ValueOf(root->Right);
		case ('-'):
			return ValueOf(root->Left) - ValueOf(root->Right);
		case ('*'):
			return ValueOf(root->Left) * ValueOf(root->Right);
		case ('/'):
			return ValueOf(root->Left) / ValueOf(root->Right);
		default:
			string error = "TreeNode::ValueOf(): invalid operand";
			throw error;
		}
	}
}

void main ()
{
	string expression;

	cout << "Enter expression: ";
	getline(cin, expression);
	
	TreeNode<char>* treeRoot = NULL;
	TreeNode<char>* currentNode = treeRoot;
	
	// Build a binary tree for the expression
	for (int i = 0; i < int(expression.length()); i++)
	{
		// for performance
		char currentChar = expression[i];
		
		if (currentChar >= '0' && currentChar <= '9')
		{
			//Create newLeaf node, set its value to 0..9
			TreeNode<char>* newLeaf = new TreeNode<char> (currentChar);

			if (currentNode == NULL)
				treeRoot = currentNode = newLeaf;
			elseif (currentNode->Right == NULL)  
				currentNode->Right = newLeaf;
			else 
				currentNode->Left = newLeaf;
		}

		// Change needed to accomodate for /, * when current 
		// node is / or *: /, * becomes parent node when current node is / or *
		else if ( (currentChar == '*' || currentChar == '/') && (currentNode->Value == '*' || currentNode->Value == '/') )
		{
			TreeNode<char>* newChild =  new TreeNode<char> (currentChar);
			newChild->Right = currentNode;
			currentNode = newChild;
			treeRoot = newChild;
		}

		// If you find + or – (or *,/ and currentNode->Right = NULL) then
		else if ( currentChar == '+' || currentChar == '-' || 
			( (currentChar == '*' || currentChar == '/') && currentNode->Right == NULL) ) 
		{
			// Create newRoot node, set its value the operation found
			TreeNode<char>* newRoot = new TreeNode<char> (currentChar);
			
			newRoot->Left = treeRoot;	// Set Left of newRoot to the current root
			treeRoot = newRoot;			// the root is now the new root (operation)
			currentNode = newRoot;
		}

		// If you find * or / then
		else if ( currentChar == '*' || currentChar == '/' )
		{
			// create newChild node, set its value to * or /
			TreeNode<char>* newChild =  new TreeNode<char> (currentChar);
			newChild->Left = currentNode->Right;
			currentNode->Right = newChild;
			currentNode = newChild;
		}
	}
	
	float expressionValue;
	char operation = treeRoot->Value;
	switch (operation)
	{	
	case ('+'):
		expressionValue = ValueOf(treeRoot->Left) + ValueOf(treeRoot->Right);
		break;
	case ('-'):
		expressionValue = ValueOf(treeRoot->Left) - ValueOf(treeRoot->Right);
		break;
	case ('*'):
		expressionValue = ValueOf(treeRoot->Left) * ValueOf(treeRoot->Right);
		break;
	case ('/'):
		expressionValue = ValueOf(treeRoot->Left) / ValueOf(treeRoot->Right);
		break;
	}

	cout << expressionValue << endl;

	system("pause");

}