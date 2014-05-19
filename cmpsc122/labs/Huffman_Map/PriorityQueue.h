// PriorityQueue.h - Priority Queue implementation
// Written by Brian Golden

#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include "SymbolPriority.h"
#include "TreeNode.h"
using namespace std;

template<typename T> class PriorityQueue
{
public:
	// Default constructor
	PriorityQueue() {}

	// Return the first element in the queue
	const T& Top() const
	{
		if(QueueData.size() == 0)
		{
			stringstream error;
			error << "ERROR: Priority Queue is empty";
			throw error.str().c_str();
		}
		return QueueData[0];
	}

	// Returns true if QueueData is empty
	bool IsEmpty() const
	{
		if(QueueData.size() == 0)
			return true;
		else
			return false;
	}

	// Pop function
	void Pop()
	{
		if(QueueData.size() == 0)
		{
			return;
		}

		if(QueueData.size() == 1)
		{
			QueueData.pop_back();
			return;
		}

		if(QueueData.size() > 1)
		{
			// Swap first & last element and pop out last element
			swap(QueueData[0], QueueData[QueueData.size()-1]);
			QueueData.pop_back();
			int parent = 0;
			// Sort elements
			while (true)
			{
				//set up elements
				int leftChild = 2*parent+1;
				int rightChild = leftChild + 1;
				int minChild;

				if ( leftChild >= int(QueueData.size()) )
				{
					break;
				}

				//look for both children to find the max child
				if ( (rightChild < int(QueueData.size())) && (QueueData[leftChild] <= QueueData[rightChild]) )
					minChild = leftChild;
				else if ( rightChild > int(QueueData.size()-1) )
					minChild = leftChild;
				else
					minChild = rightChild;

				if ( QueueData[parent] > QueueData[minChild] )
				{
					swap(QueueData[parent],QueueData[minChild]);
					parent = minChild;
				}
				else break;
			}
		}
	}

	// Adds new T value to QueueData and resorts
	void Push(const T& value)
	{
		QueueData.push_back(value);
		int child = QueueData.size() - 1;
		int parent = (child-1)/2;
		while((parent >= 0) && (QueueData[parent] > QueueData[child]))
		{
			swap(QueueData[parent], QueueData[child]);
			child = parent;
			parent = (child-1)/2;
		}

	}

	int Size()
	{
		return QueueData.size();
	}

private:
	vector<T> QueueData;
};