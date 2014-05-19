// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - CornerSquare class
// Description: Implementation for a CornerSquare class; 
//              CornerSquare objects have a name and address

public class CornerSquare extends BoardLocation
{    
    public CornerSquare()
    // POST: A default CornerSquare object is created with 
    //       name set to a blank string and address set to 0
    {
        this("", 0);
    }
    
    public CornerSquare(String name, int address)
    // PRE: name is "GO," "Free Parking," "Jail" or "Go to Jail"; 0 <= address < 40
    // POST: A CornerSquare object is created with name and 
    //       address set to their corresponding input parameters
    {
       super(name, address);
    }
    
    @Override
    public String[] GetPossibleActions(Player player, int diceSum)
    // PRE: player is initialized
    // POST: returns possible actions the player has        
    {
        String[] temp = {"Do nothing"};       // only possible action is to do nothing since all 
        return temp;                          // corner spaces are treated as free spaces
    }
}