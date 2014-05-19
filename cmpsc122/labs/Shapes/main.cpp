// main.cpp - main program entry point
// Written by Brian Golden

#include <iostream>
#include <string>
#include <vector>
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"

using namespace std;

void main ()
{
	try
	{
		// Container, holder of shapes
		vector<Shape*> shapes;

		// Populate container with shapes
		shapes.push_back(new Circle(1, 1, 10));
		shapes.push_back(new Circle(10, 10, 20));

		shapes.push_back(new Rectangle(1, 2, 3, 4));
		shapes.push_back(new Rectangle(2, 4, 6, 8));

		shapes.push_back(new Triangle(0, 0, 3, 5, 3, 0));
		shapes.push_back(new Triangle(1, 3, 4, 5, 6, 11));

		// Set colors
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			string color;
			cout << "Enter color {red, green, blue, black, white}: ";
			cin >> color;

			shapes[i]->SetColor(color);
		}

		// Move shapes
		for (unsigned int i = 0; i < shapes.size(); i++)
			shapes[i]->Move(10, 10);

		float scale; 
		cout << "Enter scale to resize: ";
		cin >> scale;

		// Resize shapes
		for (unsigned int i = 0; i < shapes.size(); i++)
			shapes[i]->Resize(scale);

		//Draw circles
		for (unsigned int i = 0; i < shapes.size(); i++)
			shapes[i]->Draw();
	}

	catch (string error)
	{
		cout << error << endl;
	}

	system("pause");
}