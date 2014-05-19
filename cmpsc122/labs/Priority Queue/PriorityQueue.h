//PriorityQueue.h - PriorityQueue class implementation
// Written by Brian Golden

#include <vector>
#include <string>
using namespace std;

#pragma once

template<typename T> class PriorityQueue
{
public:
	// Default constructor, does nothing
	PriorityQueue() {}

	// Returns first element in the vector
	const T& Top() const
	{
		if (QueueData.empty())
		{
			string error = "PriorityQueue::Top(): QueueData is empty";
			throw error;
		}
		return QueueData[0];
	}

	// Returns true if the vectoc is empty
	bool IsEmpty() const
	{
		return QueueData.empty();
	}

	int Size()
	{
		return QueueData.size();
	}

	// index operator
	T& operator [] (int index) const
	{
		if ( (index < 0) || (index >= Size) )
		{
			stringstream error;
			error << "PriorityQueue::operator[] Index " << index << " out of bounds (0.." << (Size-1) << ")";
			throw error.str();
		}
		return QueueData[index];
	}
	
	/*
	First insert the new element at the end of the priority queue vector then…
	child = vector.size() – 1
	parent = (child – 1)/2
	while ( parent >= 0 and vector[parent] < vector[child] )
	{
	swap(vector[parent], vector[child]);
	child = parent;
	parent = (child – 1)/2;
	}
	*/

	void Push(const T& value)
	{
		QueueData.push_back(value);
		int child = QueueData.size() - 1;
		int parent = (child - 1)/2;
		while ( parent >= 0 && QueueData[parent] > QueueData[child] )
		{
			swap( QueueData[parent], QueueData[child] );
			child = parent;
			parent = (child - 1)/2;
		}
	}

	// Removes the top element of the array
	void Pop()
	{
		vector<T>::iterator pos = QueueData.begin();
		QueueData.pop(pos);
		int child = QueueData.size() - 1;
		int parent = (child - 1)/2;
		while ( parent >= 0 && QueueData[parent] > QueueData[child] )
		{
			swap( QueueData[parent], QueueData[child] );
			child = parent;
			parent = (child - 1)/2;
		}
	}

private:
	vector<T> QueueData;
};