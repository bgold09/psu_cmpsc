/* 
 * This program takes an input value and finds its prime factors, 
 * as well as identifying if the input number itself is prime.
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 10/11/2010

#include <iostream>
using namespace std;

int main () {

    int value, leftToFactor, factor, answer;
	bool continueLoop = true;
    
    do {
        cout << "Input a value to factor: ";
		cin >> value;
		cout << "The following numbers are the prime factors of " << value << ":" << endl;
		leftToFactor = value;
        factor = 2;
        while (factor <= leftToFactor) {
            while (leftToFactor % factor == 0) { 
				cout << factor << endl;
				leftToFactor = leftToFactor/factor;
			}
			factor++;
		}
		if (factor-1 == value)   // factor-1 because the factor was incremented by one in the inner loop
			cout << "The input number is prime." << endl;
		
		cout << "Would you like to try another value (1 for yes, 2 for no)?  ";
		cin >> answer;
		cout << endl;
		if (answer == 2) 
			continueLoop = false;
	}
	while (continueLoop);

}
