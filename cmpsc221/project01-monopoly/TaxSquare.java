// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - TaxSquare class
// Description: Implementation for a TaxSquare class; 
//              TaxSquare objects have a name and address

public class TaxSquare extends BoardLocation
{
    private final int taxAmount;       // Amount player owes when landing on this TaxSquare
    
    public TaxSquare()
    // POST: A default TaxSquare object is created with name set
    //       to a blank string, address and taxAmount set to 0
    {
        this("", 0, 0);
    }
    
    public TaxSquare(String name, int address, int taxAmount)
    // PRE:  name.equals("luxury tax") || name.equals("income tax"), 0 <= address < 40, taxAmount > 0
    // POST: A TaxSquare object is created with name, address and
    //       taxAmount set to their corresponding input parameters
    {
       super(name, address);
       this.taxAmount = taxAmount;
    }
    
    @Override
    public String[] GetPossibleActions(Player player, int diceSum)
    // PRE: player is initialized, 2 <= diceSum <= 12
    // POST: Returns all possible actions the player has
    {
        String[] temp = new String[1];
                
        if (player.GetMoney() > GetAmountDue())     // if player has more money than amount due
            temp[0] = "Pay " + name + ", amount due: $" + GetAmountDue();
        else                                        // otherwise the player has lost all his/her money
            temp[0] = "You lost all your money to " + name + "!";
        
        return temp;
    }
    
    public int GetAmountDue()
    // POST: FCTVAL == amount the player owes
    {
        return taxAmount;
    }
}