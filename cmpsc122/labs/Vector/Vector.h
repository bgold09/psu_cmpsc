// Vector.h - Vector template class declaration
// Written by Brian Golden

#pragma once

#include <sstream>
using namespace std;

#define INTIAL_CAPACITY		20
#define CAPACITY_BOOST		10

template<typename TYPE> class Vector
{
public:
	//default constructor
	Vector(int initialCapacity = INTIAL_CAPACITY)
	{
		Capacity = initialCapacity;
		Size = 0;
		//Allocate C-array dynamically on heap
		Data = new TYPE[Capacity];
	}
	//copy constructor: performs DEEP COPY
	Vector(const Vector& source)
	{
		// inside copy-const data members of the class are not
		// yet initialized; therefore when we do delete[] Data in the
		// assignment operator the operation mat fail because
		// Data is set to a garbage value
		Data = NULL;

		// let's assignment operator to perform deep copy
		*this = source;
	}

	// destructor
	~Vector()
	{
		// dispose memeory
		// delete [] means dispose an array
		delete [] Data;
	}
	
	Vector& operator = (const Vector& source)
	{
		Size = source.Size;
		Capacity = source.Capacity;

		// dispose old data
		delete[] Data;

		// allocate new Data
		Data = new TYPE[Capacity];

		//copy all elements from source to this Data
		for (int i = 0; i < Size; i++)
			Data[i] = source.Data[i];

		// assignment must always return refrence to this
		return *this;
	}

	int GetSize() const
	{
		return Size;
	}

	int GetCapacity() const
	{
		return Capacity;
	}

	// Removes the value at a given location
	void Remove (int index) 
	{	
		if ( (index < 0) || (index >= Size) )
		{
			stringstream error;
			error << "Vector::operator[] Index " << index << " out of bounds (0.." << (Size-1) << ")";
			throw error.str();
		}
		Data[index] = NULL;

		if (index != Size)
		{
			for (int i = index; i < Size-1; i++)
			{
				Data[i] = Data[i + 1];
			}

		}
		Size--;
	}

	// index operator
	// operator [] must return TYPE& if you want to use it 
	// on the left hand side of an assignment
	TYPE& operator [] (int index) const
	{
		if ( (index < 0) || (index >= Size) )
		{
			stringstream error;
			error << "Vector::operator[] Index " << index << " out of bounds (0.." << (Size-1) << ")";
			throw error.str();
		}
		return Data[index];
	}

	// add new value at end of vector
	void Add(const TYPE& newValue) 
	{
		// are we at max capacity?
		if (Size == Capacity)
		{
			// 1. Increment capacity
			Capacity += CAPACITY_BOOST;

			// 2. Allocate new C-array
			TYPE* newData = new TYPE[Capacity];

			// 3. Copy old C-array values into new C-array
			for (int i = 0; i < Size; i++ )
				newData[i] = Data[i];

			// dispose old data and swap pointers
			delete [] Data;
			Data = newData;
			Data[Size] = newValue;
			Size++;
		}
		else
		{
			// 3. Insert newValue
			Data[Size] = newValue;
			// 4. Increment Size
			Size++;
		}
	}

private:
	// the encapsulated C-array
	TYPE* Data;
	int Size, Capacity;
};

template<typename TYPE>
ostream& operator << (ostream& out, const Vector<TYPE>& aVector)
{
	// write for loop to output all vector elements
	
	//int Size = aVector.GetSize;
	//int Size = &Vector<TYPE> aVector::GetSize;
	int size = aVector.GetSize();
	for (int i = 0; i < size; i++)
		out << aVector[i] << " ";
	return out;
}