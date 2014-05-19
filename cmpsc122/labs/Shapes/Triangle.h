// Triangle.h - Triangle class declaration
// Written by Brian Golden

#pragma once
#include <iostream>
#include "Shape.h"
using namespace std;

// 'public Shape' makes Triangle a derived class
// Triangle is derived class of Shape (aka sublass of Shape
// or child class of Shape). Shape is the base class
// (aka superclass or parent class)
class Triangle: public Shape
{
public:
	// Init-constructor, calls base class constructor
	Triangle(int x, int y, int x2, int y2, int x3, int y3) : Shape(x, y)
	{
		X2 = x2;
		Y2 = y2;
		X3 = x3;
		Y3 = y3;
	}

	// Overload pure virtual function
	virtual void Resize(float scale)
	{
		// vertex 1 (X, Y) will be fixed
		X2 = int(X2*scale);
		Y2 = int(Y2*scale);
		X3 = int(X3*scale);
		Y3 = int(Y3*scale);
	}

	// Don't need 'virtual' but emphasizes that it 
	// is an overloaded function
	virtual void Move(int deltaX, int deltaY)
	{
		// Call base class Move function
		Shape::Move(deltaX, deltaY);

		X2 += deltaX;
		Y2 += deltaY;
		X3 += deltaX;
		Y3 += deltaY;
	}

	virtual void Draw() const
	{
		cout << "Triangle with vertices at (" << X << ", " << Y << "), (" <<
			X2 << ", " << Y2 << "), (" << X3 << ", " << Y3 << "), and color = " 
			<< GetColor() << endl;
	}

private:
	// Data members
	int X2, Y2;	  // vertex 2
	int X3, Y3;   // vertex 3
	// X, Y, ShapeColor are inherited from Shape
};