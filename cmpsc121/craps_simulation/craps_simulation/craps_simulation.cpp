/* 
 * This program is a simulation of the popular casino game, Craps. 
 * The rules if the game are as follows:
 *		-A 7 or 11 on the first roll wins.
 *		-A 2, 3, or 12 on the first roll loses.
 *		-Under all other circumstances, rolling will continue.
 *		-The total on the first roll is called the point. Rolling will continue until either:
 *         o The point is rolled (win)
 *         o A 7 is rolled (lose) 
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 10/11/2010

#include <iostream>
#include <cmath>    //required for rand
using namespace std;

const int STARTING_BANKROLL = 100;

int main () {

	int roll, die_1, die_2, numRolled, point, answer, bet;
	bool continuePlay = true;  //used to check whether or not to play more rounds

	int bankroll = STARTING_BANKROLL;  
	roll = 1;

	cout << "Let's play Craps!" << endl;

	for (roll; continuePlay; roll++) {
		
		// special terms for the first roll of the round
		if (roll == 1) {		
			
			// betting sequence
			cout << "You currently have $" << bankroll << " to bet." << endl;
			cout << "How much would you like to bet?  ";
			cin >> bet;
			cout << endl;
			while (bet > bankroll) {
				cout << "You do not have that much money remaining. Please choose another value to bet:  ";
				cin >> bet;
				cout << endl;
			}

			die_1 = 1 + rand() % 6;
			die_2 = 1 + rand() % 6;
			numRolled = die_1 + die_2;
			cout << "You rolled a " << numRolled << endl;
			
			// player wins if first roll is 7 or 11, loses if 2, 3, 12
			// otherwise, number is assigned to 'point value'
			switch (numRolled) {
				case 7:
				case 11:
					cout << "You won!" << endl;
					bankroll += bet;
					roll = 0;
					cout << "Would you like to play again (1 for yes or 2 for n)?  ";
					cin >> answer;
					if (answer == 1) {
						continuePlay = true;
						roll = 0;
						continue;
					} else if (answer == 2) {
						cout << "Thanks for playing!" << endl;
						continuePlay = false;
					}
				case 2: 
				case 3:
				case 12:
					cout << "Sorry, you lost." << endl;
					bankroll -= bet;
					if (bankroll <= 0) {
						cout << "I'm sorry, but you are all out of money to bet. Better luck next time!" << endl;
						return 0;
					}
					cout << "Would you like to play again (1 for yes or 2 for n)?  ";
					cin >> answer;
					cout << endl;
					if (answer == 1) {
						continuePlay = true;
						roll = 0;
						continue;
					} else if (answer == 2) {
						cout << "Thanks for playing!" << endl;
						continuePlay = false;
					}
				default:
					point = numRolled;
					cout << "The point is now " << point << endl;
					continue;
			}
		}

		// rolling sequence, two standard dice
		die_1 = 1 + rand() % 6;
		die_2 = 1 + rand() % 6;
		numRolled = die_1 + die_2;
		cout << "You rolled a " << numRolled << endl;
		
		// rules for rolls following the 'come-out' roll
		if (numRolled == point) {
			cout << "You rolled the point, so you win!" << endl << "Would you like to play again (1 for yes or 2 for n)?  ";
			cin >> answer;
			bankroll += bet;
			if (answer == 1) {
				continuePlay = true;
				roll = 0;
			} else if (answer == 2)
				continuePlay = false;
		} else if (numRolled == 7) {
			cout << "You rolled a 7, so you lose." << endl;
			bankroll -= bet;
			if (bankroll <= 0) {
				cout << "I'm sorry, but you are all out of money to bet. Better luck next time!" << endl;
				return 0;
			}
			cout << "Would you like to play again (1 for yes or 2 for n)?  ";
			cin >> answer;
			cout << endl;
			if (answer == 1) {
				continuePlay = true;
				roll = 0;
			} else if (answer == 2)
				continuePlay = false;
		}
	}

	return 0;
}