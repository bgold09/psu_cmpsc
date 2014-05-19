/* 
 * This program is a simulation of the card game Go Fish. 
 */

// Author: Brian Golden
// Environment: Visual Studio Professional 2010, Windows 7 Home Premium (x86)
// Final Version: 11/15/10

#include <iostream>
#include <cmath>      //required for rand
using namespace std;

const int CARDSIZE = 32;
bool completedSet = true;
bool continueGame = true;

// declare functions
void shuffleDeck (int deck[]);
void drawCard (int handToGiveTo[], int deck[], int index);
void takeCard (int handToGiveTo[], int handToTakeFrom[], int cardLocation);
void printHand (int deck[]);
void guessCard (int guesserHand[], int oppHand[], int card, int &numberFound); 
void checkForSets (int hand[]);
void shiftCards (int hand[]);
void runPlayerTurn (int playerHand[], int compHand[], int deck[], bool& continueTurn);
void runCompTurn (int compHand[], int playerHand[], int deck[], bool& continueTurn);
void runCheck (int deck[], int playerHand[]);

int main () {

	int deck[CARDSIZE],	playerHand[CARDSIZE], compHand[CARDSIZE];
	char answer;
	bool continueTurn = true;

	cout << "Let's play Go Fish!" << endl;
	do {
		//populate the deck with cards and shuffle
		for (int i = 0; i < CARDSIZE; i = i+8)
			for (int count = 0; count <= 7; count++)
				deck[count+i] = count+2;
		shuffleDeck(deck);
	
		// deal player 7 cards
		for (int i = 0; i < 7; i++)
			drawCard(playerHand, deck, i);
		//makes other values of the array equal zero to aid other functions
		for (int i = 7; i < CARDSIZE; i++)
			playerHand[i] = 0;

		// deal computer 7 cards
		for (int i = 0; i < 7; i++)
			drawCard(compHand, deck, i);
		for (int i = 7; i < CARDSIZE; i++)
			compHand[i] = 0;
	
		// The game ends when either player runs out of cards, or if the ocean runs out of cards 
		do {
			continueTurn = true;
			while (continueTurn == true && continueGame == true)
				runPlayerTurn(playerHand, compHand, deck, continueTurn);
			continueTurn = true;
			while (continueTurn == true && continueGame == true)
				runCompTurn(compHand, playerHand, deck, continueTurn);
		} 
		while (continueGame == true);

		runCheck (deck, playerHand); // prints reason for game ending
		cout << "Would you like to play again (y or n)?    ";
		cin >> answer;
		cout << endl;
	} 
	while (answer == 'y');

	cout << "Thanks for playing!" << endl;
}

// runs a turn for the player
void runPlayerTurn (int playerHand[], int compHand[], int deck[], bool& continueTurn) {

	int timesFound = 0;
	int cardToGuess;
	bool drewGuessedCard = 0;

	do {
		drewGuessedCard = false;
		cout << "You have: ";
		printHand (playerHand);
		cout << "You ask if I have any... ";
		cin >> cardToGuess;
		while (cardToGuess > 9 || cardToGuess < 2) {
			cout << "The number must be between 2 and 9. Please guess again:  ";
			cin >> cardToGuess;
		}
		
		guessCard (playerHand, compHand, cardToGuess, timesFound);
		shiftCards (compHand);

		if (timesFound > 0)
			cout << "Yes, I have " << timesFound << "." << endl;
		else
			cout << "No, I don't have any." << endl;

		// SPECIAL CASE, may happen when cards are taken from the computer
		if (compHand[0] == 0) {
			continueGame = false;
			break; // break out because the computer no longer has any cards (game is over)
		}

		int count = 0;
		if (timesFound == 0) {
			cout << "Go fish!" << endl;
			for (count; (count < CARDSIZE) && (playerHand[count] != 0); count++);
			drawCard (playerHand, deck, count);
			cout << "You drew a " << playerHand[count] << endl;
			if (playerHand[count] == cardToGuess) 
				drewGuessedCard = true;
		}
		checkForSets(playerHand);
		if (completedSet == true) {
			cout << "You completed a set!" << endl;
			
			// SPECIAL CASE, may happen when sets are removed from the player's hand
			if (playerHand[0] == 0) {
				continueGame = false;
				break; // break out because the player no longer has any cards (game is over)
			}
		}

	} 
	while ((timesFound > 0) || (drewGuessedCard == true));

	continueTurn = false;

	if (continueGame == true)
		cout << "My turn!" << endl << endl;
}

