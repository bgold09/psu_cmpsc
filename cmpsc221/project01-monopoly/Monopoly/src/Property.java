// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Property class
// Description: Implementation for a Property class; Property objects have a name, address, owner and
//              purchaseCost; Property objects may model a Lot, Railroad or Utility object

public abstract class Property extends BoardLocation
{
    protected int purchaseCost;     // cost to purchase the property, in Monopoly dollars 
    protected Player owner;         // the current owner of the property
    
    public Property()
    // POST: A default Property object is created with name set to blank, 
    //       rentAmount and purchaseCost set to 0
    {
        this("", 0, 0);
    }
    
    public Property(String name, int address, int purchaseCost)
    // PRE: name and owner are initialized; 0 <= address < 40, purchaseCost > 0        
    // POST: A Property object is created with name, address and purchaseCost set to 
    //       their corresponding input parameters
    {
        super(name, address);               // Call BoardLocation init constructor
        this.purchaseCost = purchaseCost;
    }
    
    @Override
    public String[] GetPossibleActions(Player player, int diceSum)
    // PRE:  player is initialized, 2 <= diceSum <= 12 
    // POST: FCTVAL == array of options player has upon landing on this space, to be used in a menu in 
    //       a user interface; diceSum used to compute rent when landing on a Utility
    {
        String[] result;                        // Actions available to the player
        int counter = 0;                        // Index of result actions
        
        if (owner == null)                      // If the property has no owner
        {                                       // Player may purchase the property or do nothing
            if (player.GetMoney() > purchaseCost) // if player has enough money to purchase the property
            {
                result = new String[2];
                result[counter] = "Purchase " + name + " for $" + purchaseCost;
                result[counter+1] = "Do nothing";
                counter += 2;
            }
            else                          // player does not have enough money to purchase this property
            {
                result = new String[1];
                result[counter] = "Do nothing";
            }
        }
        else if (owner.GetToken().equals(player.GetToken()))   // If player is the owner, do nothing
        {
            result = new String[1];
            result[counter] = "Do nothing";
        }
        else                                                   // If property is owned by another player
        {                                                      // pay rent due
            if (player.GetMoney() > GetRentDue(diceSum))
            {
                result = new String[1];
                result[counter] = "Pay rent, amount due: $" + GetRentDue(diceSum);
            }
            else
            {
                result = new String[1];
                result[counter] = "You do not have enough money to pay rent";
            }
        }
        
        return result;
    }
    
    public void ChangeOwner(Player player)
    // PRE: player is initialized
    // POST: the property's owner is changed to player
    {
        owner = player;
    }
    
    public abstract int GetRentDue(int diceSum);
    // PRE: 2 <= diceSum <= 12
    // POST: FCTVAL == amount of rent due to the player, diceSum used to compute rent for Utility object
    
    public int GetPurchaseCost()
    // POST: FCTVAL == cost of the property
    {
        return purchaseCost;
    }
    
    public Player GetOwner()
    // POST: FCTVAL == current owner of the property
    {
        return owner;
    }
    
    public String GetOwnerToken()
    // POST: FCTVAL == token of current owner of the property, "Bank" if no owner
    {
        if (owner == null)
            return "Bank";
        else        
            return owner.GetToken();
    }
    
    @Override
    public String toString()
    // POST: Returns a string representation of the Property object
    {
        return super.toString() + " [cost = $" + purchaseCost + " , owner: " + GetOwnerToken();
    }
}