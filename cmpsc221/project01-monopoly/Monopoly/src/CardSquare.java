// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - CardSquare class
// Description: Implementation for a CardSquare class; 
//              CardSquare objects have a name and address

public class CardSquare extends BoardLocation
{
    public CardSquare()
    // POST: A default CardSquare object is created with 
    //       name set to a blank string and address set to 0
    {
        this("", 0);
    }
    
    public CardSquare(String name, int address)
    // PRE: name.equals("Chance") || name.equals("Community Chest"), 0 <= address < 40
    // POST: A CardSquare object is created with name and 
    //       address set to their corresponding input parameters
    {
       super(name, address);
    }
    
    @Override
    public String[] GetPossibleActions(Player player, int diceSum)
    // PRE: player is initialized
    // POST: returns possible actions the player has 
    {
        String[] temp = new String[1];
        int randAmount;                     // amount of monopoly dollars to win or lose, range 0 to 200
        int randWinLose;                    // random number between 0 and 1; 0 for gain money
                                            // 1 for lose money
        
        randAmount = (int)(200 * Math.random());
        randWinLose = (int)(Math.random());
        
        if (randAmount != 0)
        {
            if (randWinLose == 0)
                temp[0] = "You gain $" + randAmount;     
            else
            {
                if (player.GetMoney() > randAmount)
                    temp[0] = "You lose $" + randAmount;
                else 
                    temp[0] = "You lost all your money!";
            }
        }
        else
            temp[0] = "You did not gain or lose any money";
        
        return temp;
    }
}