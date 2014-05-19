// Practice Loop
// Roger Christman		Fall 2010
//
// A bit of practice in implementing loops:
// Pick any applicable loop from the following:
//
//                     do
// while (condition)       statement        for( init; condition; update)
//     statement       while (condition);       statement
//
// Feel free to test each loop after it is written,
// instead of waiting until all the code for the tutorial is completed.
//
// SPECIAL NOTE FOR THIS TUTORIAL:
// All of the decisions made by the practice examples today
// should be handled in the manners shown above.
// Any answer that uses an If or Switch statement for any reason
// will not receive full credit.

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
	int n = 1;		// this variable can be reused in many loops
					// initialized only so program compiles as given

	// This will show 3 decimal places for all real numbers
	cout << setprecision(3) << fixed;
	
	// I:  display a table of square roots for 5 <= n <= 10
	//     (there should be six lines of output)
	cout << "Square Roots:" << endl;
	for (n = 5; n <= 10; n++)
		cout << n << "  " << sqrt( static_cast<float>(n) ) << endl;


	// II:   display a table of (n vs. ln n), for all integers n > 6 
	//       where the logarithm does not exceed 2.5
	//cout << "Logarithms:" << endl;
	//for (n = 6; log(n) <= 2.5; n++)
	//	cout << n << "  " << setw(10) << log( static_cast<float>(n) ) << endl;


	// III: compute the sum of the odd integers less than 30
	//      Remember: full credit is WITHOUT using an if statement
	
	int sum = -1;				// initialized only to allow program to compile
	for (n = 1; n <= 29; n+2)
		sum += n;

	cout << "The computed sum is " << sum << " and should be 225." << endl;

	// IV:  display factorials 1! to 9!, where n! = n * (n-1)!
	//	    Hint: this means it is n times the previous output
	int factorial = 1;

	for (n = 1; n <= 9; n++) {
		factorial = factorial * n;
		cout << n << "! = " << factorial << endl;
	}

	// V:   display factorials 8! down to 1!
	//      Full Credit if you do NOT use a nested loop for this!
	//		Hint: begin with the results from the last loop and work in reverse

	for (n = 8; n >= 1; n--) {
		factorial = factorial / n;
		cout << n << "! = " << factorial << endl;
	}

    // Make sure all of the above works before proceeding,
	// since this last one is a bit more of a challenge.

	// VI:  Use loops to produce a display like the        ****
	//      one shown on the right here, using an             ##
	//      input value that says how big it                 ####
	//      This image has a size of 4 -- with 4 stars      ######
	//      in the top row, and 4 rows of # signs          ########
	// HINT 1:  Output the symbols one at a time (with a loop)
	// HINT 2:  You can count how many spaces to output also
	// This MUST use loops:  NO if or switch statements.
	// It should work for any small integer value (not just 4)
	// Uncomment the two statements below when ready to test it.

	int rows;

//	cout << "How big will this image be?   ";
//	cin >> rows;





	return 0;
}