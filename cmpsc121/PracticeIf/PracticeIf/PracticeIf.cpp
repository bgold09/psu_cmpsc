// PracticeIf -- in-class practice with If statements and arithmetic
//
// Name: Brian Golden
//
// The students' goal is to complete the following
// program to properly identify features of an input value.
// Insert the code that displays the given outputs correctly.
//
// You may compile and test your program at any time --
// you need not do all the parts before testing.
//
// You must turn something in before the end of the class period.
// It is better to have several things working, then
// have everything started and nothing working.
//
// Your program must run correctly to completion for ANY 
// integer input, including negative values and zero.

#include <iostream>				// access to cin, cout
#include <cmath>				// access to math library
using namespace std;			// easier use of cin, cout

int main() {
	int value;					// value to work with

	// obtain the value to examine
	cout << "Please enter an integer to test:   ";
	cin >> value;

	// Use if statements here (display or do not display)
	if (value < 8)
		cout << "The value is less than 8" << endl;
	if (value > 2.5 && value < 5.5) 
	    cout << "The value is between 2.5 and 5.5" << endl;
	if (value != 0)
		if (60 % value == 0)
			cout << "60 is a multiple of this number" << endl;

    // Use if-else to display one of these two outputs
	if (value % 2 == 0)
		cout << "The value is even" << endl;
	else
		cout << "The value is odd" << endl;

    // Use if-else to display exactly one of these three outputs
	if (value < 0)
		cout << "The value is negative" << endl;
	else if (value > 0)
		cout << "The value is positive" << endl;
	else
		cout << "The value is equal to zero" << endl;

	// Make sure all of the above are correct before proceeding.
	// This last practice exercise is more challenging.

	// Solution Hint:  the base10 logarithm of a number
	// (available in a function named "log10") tells what
	// exponent to raise 10 to to get that number.
	// e.g. 10^2 = 100, so log10(100) = 2; log10(1000) = 3, etc.
	// Numbers between 100 and 1000 would have logarithms
	// between 2 and 3 (so if you drop the fraction, you get 2).
	//
	// BUT: log10 expects a real number of some kind
	// (float or double) as an argument, so to successfully
	// use it in Visual Studio, one must convert the value
	// above to a real number first, and then convert the
	// computed logarithm back to an integer for the desired result.
	// You may use either     static_cast<float>()   or   (float)   
	// to convert an integer to float; and similarly in the reverse.
	//
	// NOTE: A correct solution that handles all valid integers
	// will still need an IF statement!

	int digits = 0;			// only initialized to satisfy compiler
	float floatValue = static_cast<float> (value);
	floatValue =  abs (floatValue);
	digits = log10(floatValue);
	if (floatValue != 0)	
		cout << "The value has " << digits+1 << " digits" << endl;
	else
		cout << "The value has 1 digit" << endl;

}