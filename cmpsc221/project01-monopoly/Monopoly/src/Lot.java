// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Lot class
// Description: Implementation for a Lot class, a Property on the board that is part of a color group; 
//              Lot objects have a name, address, color, houseCost, number of houses and rent structure

public class Lot extends Property
{
    private String color;            // color of the lot group this lot belongs to
    private int houseCost;           // cost to improve the lot (add house / hotel), in Monopoly dollars
    private int numHouses;           // number of houses built on the lot, 5 houses == 1 hotel
    private int[] rentStructure;     // based on numHouses built on this lot determines current rent,
                                     // rents in Monopoly dollars
    
    public Lot()
    // POST: A default Lot object is created with name and color set to blank strings; 
    //       address, purchaseCost, numHouses and houseCost set to 0 
    //       rentStructure set to empty array
    {
        this("", 0, 0, "", 0, new int[6]);
    }
    
    public Lot(String name, int address, int purchaseCost, String color, int houseCost, int[] rentStructure)
    // PRE: name and color initialized; 0 <= address < 40; purchaseCost and houseCost > 0,
    //      each data point in rentStructure is initialized, each subsequent point 
    //      is greater than the one before it
    // POST: a Lot object is created with name, address, purchaseCost, color, numHouses, houseCost 
    //       and rentStructure set to their corresponding input parameters
    {
        super(name, address, purchaseCost);     // Call Property init constructor
        numHouses = 0;
        this.color = color;
        this.houseCost = houseCost;
        this.rentStructure = rentStructure;
    }
    
    public boolean BuildHouse()
    // PRE: owner must have a monopoly on the color of this Lot, numHouses < 5, 
    //      owner.GetMoney() > houseCost
    // POST: Returns true if a house/hotel may be built on the property
    {
        if (numHouses < 5 && owner.GetMoney() > houseCost)// if there are 4 or less houses and owner has
        {                                                 // enough money, owner may improve the Lot
            owner.SubtractMoney(houseCost);
            numHouses++;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    public boolean SellHouse()
    // POST: Returns true if a house/hotel on the property may be sold 
    {
        if (numHouses > 0)              // if there are houses/hotels on this Lot, they may be sold
        {
            numHouses--;
            owner.AddMoney((int)(houseCost*0.5));     // sell house/hotel back for 1/2 its purchase cost
            return true;
        }
        else
        {
            return false;
        }
    }
    
    @Override
    public int GetRentDue(int diceSum)
    // POST: Return $0 if Lot is unowned, otherwise returns appropriate 
    //       amount based on number of houses on this Lot        
    {
        return GetRentDue();
    }
    
    public int GetRentDue()
    // POST: FCTVAL == amount of rent due, in Monopoly dollars
    {
        if (owner == null)                      // if unowned, no rent due
            return 0;
        else                                    // if owned, rent due based on rent structure
            return rentStructure[numHouses];    // and number of houses currently built on it                       
    }
    
    public String GetRentStructure()
    // POST: Returns a comma-separated list of the rent structure of this Lot
    {
        String result = "{";        // resulting comma-seperated list of the rent structure
        int i;                      // loop iterator, declared to use outside the loop
        result += "$" + rentStructure[0] + ", ";
        result += "1 house: $" + rentStructure[1] + ", ";
        for (i = 2; i < 5; i++)     // add the name of each property to result
        {
            result += i + " houses: $" +  rentStructure[i] + ", ";
        }
        result += "hotel: $" + rentStructure[i] + "}";  // last rent does not need a comma after it
        
        return result;
    }
    
    public int GetNumHouses()
    // POST: FTCVAL == number of houses built on this Lot, 5 houses == 1 hotel
    {
        return numHouses;
    }
    
    public int GetHouseCost()
    // POST: FCTVAL == cost to improve the lot, in Monopoly dollars
    {
        return houseCost;
    }
    
    public String GetColor()
    // POST: FCTVAL == color of this Lot
    {
        return color;
    }
    
    @Override
    public String toString()
    // POST: FCTVAL == returns a string representation of a Lot object    
    {
        String result = new String();         // resulting output string
        result = "[" + color + "] " + super.toString();
        
        if (numHouses == 0)                     // if there are no houses on this Lot        
            result += ", unimproved]";    
        else if (numHouses < 5)                 // if this Lot has 1-4 houses
            result += ", " + numHouses + " house(s)]";
        else                                    // if this Lot has a hotel
            result += ", 1 hotel]";
        
        result += "\n   RENT: " + GetRentStructure() + "; Improve for $" + houseCost;
        return result;
    }
}