// main.cpp - Vector template class test application
// Written by Brian Golden

#include <iostream>
#include "Vector.h"

using namespace std;


void CopyTest (Vector <float> byValue)
{
	// print byValue to cout
	// cout << byValue;
}


void main ()
{
	
	try 
	{
		Vector<float> v;
		v.Add(10);
		v.Add(3.4);
		v.Add(6.2);
		v.Add(9.3);
		v.Add(1.8);
		v.Add(6.7);
		v.Add(1.6);
		v.Add(3.5);
		v.Add(4);
		v.Add(8.3); //10
		v.Add(7.1);
		v.Add(8.4);
		v.Add(5.3);
		v.Add(2.6);
		v.Add(3.8);
		v.Add(6.7);
		v.Add(2.5);
		v.Add(8.4);
		v.Add(2.7);
		v.Add(9.5); // 20
		v.Add(3.9);
		cout << "Should print 10...3.9: " << endl;
		cout << v << endl;

		v.Add(5);
		v.Add(2.9);
		cout << "Should print 10...2.9: " << endl;
		cout << v << endl;

		v.Remove(0);
		cout << "Should remove 10, so should print 3.4...3.9: " << endl;
		cout << v << endl;

		// Copy constructor test
		CopyTest(v);


		Vector<float> aVector;
		aVector = v;

		cout << "Should print 3.4...3.9: " << endl;
		cout << aVector << endl;
	}

	catch (string error)
	{
		cout << error << endl;
	}
	
	system("pause");
}