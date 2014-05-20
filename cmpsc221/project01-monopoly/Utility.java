// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Utility class
// Description: Implementation for a Utility class; 
//              Utility objects have a name, address and purchase cost

public class Utility extends Property 
{    
    public Utility()
    // POST: A default Utility object is created with name set to blank string, 
    //       address and purchaseCost set to 0
    {
        this("", 0 , 0);
    }
    
    public Utility(String name, int address, int purchaseCost)
    // PRE: name.equals("Electric Company") || name.equals("Water Works"); 
    //      0 <= address < 40; purchaseCost > 0  
    // POST: A Utility object is created with name, address and purchaseCost set to 
    //       their corresponding input parameters
    {
        super(name, address, purchaseCost);     // Call Property init constructor
    }
    
    @Override
    public int GetRentDue(int diceSum)
    // PRE: 2 <= diceSum <= 12, owner == null
    // POST: FCTVAL == amount of rent due, in Monopoly dollars
    {
        if (owner == null)    // if no owner, no rent due
            return 0;
        
        switch(owner.GetNumUtilities())
        {
            case 1:                 // if owner owns one utility, rent is 4 times diceSum
                return 4*diceSum;
            case 2:                 // if owner owns both utilities, rent is 10 times diceSum
                return 10*diceSum;
            default:
                return 0;
        }
    }
    
    @Override
    public String toString()
    // POST: Returns string representation of this Utility object
    {
        return super.toString() + "]";
    }
}