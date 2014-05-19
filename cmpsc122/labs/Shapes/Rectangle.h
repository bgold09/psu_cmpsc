// Rectangle.h - Rectangle class declaration
// Written by Brian Golden

#pragma once
#include <iostream>
#include "Shape.h"
using namespace std;

class Rectangle: public Shape
{
public:
	// Init-constructor
	Rectangle(int x, int y, int x2, int y2) : Shape(x, y)
	{
		X2 = x2;
		Y2 = y2;
	}

	// Don't need 'virtual' but emphasizes that it 
	// is an overloaded function
	virtual void Move(int deltaX, int deltaY)
	{
		// Call base class Move function
		Shape::Move(deltaX, deltaY);

		X2 += deltaX;
		Y2 += deltaY;
	}

	void Resize(float scale)
	{
		int width = X2 - X;
		int height = Y2 - Y;

		// Top-left corner will be fixed
		X2 = int(X + width*scale);
		Y2 = int(Y + width*scale);
	}

	void Draw() const
	{
		cout << "RECTANGLE with corners (" << X << ", " << Y << "), (" << 
			X2 << ", " << Y2 << ") and color = " << GetColor() << endl;
	}

private:
	// Data members
	// X, Y inherited --> top-left corner
	int X2, Y2; // Bottom-left corner
	// ShapeColor inherited
};