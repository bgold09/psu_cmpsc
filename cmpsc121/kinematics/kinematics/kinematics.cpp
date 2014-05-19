/* This program takes the speed of an object (in mph) and outputs the following:
 * 1) The distance traveled (in ft) in 0.5 seconds
 * 2) The distance traveled (in ft) if slamming on the brakes yields a constant deceleration of 1 g 
 * 3) Solves the following problem: Suppose you are driving on the highway
 *    following another car traveling at the same speed. If the driver in front slams on his brakes and decelerates at 1 g 
 *	  and your reaction time is 0.5 seconds, determine the deceleration needed to achieve to avoid a collision. 
 *		Solve for:
 *			a) trailing by 2 seconds
 *			a) trailing by 11 feet
 * 4) If your deceleration is now 0.8g, what is the minumum safe following distance between you and the other car?
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 09/05/2010

#include <iostream>
#include <cmath> //needed for pow function
using namespace std;

int main () {
	
	// declare variables
	double speed, distance, distance1, distance2, time, decel_3a, g_3a, decel_3b, g_3b, distance4;

	// Get user speed
	cout << "Please enter a positive non-zero speed (in mph): ";
	cin >> speed;

	// verify that input speed is positive and nonzero
	while (speed <= 0)
		{
		cout << "That number is not valid. Please enter a positive non-zero speed: ";
		cin >> speed;
		}

	// compute distance traveled in 0.5 seconds
	speed = speed*22/15; // puts speed in ft/s -- conversion factor, 5280ft/1mi * 1hr/3600s = 22/15
	distance1 = 0.5*speed; // true because x = vt
	
	// compute distance traveled if slamming on the brakes yields a constant deceleration of 1 g 
	distance2 = pow(speed, 2)/(2*32); // true because v^2 = v0^2 + 2ax, rearrange to |x| = v0^2/(2*g)

	// compute deceleration for problem #3a
	// for the purposes of this solution, let x = 0 be the point where your car begins, all other points are +x
	distance = distance2 + 2*speed; // the distance from the origin the other car stops
	decel_3a = pow(speed, 2)/(2*(distance - distance1));
	g_3a = decel_3a/32; // decel in terms of g

	// compute deceleration for problem #3b
	distance = distance2 + 11;
	decel_3b = pow(speed, 2)/(2*(distance - distance1));
	g_3b = decel_3b/32;

	//compute safe follwing distance behind car give the same circumstaces, but your own deceleration is 0.8g
	distance4 = pow(speed, 2)/(2*0.8*32); //distance it takes for you to stop after braking
	distance4 = distance4 + distance1; // add braking distance to additional distance travelled in 0.5 s
	distance4 = -(distance2 - distance4); // the difference between the distance it takes for the other car to stop and you to stop

	//print results
	cout << "When driving at " << speed*15/22 << " mph one covers " << distance1 << " feet in 0.5 sec." << endl;
	cout << "At 1 g deceleration, one would need " << distance2 << " feet to come to a stop." << endl;
	cout << "At 2 seconds tailing, with the 0.5-second reaction time, one would need to decelerate at about " << decel_3a << " ft/sec^2 (" << g_3a << " g)" << endl;
	cout << "At 11' tailing, with the 0.5-second reaction time, one would need to decelerate at about " << decel_3b << " ft/sec^2 (" << g_3b << " g)" << endl;
	cout << "If your deceleration is now 0.8g, then the minimum safe follwing distance is " << distance4 << " feet.";

	return 0;
}