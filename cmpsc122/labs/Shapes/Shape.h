// Shape.h - Shape class declaration
// Written by Brian Golden

#pragma once
#include <iostream>
#include <string>
using namespace std;

// enum declares user-defined primitive type, which
// can be set only through the defined values
enum Color {Red, Green, Blue, Black, White};

// Shape is an abstract base class
class Shape
{
public:
	// Init-constructor
	Shape(int x, int y)
	{
		X = x; 
		Y = y;
		ShapeColor = White;
	}

	void SetColor(const string& color)
	{
		if ( color == "red" )
			ShapeColor = Red;
		else if ( color == "green" )
			ShapeColor = Green;
		else if ( color == "blue" )
			ShapeColor = Blue;
		else if ( color == "black" )
			ShapeColor = Black;
		else if ( color == "white" )
			ShapeColor = White;
		else
		{
			string error = "Shape::SetColor(): invalid color ";
			error += color;
			throw error;
		}
	}

	string GetColor() const
	{
		switch( ShapeColor )
		{
		case Red:
			return "red";
		case Green:
			return "green";
		case Blue:
			return "blue";
		case Black:
			return "black";
		case White:
			return "white";
		default:
			{
			string error = "Shape::GetColor(): unknown color";
			throw error;
			}
		}
	}

	// virtual means that the member function CAN 
	// be overloaded in a derived class
	virtual void Move(int deltaX, int deltaY)
	{
		X += deltaX;
		Y += deltaY;
	}

	// = 0 means 'pure virtual' which means function 
	// MUST be overloaded. A class with pre virtual
	// functions is called abstract base class
	virtual void Resize(float scale) = 0;

	virtual void Draw() const = 0;

// protected is similar to private in the sense that you 
// CANNOT access protected members outside the base class
// or derived classes (derived classes have access to protected members)
protected:
	// Data members
	int X, Y; // All shapes need at least one set of coordinates
	Color ShapeColor;
};