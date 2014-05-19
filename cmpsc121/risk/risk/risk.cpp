/* 
 * This program is a simulation of the board game Risk. 
 * The rules if the game are as follows:
 *		o Each battle begins with an attacking army and a defending army. The attacker may roll up to three dice, 
 *		  or one per army unit (whichever is smaller). The defender may roll up to two dice (whichever is smaller).
 *
 *		o To determine the outcome of the battle, compare the dice, starting with the largest values on each side. 
 *		  Whoever rolls the higher number wins, and the loser loses one army unit. Only rolled dice are compared.
 *		  Also, the defender wins all ties.
 *
 *		o The program will simulate any of the following:
 *			1) Roll and resolve just one skirmish (directly simulating how the game is played)
 *			2) Run the battle all the way to its end, when one army is complete defeated
 *			3) Run the battle until the attacking army is reduced to some conservative amount. It may end early if the defender is completely defeated.
 *			4) Withdraw from the battlefield -- with no more skirmishes for the battle 
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 10/25/2010

#include <iostream>
using namespace std;

// declare fucntions
void skirmish (int& attack, int& defend);
int rollDie ();
void sortThree (int& first, int& second, int& third);

int main () { 

	//declare variables
	int attack, defend, choice;
	char answer;

	cout << "Let's play Risk, the game of strategy. " << endl << endl;

	do {
		//Inputs (from the keyboard):
		cout << "Enter the following: " << endl;
		cout << "Size of attacking army:  "; 
		cin >> attack;
		cout << "Size of defending army:  "; 
		cin >> defend;
		cout << endl;

		do {
			cout << "What would you like to do?" << endl;
			cout << "1 = Roll and resolve just one skirmish" << endl;
			cout << "2 = Run the battle all the way to its end, when one army is completely defeated" << endl;
			cout << "3 = Run the battle until the attacking army is reduced to some amount" << endl;
			cout << "4 = Withdraw from the battlefield" << endl;
			cin >> choice;
			cout << endl;
	
			switch (choice) {
				case 1: skirmish (attack, defend);
						break;
				case 2: while (attack > 0 && defend > 0) 
							skirmish (attack, defend);
						break;
				case 3: int reduce; // number to reduce attacking army to
						cout << "What would you like to reduce the attacking army to? ";
						cin >> reduce;
						while (attack > reduce && defend > 0)
							skirmish (attack, defend);
						break;
				case 4: break;
			}

			//when user decides to withdraw, it is not necessary to repeat army sizes as they will be the same
			if (choice == 4)
				break;

			cout << "The attacking army is " << attack << " strong." << endl;
			cout << "The defending army is " << defend << " strong." << endl << endl;

			// end battle is attack or defend equals 0
			if (attack == 0) {
				cout << "The attacking army was defeated, so this battle is over. " << endl;
				choice = 4;
			} else if (defend == 0) {
				cout << "The defending army was defeated, so this battle is over. " << endl;
				choice = 4;
			}

		} 
		while (choice != 4);

		cout << "Would you like to simulate another battle (y or n)?  ";
		cin >> answer;
		cout << endl;

	}
	while (answer == 'y');

}

//rolls any needed dice, sorts them, and resolves the outcome
void skirmish (int& attack, int& defend) {

	int attack_die_1, attack_die_2, attack_die_3, defend_die_1, defend_die_2, defend_die_3;
	attack_die_1 = attack_die_2 = attack_die_3 = defend_die_1 = defend_die_2 = defend_die_3 = 0;
	
	//roll attacker dice
	if (attack >= 3) {
		attack_die_1 = rollDie();
		attack_die_2 = rollDie();
		attack_die_3 = rollDie();
	}
	else if ( attack == 2) {
		attack_die_1 = rollDie();
		attack_die_2 = rollDie();
	}
	else 
		attack_die_1 = rollDie();
	
	//roll defender dice
	if (defend >= 2) {
		defend_die_1 = rollDie();
		defend_die_2 = rollDie();
	}
	else
		defend_die_1 = rollDie();
	
	//sort dice
	sortThree (attack_die_1, attack_die_2, attack_die_3);
	sortThree (defend_die_1, defend_die_2, defend_die_3);
	
	//compare and resolve

	//compare first dice
	if (attack_die_1 < defend_die_1 || attack_die_1 == defend_die_1)
		attack -= 1;
	else 
		defend -= 1;

	//compare second dice
	//compare only if attacking and defending armies still exist, i.e. they are not defeated, != zero
	if (attack != 0 && defend != 0) {
		if (attack_die_2 < defend_die_2 || attack_die_2 == defend_die_2)
			attack -= 1;
		else 
			defend -= 1;
	}
}

//rolls a single six-sided die
int rollDie () {
	
	return 1 + rand() % 6;
}

//sorts its parameters so that they result in decreasing order
void sortThree (int& first, int& second, int& third) {

	if (third > second) 
		swap (second, third);
	if (second > first)
		swap (first, second);
}

//swaps the values of two variables, in this case dice
void swap (int& numA, int& numB) {
	
	int temp; // used to keep number for swapping
	temp = numA;
	numA = numB;
	numB = temp;
}