// Circle.h - Circle class declaration
// Written by Brian Golden

#pragma once
#include <iostream>
#include "Shape.h"
using namespace std;

// 'public Shape' makes Circle a derived class
// Circle is derived class of Shape (aka sublass of Shape
// or child class of Shape). Shape is the base class
// (aka superclass or parent class)
class Circle: public Shape
{
public:
	// Init-constructor, calls base class constructor
	Circle(int x, int y, int radius) : Shape(x, y)
	{
		Radius = radius;
	}

	// Overload pure virtual function
	virtual void Resize(float scale)
	{
		// force explicit conversion to int
		// to avoid compiler warning
		Radius = int(Radius*scale);
	}

	virtual void Draw() const
	{
		cout << "CIRCLE at (" << X << ", " << Y << "), radius = " << Radius <<
			" and color = " << GetColor() << endl;
	}

private:
	// Data members
	int Radius;
	// X, Y, ShapeColor are inherited from Shape
};