// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Player class
// Description: Implementation for a Player class; Player objects have a game token, position, money, 
//              number of properties, utilities and railroads owned and which properties are owned

public class Player 
{
    private static final int NUMBER_OF_SPACES = 40;         // Number of spaces on the Monopoly board
    private static final int NUMBER_OF_PROPERTIES = 28;     // No. of properties on the Monopoly board
    private static final int STARTING_MONEY = 1500;         // Monopoly dollars a player starts with
    
    private String token;                   // Game piece representing the player
    private Property[] propertiesOwned;     // Which properties are owned by the player
    private int numProperties;              // Total number of properties the player owns; 
                                            //  logical size of propertiesOwned
    private int numUtilities;               // Number of utilities owned by the player, range 0 - 2
    private int numRailroads;               // Number of railroads owned by the player, range 0 - 4
    private int position;                   // Current location of the player on the board, 
                                            //  number of spaces from "GO"
    private int money;                      // amount of money the player has, in Monopoly dollars
    
    public Player()
    // POST: A default Player object is created with token set to a blank string, numProperties, 
    //       numUtilities, numRailroads, and position to 0, money set to STARTING_MONEY and  
    //       propertiesOwned set to empty array of properties with size == NUMBER_OF_PROPERTIES
    {
        this("");        
    }
    
    public Player(String token)
    // PRE:  token is initialized
    // POST: A Player object is created with class member token input parameter token;
    //       money set to STARTING_MONEY; position, numProperties, numUtilities and numRailroads
    //       set to 0, propertiesOwned set to empty properties array with size == NUMBER_OF_PROPERTIES  
    {
        this.token = token;
        money = STARTING_MONEY;
        position = 0;
        numProperties = 0;
        numUtilities = 0;
        numRailroads = 0;
        propertiesOwned = new Property[NUMBER_OF_PROPERTIES];
    }
    
    public boolean PurchaseProperty(Property aProperty)
    // PRE: aProperty.GetOwner()== null, money >= aProperty.GetPurchaseCost()
    // POST: Returns true if player may purchase aProperty and this player now owns aProperty,
    //       otherwise return false        
    {
        // re-check precondition
        if (money > aProperty.GetPurchaseCost() && aProperty.GetOwner() == null)
        {
            aProperty.ChangeOwner(this);            // change owner to this player
            money -= aProperty.GetPurchaseCost();   // subtract cost of property from player's money
            propertiesOwned[numProperties] = aProperty;
            numProperties++;
            if (aProperty instanceof Utility)       
                numUtilities++;             // if aProperty is a Utility, increment numUtilities
            else if (aProperty instanceof Railroad)
                numRailroads++;             // if aProperty is a Railroad, increment numRailroads
            return true;
        }
        else
        {
            return false;
        }
    }
    
    public boolean BuyHouse(Lot aLot)
    // PRE: aLot.GetOwner() == this, money > aLot.GetHouseCost(), aLot has < 5 houses; this player owns
    //      all Lots of the same color as aLot (not enforced through code)
    // POST: a house is built on the lot
    {
        // re-check precondition
        if (aLot.GetOwner() == this && money > aLot.GetHouseCost())
        {
            if(aLot.BuildHouse())           // if a house may be built on the lot, build it 
                return true;               
            return false;                   // otherwise return false if Lot cannot be improved
        }
        else
        {
            return false;
        }
    }
    
    public void AddMoney(int moneyToAdd)
    // PRE:  moneyToAdd > 0
    // POST: player's money will be increased by moneyToAdd amount
    {
        money += moneyToAdd;
    }
    
    public boolean SubtractMoney(int moneyToSubtract)
    // PRE:  moneyToSubtract > 0
    // POST: Returns true if this player has enough money to pay moneyToSubtract and subtracts that 
    //       amount from money; false if this player does not have enough money to pay the amount due        
    {
        if (money > moneyToSubtract)    // if this player can pay amount due, then pay it
        {
            money -= moneyToSubtract;
            return true;
        }
        else
        {
            money = 0;                  // this player now has no money
            return false;
        }
    }
    
    public boolean MovePlayer(int diceSum)
    // PRE: 2 <= diceSum <= 12
    // POST: this Player has advanced numSpaces from previous location; Returns true if this Player
    //        passed "GO" and adds $200 in monopoly dollars to money, returns false otherwise
    {
        if (position + diceSum < NUMBER_OF_SPACES)     // if player will not pass "GO" during this turn
        {
             position += diceSum;
             return false;
        }
        else
        {
            position = diceSum - (NUMBER_OF_SPACES - position);
            money += 200;
            return true;
        }
    }
    
