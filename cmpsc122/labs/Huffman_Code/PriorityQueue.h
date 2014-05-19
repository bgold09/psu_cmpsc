//PriorityQueue.h - PriorityQueue class implementation
// Written by Brian Golden

#include <vector>
#include <string>
#include <sstream>
using namespace std;

#pragma once

template<typename T> class PriorityQueue
{
public:
	// Default constructor, does nothing
	PriorityQueue()
	{
		Size = QueueData.size();
	}

	// Returns first element in the vector
	const T& Top() const
	{
		if (QueueData.empty())
		{
			stringstream error;
			error << "PriorityQueue::Top(): QueueData is empty";
			throw error.str();
		}
		return QueueData[0];
	}

	// Returns true if the vectoc is empty
	bool IsEmpty() const
	{
		return QueueData.empty();
	}

	int GetSize()
	{
		return Size;
	}

	// index operator
	const T& operator [] (int index) const
	{
		if ( (index < 0) || ( index >= int(QueueData.size()) ) ) 
		{
			stringstream error;
			error << "PriorityQueue::operator[] Index " << index << " out of bounds (0.." << QueueData.size()-1 << ")";
			throw error.str();
		}
		return QueueData[index];
	}

	// Adds an element and sorts as a min heap
	void Push(const T& value)
	{
		if (Size == 0)
			QueueData.push_back(value);

		else if (Size == 1)
		{
			int child = QueueData.size() - 1;
			int parent = (child - 1)/2;
			while ( parent >= 0 && *QueueData[parent].Priority > *QueueData[child].Priority )
			{
				swap( QueueData[parent], QueueData[child] );
				child = parent;
				parent = (child - 1)/2;
			}
		}
		Size++;
	}

	// Removes the top element of the array and resorts as min heap
	void Pop()
	{
		vector<T>::iterator pos = QueueData.begin();
		QueueData.erase(pos);
		int child = QueueData.size() - 1;
		int parent = (child - 1)/2;
		while ( parent >= 0 && *QueueData[parent].Priority > *QueueData[child].Priority )
		{
			swap( QueueData[parent], QueueData[child] );
			child = parent;
			parent = (child - 1)/2;
		}
		Size--;
	}

private:
	vector<T> QueueData;
	int Size;
};