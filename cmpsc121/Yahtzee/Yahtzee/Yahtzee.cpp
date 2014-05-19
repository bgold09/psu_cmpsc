/* 
 * This program is a simulation of the game Yahtzee. 
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 11/6/10

#include <iostream>
#include <cmath>      //required for rand
using namespace std;

//const char nameLower[7][16] = { "Three of a Kind", "Four of a Kind", "Full House",
//        "Small Straight", "Large Straight", "Yahtzee", "Chance" };

//int (*scoreLower[7])( int[] ) = { threeOfaKind, fourOfaKind, fullHouse,
//       smallStraight, largeStraight, yahtzee, chance };

// declare functions
void printArray (int dice[]);
int rollDie ();
void showScores (int dice[]);
void rerollDice (int dice[], int diceToReroll);
int threeOfaKind (int dice[]);
int fourOfaKind (int dice[]);
int fullHouse (int dice[]);
int smallStraight (int dice[]);
int largeStraight (int dice[]);
int chance (int dice[]) ;
int yahtzee (int dice[]);

int main () {

	int die;
	char answer;

	do {
		int diceToReroll = 88;	//dummy value
		int dice[] = {0, 0, 0, 0, 0, 0};
		
		// roll dice
		cout << "Rolling 5 dice: ";
		for (int i = 0; i < 5; i++) {
			die = rollDie ();
			dice[die-1] += 1;
		}
		printArray(dice);

		// handles rerolling; allows three rerolls or stops when 
		// the user doesn't want to reroll any more (enters 0)
		for (int i = 0; i < 3 && diceToReroll != 0; i++) {
			cout << "Enter dice to reroll (e.g. 15 rerolls a 1 and a 5); 0 to quit: ";
			cin >> diceToReroll;
			if (diceToReroll != 0) {
				float fDiceToReroll = diceToReroll;
				int intNumRolling = log10(fDiceToReroll);
				cout << "Rerolling " << intNumRolling + 1 << " dice: ";
				rerollDice (dice, diceToReroll);
				printArray(dice);
			}
		}

		showScores(dice);
		cout << "Play another round [y/n]? ";
		cin >> answer;
		cout << endl;
	}
	while (answer == 'y');


	cout << "Thanks for playing!    ";
}

// shows all the appropriate scores for the round
void showScores (int dice[]) {

	cout << "                  " << "Three of a Kind " << threeOfaKind(dice) << endl;
	cout << "Sets of 1's: " << dice[0] << "    " << "Four of a Kind " << fourOfaKind(dice) << endl;
	cout << "Sets of 2's: " << dice[1]*2 << "    " << "Full House " << fullHouse(dice) << endl;
	cout << "Sets of 3's: " << dice[2]*3 << "    " << "Small Straight " << smallStraight(dice) << endl;
	cout << "Sets of 4's: " << dice[3]*4 << "    " << "Large Straight " << largeStraight(dice) << endl;
	cout << "Sets of 5's: " << dice[4]*5 << "    " << "Yahtzee " << yahtzee(dice) << endl;
	cout << "Sets of 6's: " << dice[5]*6 << "    " << "Chance " << chance(dice) << endl << endl;
}

// handles the rerolling of the input dice
void rerollDice (int dice[], int diceToReroll) {

	int die; // taken from diceToReroll
	int newDie;
	while (diceToReroll > 0) {
		die = diceToReroll % 10;
		diceToReroll = diceToReroll / 10;
		// set so that if there are no more of that die
		// the program will simply ignore that number
		if (dice[die-1] > 0) {
			dice[die-1] -= 1;
			newDie = rollDie ();
			dice[newDie-1] += 1;
		}
	}
}

// if there are at least three of the same die, awards the sum of those three dice
int threeOfaKind (int dice[]) {

	int points = 0;
	for (int i = 0; i < 6; i++)
		if (dice[i] >= 3)
			points = dice[i] * (i+1);
	return points;
}

// if there are at least four of the same die, awards the sum of those four dice
int fourOfaKind (int dice[]) {

	int points = 0;
	for (int i = 0; i < 6; i++)
		if (dice[i] >= 4)
			points = dice[i] * (i+1);
	return points;
}

// awards 30 points if there are three of one die, two of another
int fullHouse (int dice[]) {

	int points = 0;
	int index;
	bool foundPair = false;

	// find where there are three of one die
	for (int i = 0; i < 6; i++)
		if (dice[i] >= 3) {
			index = i;
			for (int i = 0; i < 6 && foundPair == false; i++)
				// ignore location containing the triple
				if (i != index)
					if (dice[i] == 2) {
						foundPair = true;
						points = 30;
					}
		}
	return points;
}

// awards 25 points if there are four consecutive numbers
int smallStraight (int dice[]) {

	if ((dice [0] >= 1 && dice [1] >= 1 && dice [2] >= 1 && dice [3] >= 1) || (dice [1] >= 1 && dice [2] >= 1 && dice [3] >= 1 && dice [4] >= 1) 
		|| (dice [2] >= 1 && dice [3] >= 1 && dice [4] >= 1 && dice [5] >= 1))
		return 25;
	return 0; 
}

// awards 30 points if there are five consecutive numbers
int largeStraight (int dice[]) {

	if ((dice[0] >= 1 && dice[1] >= 1 && dice[2] >= 1 && dice[3] >= 1 && dice[4] >= 1) || (dice[1] >= 1 && dice[2] >= 1 && dice[3] >= 1 && dice[4] >= 1 && dice[5] >= 1))
		return 25;
	return 0; 
}

// awards 50 points if there are five of one die
int yahtzee (int dice[]) {

	int points = 0;
	for (int i = 0; i < 6; i++)
		if (dice[i] == 5)
			points = 50;
	return points;
}

// computes the sum of all the dice
int chance (int dice[]) {

	int points = 0; 
	for (int i = 0; i < 6; i++) 
		points += dice[i] * (i+1);
	return points;
}

// rolls a single six-sided die
int rollDie () {
	
	return 1 + rand() % 6;
}

// prints the dice
void printArray (int dice[]) {

	for (int i = 0; i < 6; i++) 
		for (int index = dice[i]; index > 0; index--)
			cout << i+1 << " ";
	cout << endl;
}