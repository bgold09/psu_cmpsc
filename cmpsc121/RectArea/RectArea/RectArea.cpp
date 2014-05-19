// STEP 1: Complete the documentation below
//
// RectArea.cpp
// Written by:
// On the Date:
//
// This program computes the area of a rectangle given
// its total dimensions and the ratio between its length
// and width.  Along the way, it must determine what the
// actual measurements are.
//
// For example, some problems this could solve are:
//    The length is 2 * the width and the perimeter is 12.
//			What is the area?   (Answer: 8 for 2x4)
//	  The length is 3 * the width and the perimeter is 12.
//			What is the area?   (Answer: 6.75 for 1.5x4.5)
//
// Some of the relevant formulas for this program are
//    Area of a Rectangle = length * width
//    Perimeter of a Rectangle = 2*length + 2*width
//    if length = ratio * width, then Perimeter =
//    or solving for width, given Perimeter, width = 

// The following lines will be part of every C++ program in this course:
#include <iostream>
using namespace std;

// Every C++ program has a function named main()
int main()
{
	//  STEP 2:  Declare all of the variables needed for this program,
	//     including those for input, output, and intermediate calculations.
	
	int width, ratio, length, perimeter, area;

	// STEP 3:  Input values from the keyboard (there are two inputs).
	//    Be sure to tell that user what inputs are expected.

	cout << "Enter the ratio of width to length: ";
	cin >> ratio;
	cout << "Enter the rectangle's perimeter: ";
	cin >> perimeter;
	
	// STEP 4:  Determine the width, length and area of the rectangle.
	//    Remember to use an asterisk to represent multiplication.

	width = perimeter/(2*(1+ratio));
	length = ratio * width;
	area = 2*width + 2*length;
	
    // STEP 5:  Output the results, with explanatory text.

	cout << "The rectangle has dimensions " << length << "x" << width << " and area " << "area" << ".";

	return 0;
}
//  STEP 6:  Build a solution, and then Debug/Start-without-debugging.
//         (Ratio 1, Perimeter 24)  (Ratio 4, Perimeter 30) and
//         (Ratio 2.25, Perimeter 26)  should all have an area of 36.
//
//  STEP 7:  Log in to ANGEL, and submit this .cpp file.
