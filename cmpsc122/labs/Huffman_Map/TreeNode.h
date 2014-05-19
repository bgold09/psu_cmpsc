//TreeNode.h - TreeNode implementation
// Written by Brian Golden

#pragma once
#include <iostream>
#include "SymbolPriority.h"
#include "PriorityQueue.h"

// TreeNode class
template<typename T> struct TreeNode
{
	T Value;
	TreeNode<T>* Left;
	TreeNode<T>* Right;

	TreeNode() {}

	TreeNode(const T& value, TreeNode<T>* left = NULL, TreeNode<T>* right = NULL)
	{
		Value = value;
		Left = left;
		Right = right;
	}

	bool IsLeaf() const
   {
      return Left == NULL && Right == NULL;
   }

	bool operator > (const TreeNode& compareTo)
	{
		return Value > compareTo.Value;
	}
	bool operator < (const TreeNode& compareTo)
	{
		return Value < compareTo.Value;

	}
	bool operator == (const TreeNode& compareTo)
	{
		return Value == compareTo.Value;
	}
};