    public String GetProperties()
    // POST: Returns a comma-separated list of names of properties owned by this Player
    {
        String result = "";      // resulting comma-seperated list of properties the player owns 
        
        for (int i = 0; i < numProperties; i++)    // add the name of each property to temp
        {
            if (i+1 < numProperties)
                result += propertiesOwned[i].GetName() + ", ";
            else 
                result += propertiesOwned[i].GetName();  // last property does not need a comma after it
        }        
        return result;
    }
    
    public Lot[] GetImprovableLots()
    // POST: FCTVAL == Lots owned by this player that can be improved
    {
        Lot[] result;               // Resulting output of improvable Lots
        Lot aLot;                   // Temporary Lot object for checking number of houses
        int numImprovableLots = 0;  // Number of Lots this Player owns that may be improved
        int count = 0;              // Index of result
        
        for (int i = 0; i < numProperties; i++)     // check each Property to see if it can be improved
        {
            if (propertiesOwned[i] instanceof Lot)  // if propertiesOwned[i] is a Lot object
            {
                aLot = (Lot)propertiesOwned[i];  
                if (aLot.GetNumHouses() < 5)  // check if aLot has less than a hotel
                    numImprovableLots++;
            }
        }
        result = new Lot[numImprovableLots];        // Make result.length == numImprovableLots
               
        for (int i = 0; i < numProperties; i++)     // check each Property to see if it can be improved
        {
            if (propertiesOwned[i] instanceof Lot)  // if propertiesOwned[i] is a Lot object
            {
                aLot = (Lot)propertiesOwned[i];  
                if (aLot.GetNumHouses() < 5)    // check if aLot has less than a hotel 
                {
                    result[count] =  aLot;          // add Lot to the result
                    count++;
                }
            }
        }
        return result;
    }
    
    public Lot[] GetUnimprovableLots()
    // POST: FCTVAL == Lots owned by this player that can be unimproved
    {
        Lot[] result;                 // Resulting output of unimprovable Lots
        Lot aLot;                     // Temporary Lot object for checking number of houses
        int numUnimprovableLots = 0;  // Number of Lots this Player owns that may be unimproved
        int count = 0;                // Index of result
        
        for (int i = 0; i < numProperties; i++)    // check each Property to see if it can be unimproved
        {
            if (propertiesOwned[i] instanceof Lot) // if propertiesOwned[i] is a Lot object
            {
                aLot = (Lot)propertiesOwned[i];
                if (aLot.GetNumHouses() > 0)       // check if aLot at least a house
                    numUnimprovableLots++;
            }
        }
        result = new Lot[numUnimprovableLots];
               
        for (int i = 0; i < numProperties; i++)    // check each Property to see if it can be improved
        {
            if (propertiesOwned[i] instanceof Lot) // if propertiesOwned[i] is a Lot object
            {
                aLot = (Lot)propertiesOwned[i];
                if (aLot.GetNumHouses() > 0)       // check if aLot has at least a house
                {
                    result[count] =  aLot;         // add Lot to the result
                    count++;
                }
            }
        }
        return result;
    }
    
    public String[] GetImprovableLotNames()
    // POST: Returns names of all lots that this Player may improve
    {
        Lot[] improvableLots = GetImprovableLots();                 // Lots this player may improve
        String[] result = new String[improvableLots.length];        // Names of improvable Lots
        
        for (int i = 0; i < improvableLots.length; i++)             // get name of each Lot
            result[i] = improvableLots[i].GetName();
        
        return result;
    }
    
    public String[] GetUnimprovableLotNames()
    // POST: Returns names of all lots that this Player may unimprove
    {
        Lot[] unimprovableLots = GetUnimprovableLots();             // Lots this player may unimprove
        String[] result = new String[unimprovableLots.length];      // Names of unimprovable Lots
        
        for (int i = 0; i < unimprovableLots.length; i++)           // get name of each Lot
            result[i] = unimprovableLots[i].GetName();
        
        return result;
    }
    
    public String GetToken()
    // POST: FCTVAL == name of the player's token
    {
        return token;
    }
    
    public int GetNumUtilities()
    // POST: FTCVAL == number of utilities owned by the player
    {
        return numUtilities;
    }
    
    public int GetNumRailroads()
    // POST: FTCVAL == number of railroads owned by the player
    {
        return numRailroads;
    }
    
    public int GetPositon()
    // POST: FCTVAL == current position of the player on the board
    {
        return position;
    }
    
    public int GetMoney()
    // POST: FTCVAL == amount of money the player has, in Monopoly dollars
    {
        return money;
    }
    
    @Override
    public String toString()
    // POST: FCTVAL == string representation of a Player object        
    {
        String temp = new String();       // resulting output string
        temp = token + " at position " + position + " has $" + money;
        
        if (numProperties > 0)
            return  temp + "\n   owns: " + GetProperties();
        else
            return temp + ", owns no properties";
    }
}