// Drink Machine Simulation
// Roger Christman		December 2008
// Motivated by Programming Challenge #12 in 
// "Starting Out with C++" by Tony Gaddis
//
// This tutorial practices the use of structures to
// maintain and record information.  The simulated
// machine holds onto both the drinks it sells and
// the change it has available.
#include <iostream>
#include <iomanip>
using namespace std;

//  Information recorded about beverages within the machine.
struct DrinkType {
	char	name[12];			// name of the drink
	int		price;				// price (in cents)
	int		stock;				// how many are available
};

//  Information recorded about coins within the machine
struct CoinType {
	char	name[10];			// name of the coin
	int		value;				// value of coin (in cents)
	int		stock;				// how many are available
};

//  Function prototypes for this program
void	insertCoin( CoinType &, int & );	// insert a coin into the machine
void	coinReturn( CoinType[], int );		// return coins from the machine
bool	purchaseable( DrinkType, int );		// can we buy this drink?
void	purchase( DrinkType &, int & );		// if so, purchase the drink

//  This driver program provides a very simple user-interface,
//  and should not require any major changes.  The focus of the
//  tutorial is in the functions that follow it.
int main()
{
	struct DrinkType drinks[] =
		{ { "Pepsi Cola", 75, 5 }, { "Root Beer", 75, 5 },
		  { "Lemon-Lime", 80, 5 }, { "Grape Soda", 80, 5 } };
	struct CoinType coins[] =
		{ { "quarter", 25, 5 }, { "dime", 10, 5 }, { "nickel", 5, 10 } };
	
	char response;						// user-interface input
	int  balance = 0;					// money spent so far

	cout << left;						// left justify strings below
	cout << fixed << setprecision(2);	// all real numbers reported this way
	cout << "Welcome to the not-so-refreshing Drink Simulation" << endl;
	do {
		cout << endl;
		cout << "Please select one of these choices by typing the letter:" << endl;
		cout << setw(25) << "buy (P)epsi Cola" << "insert a (Q)uarter" << endl;
		cout << setw(25) << "buy (R)oot Beer" << "insert a (D)ime" << endl;
		cout << setw(25) << "buy (L)emon-Lime" << "insert a (N)ickel" << endl;
		cout << setw(25) << "buy (G)rape Soda" << "insert (O)ne Dollar" << endl;
		cout << setw(25) << "press the (C)oin Return" << "e(X)it the simulation" << endl;

		cin >> response;
		switch (response)
		{
		case 'p': case 'P':
			if (purchaseable( drinks[0], balance ))
				purchase( drinks[0], balance );
			break;
		case 'r': case 'R':
			if (purchaseable( drinks[1], balance ))
				purchase( drinks[1], balance );
			break;
		case 'l': case 'L':
			if (purchaseable( drinks[2], balance ))
				purchase( drinks[2], balance );
			break;
		case 'g': case 'G':
			if (purchaseable( drinks[3], balance ))
				purchase( drinks[3], balance );
			break;
		case 'q': case 'Q':
			insertCoin( coins[0], balance );
			break;
		case 'd': case 'D':
			insertCoin( coins[1], balance );
			break;
		case 'n': case 'N':
			insertCoin( coins[2], balance );
			break;
		case 'o': case 'O':
			balance += 100;
			cout << "Current Balance:  $" << (balance * 0.01) << endl;
			break;
		case 'c': case 'C':
			coinReturn( coins, balance );
			balance = 0;
		}
	} while ( response != 'x' && response != 'X' );
}

//  TUTORIAL:  Complete the functions below
//  Remember, to manipulate a structure, type the variable name followed by a period (.)
//  Visual Studio will remind you what the member variables are when you type that period

//  Purchaseable
//  Determines whether a drink can be purchased.
//  If it cannot, it will also say why not
//     (either not enough money is supplied, or the drink is out of stock)
//  Parameters:
//		drink		(input struct)		the selected beverage
//		money		(input int)			the amount of money supplied
// NOTE: This does not actually change any data -- it simply answers the
// boolean question, and produces an output if the answer is false.
bool	purchaseable( DrinkType drink, int money )
{	
	cout << "Price: $" << drink.price * .01 << endl;
	if (drink.stock > 0 && money >= drink.price)
		return true;
	else {
		cout << "You have not entered enough money to purchase that drink." << endl;
		return false;
	}
}

//  Purchase
//  Actually purchases a drink.  The cost of the drink is deducted
//		from the money supplied -- but the change is not yet returned.
//		(in case the person wants to make additional purchases)
//  Parameters:
//		drink		(modified struct)	the selected beverage
//		money		(modiifed int)		the amount of money supplied (reduced when spent)	
//  NOTE: You may assume that the Purchaseable function above has
//  already been called and yields a true result.
void	purchase( DrinkType &drink, int &money )
{
	drink.stock -= 1;
	money -= drink.price;
	cout << "Enjoy your " << drink.name << "!" << endl;	
}

//  InsertCoin
//  Supplies a coin to the machine, recording the balance so far
//      The number of coins in the machine will increase
//  Parameters:
//		coin		(modified struct)	the coin inserted
//		money		(modified int)		amount of money entered so far (increased)
void	insertCoin( CoinType &coin, int &money )
{
	coin.stock += 1;
	money += coin.value;
	cout << "Current Balance:  $" << (money * 0.01) << endl; 
}

//  CoinReturn
//  Returns change to the customer (and out of the machine)
//  NOTE: For best results, take advantage of the array of structures.
//  The code should then work with any monetary system.
//
//  Hint:  Recall the programming question from the first midterm
//      which also computed change to return.
//  NOTE:  The array is there to allow code repetition in a loop.
//	    For full credit, your implementation must take advantage of that
//
//  Parameters:
//		coins		(modified struct array)	all the types of coins available
//		change		(input integer	)		amount of change to return		
void	coinReturn( CoinType coins[], int change )
{
	cout << "Returning: $" << change*.01 << endl;
	for (int i = 0; i < 3; i++) {

		CoinType coin = coins[i];
		int numCoinsRemaining = change / coin.value;
		if (numCoinsRemaining > coin.stock)	// if we don't have that many
			numCoinsRemaining = coin.stock;
		change -= coin.value * numCoinsRemaining;
		cout << coin.name << "s: " << numCoinsRemaining << " " << endl;
	}
	
}