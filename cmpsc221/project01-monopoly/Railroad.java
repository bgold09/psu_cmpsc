// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Railroad class
// Description: Implementation for a Railroad class; 
//              Railroad objects have a name, address and purchase cost

public class Railroad extends Property 
{
    private static final int RAILROAD_RENT = 25;   // Rent multiplier for railroads, in Monopoly dollars
    
    public Railroad()
    // POST: A default Railroad object is created with name set to blank string, 
    //       address and purchaseCost set to 0
    {
        this("", 0 , 0);
    }
    
    public Railroad(String name, int address, int purchaseCost)
    // PRE: name is initialized; 0 <= address < 40; purchaseCost > 0
    // POST: A Railroad object is created with name, address and purchaseCost set to 
    //       their corresponding input parameters        
    {
        super(name, address, purchaseCost);     // Call Property init constructor
    }
    
    @Override
    public int GetRentDue(int diceSum)
    // POST: FCTVAL == rent player owes, in Monopoly dollars
    {
        return GetRentDue();
    }
    
    public int GetRentDue()
    // POST: FCTVAL == rent player owes, in Monopoly dollars
    {
        if (owner == null)                      // if unowned, no rent due
            return 0; 
        else
            switch(owner.GetNumRailroads())     // if owned, follow structure below
            {         
                case 1:                         // if owner owns only 1 Railroad
                    return 25;
                case 2:                         // if owner owns 2 Railroads
                    return 50;
                case 3:                         // if owner owns 3 Railroads
                    return 100;
                case 4:                         // if owner owns 4 Railroads
                    return 200;
                default:                        // default case to satisfy compiler, numRailroads should
                    return 0;                   // never be less than 0 or greater than 4
            }
    }
    
    @Override
    public String toString()
    // POST: Returns string representation of this Railroad object
    {
        return super.toString() + "]";
    }
}