void runCompTurn (int compHand[], int playerHand[], int deck[], bool& continueTurn) {
	
	int timesFound = 0;
	int cardToGuess;
	bool drewGuessedCard;

	do {

		drewGuessedCard = false;
		cardToGuess = rand() % 8 + 2;  // random card value bewteen 2 and 9
		cout << "I ask: Do you have any " << cardToGuess << "'s?" << endl;
		
		guessCard (compHand, playerHand, cardToGuess, timesFound);
		shiftCards (playerHand);

		if (timesFound > 0)
			cout << "You have " << timesFound << "." << endl;
		else 
			cout << "You don't have any, so I'll draw a card." << endl;

		// SPECIAL CASE, may happen when cards are taken from the player
		if (playerHand[0] == 0) {
			continueGame = false;
			break; // break out because the player no longer has any cards (game is over)
		}

		int count = 0;
		if (timesFound == 0) {
			for (count; (count < CARDSIZE) && (compHand[count] != 0); count++);
			drawCard (compHand, deck, count);
			if (playerHand[count] == cardToGuess) 
				drewGuessedCard = true;
		}

		checkForSets(compHand);
		if (completedSet == true)
			cout << "I completed a set!" << endl;

		// SPECIAL CASE, may happen when sets are removed from the player's hand
		if (playerHand[0] == 0) {
			continueGame = false;
			break; // break out because the player no longer has any cards (game is over)
		}
	} 
	while ((timesFound > 0) || (drewGuessedCard == true));

	continueTurn = false;

	if (continueGame == true)
		cout << "It's your turn again." << endl << endl;
}

// prints reason for game ending
void runCheck (int deck[], int playerHand[]) {

	if (deck[CARDSIZE-1] == 0)
		cout << "There are no more cards in the deck, so the game is over" << endl;
	else if (playerHand[0] == 0)
		cout << "You have no more cards in your hand, so the game is over" << endl;
	else 
		cout << "I have no more cards in my hand, so the game is over" << endl;
}

// one player guesses a card, gives all of the opposing player's 
// cards of that value to the guessing player
void guessCard (int guesserHand[], int oppHand[], int card, int &numberFound) {
	
	// search array for guessed card
	numberFound = 0;	// holds value for number of times the guessed card is found in the hand
	for (int i = 0; i < CARDSIZE; i++)
		if (oppHand[i] == card) {
			takeCard (guesserHand, oppHand, i);
			numberFound += 1;
		}
}

// checks the hand for sets of 4 of each card 
// removes them if four of that card are found
void checkForSets (int hand[]) {

	completedSet = false;	// reset value of the boolean, no sets are found initially
	// check for sets of each card 2-9
	for (int card = 2; card <= 9; card++) {
		
		// holds locations of where the card was found
		// initially holds dummy values to show that the card
		// has not yet been found anywhere
		int locationsFound[] = {88, 88, 88, 88};

		for (int i = 0; i < CARDSIZE; i++) {
			
			if (hand[i] == card) {
				// enters the locations of the card into the array locationsFound
				int index;
				for (index = 0; (index < 4) && (locationsFound[index] != 88); index++); 
				// enter location as long as the entry in the locationsFound 
				// array is not already holding some other location
				locationsFound[index] = i;
			}
			// if the card was found 4 times, a set has been completed
			// so remove those four cards from the hand
			if (locationsFound[3] != 88) {
				for (int c = 0; c < 4; c++) {
					int location = locationsFound[c];
					hand[location] = 0;
				}
				completedSet = true;
			}
		}
	}
}

//shifts the cards to eliminate 'holes' in the array (the hand)
void shiftCards (int hand[]) {

	for (int i = 0; i < CARDSIZE; i++) {
		bool swapped = false;
		// if the location holds zero, check the rest
		// of the hand for a real card to switch with
		if (hand[i] == 0) {
			for (int index = i+1; (index < CARDSIZE) && (swapped == false); index++)
				if (hand[index] != 0) {
					swap (hand[i], hand[index]);
					swapped = true;
				}
		}
	}
}

// takes a card from the deck and puts it into a hand at the index
void drawCard (int handToGiveTo[], int deck[], int index) {
	
	// put the new card into the first location of the array that
	// isn't already holding a card
	int i = 0;
	for (i; (i < CARDSIZE) && (deck[i] == 0); i++);
		handToGiveTo[index] = deck[i];
		deck[i] = 0;
}

// takes a card from one hand and puts it into the other hand
void takeCard (int handToGiveTo[], int handToTakeFrom[], int cardLocation) {

	int temp = handToTakeFrom[cardLocation];
	int i = 0;
	for (i; (i < CARDSIZE) && (handToGiveTo[i] != 0); i++);
		handToGiveTo[i] = temp;
		handToTakeFrom[cardLocation] = 0;
}

//shuffles the deck
void shuffleDeck (int deck[]) {
	
	for (int i = 0; i < CARDSIZE; i++) {
		int x = rand() % (CARDSIZE);	// number between zero and end of the array
		swap (deck[i], deck[x]);	// swaps the current card with a random one within the deck
	}
}

// prints the cards in the hand
void printHand (int hand[]) {

	for (int i = 0; i < CARDSIZE; i++)
		if (hand[i] != 0)
			cout << hand[i] << " ";
	cout << endl;
}