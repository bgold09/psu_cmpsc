// main.cpp - test c++ program
// Written by Brian Golden

#include <iostream>
#include "Complex.h"

using namespace std;

template <typename TYPE>
void test (Complex<TYPE> byValue)
{
}

void main ()
{
	// a is a variable of UDT called Complex
	// a is an instance of class Complex
	// a is an object of type Complex
	// object = instance of a class
	Complex<float> a(1, 2), b(-1, -2);

	test(a);

	Complex<float> c;
	cout << c.Re << endl;
	c = a + b;
	cout << "a + b should produce     0, 0: " << a+b << endl;
	cout << "a - b should produce     2, 4: " << a-b << endl;
	cout << "a * b should produce   -1, -4: " << a*b << endl;
	cout << "a / b should produce   -1, -1: " << a/b << endl;


	// 7.5f means float constant
	// 7.5 means double constant
	float x = 7.5f;
	c = c + x;


	cout << "a + x should produce    8.5, 2: " << a+x << endl;
	cout << "a - x should produce   -6.5, 2: " << a-x << endl;
	cout << "a * x should produce    7.5, 2: " << a*x << endl;
	cout << "a / x should produce  0.133, 2: " << a/x << endl;
	
	cout << "a += b should produce     0, 0: " << (a += b) << endl;
	a = Complex<float> (1, 2);
	cout << "a -= b should produce     2, 4: " << (a -= b) << endl;
	a = Complex<float> (1, 2);
	cout << "a *= b should produce   -1, -4: " << (a *= b) << endl;
	a = Complex<float> (1, 2);
	cout << "a /= b should produce   -1, -1: " << (a /= b) << endl;

	// when you call c.Absolute() c is the current instance of the class
	cout << "Absolute value of c = " << c.Absolute() << endl;

	system("pause");
}