/* Author: Brian Golden
 * Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
 * Final Version: 09/17/2010
 *
 * This program takes three ordered pairs on the cartesian plane and outputs the area using:
 * 1) the 18th century formula Area = ½  | (x1y2 - x2y1) + (x2y3 - x3y2) + (x3y1 - x1y3) |
 * 2) the root method (s(s-a)(s-b)(s-c))^1/2 
 * 3) an application of the law of cosines
 */

#include <iostream>
#include <cmath>   // needed for sqrt, abs, trig functions
#include <iomanip> // needed for precision and setf
using namespace std;

int main() {

	// declare variables
	float x1, x2, x3, y1, y2, y3, area1, area2, area3, a, b, c, s, angle;

	// Get ordered pairs from user
	cout << "Please enter values for the following: " << endl;
	cout << "x1: ";
	cin >> x1; 
	cout << "y1: ";
	cin >> y1;
	cout << "x2: ";
	cin >> x2;
	cout << "y2: ";
	cin >> y2;
	cout << "x3: ";
	cin >> x3;
	cout << "y3: ";
	cin >> y3;

	// compute area by "18th century" method
	area1 = abs(0.5*((x1*y2 - x2*y1) + (x2*y3 - x3*y2) + (x3*y1 - x1*y3)));
	cout.precision(10);         
	cout.setf(ios::fixed,ios::floatfield);   // set precision for all outputs
	cout << "The area computed by the '18th century' method is " << setprecision(9) << area1 << "." << endl;

	// compute area by root method
	a = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); // compute each side by distance formula
	b = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
	c = sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));
	s = (a+b+c)/2; // semiperimeter = perimeter/2
	area2 = sqrt(s*(s-a)*(s-b)*(s-c));
	cout << "The area computed by the the squareroot  method is " << setprecision(9) << area2 << "." << endl;

	// compute area by law of cosines
	angle = acos((a*a +b*b - c*c)/(2*a*b)); // angle bewteen sides a & b
	area3 = 0.5*a*b*sin(angle);
	cout << "The area computed by the law of cosines method is " << setprecision(9) << area3 << "." << endl;

	return 0;
}