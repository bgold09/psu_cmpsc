// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - BoardLocation class
// Description: Implementation for a BoardLocation class; BoardLocation objects have a name and address; 
//              BoardLocations objects may model a Property object, TaxSquare, 
//              CardSquare or CornerSquare object

public abstract class BoardLocation 
{
    protected String name;      // name of the board location (e.g. property name, "chance," etc.)
    protected int address;      // Address of the board location, i.e. number of spaces from "GO"
    
    public BoardLocation()
    // POST: A default BoardLocation object is created with name set to a blank string
    //       and address set to 0
    {
        this("", 0);
    }
    
    public BoardLocation(String name, int address)
    // PRE:  name is initialized; 0 <= address < 40       
    // POST: A BoardLocation object is created with name and address set to 
    //       their corresponding input parameters
    {
        this.name = name;
        this.address = address;
    }
    
    public abstract String[] GetPossibleActions(Player player, int diceSum);
    // PRE:  player is initialized, 2 <= diceSum <= 12
    // POST: FCTVAL == array of options player has upon landing on this space, to be used in a menu in 
    //       a user interface; diceSum is sum of two rolled 6-sided dice, used to compute rent when 
    //       landing on a Utility
    
    public String GetName()
    // POST: FCTVAL == name of the board location
    {
        return name;
    }
    
    public int GetAddress()
    // FCTVAL == address of the board location
    {
        return address;
    }
    
    @Override
    public String toString()
    // POST: Returns a string representation of the BoardLocation object
    {
        return name + " (" + address + ") ";
    }
}