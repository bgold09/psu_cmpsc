// Name:        Brian Golden
// Section:     001
// Date:        09/13/11
// Program:     Lab 07: Bank class
// Description: Bank class, used to represent a bank type building with name, description, address and 
//              number of accounts

public class Bank extends BusinessBuilding
{
    private int numAccounts;       // Number of accounts open in this bank
    
    public Bank()
    // POST: A default Bank has been constructed with blank streetAddress, name and description; 
    //       zip and numAccounts of 0
    {
        this(" ", " ", 0, " ", 0);
    }
    
    public Bank(String name, String streetAddress, int zip, String description, int numAccounts)
    // PRE:  streetAddress, name, description are initialized; 
    //       zip is a valid 5-digit US ZIP code; numAccounts >= 0
    // POST: A Bank object has been constructed with data members name, streetAddress, zip, description
    //       and numAccounts set the values passed in the corresponding parameters    
    {
        super(name, streetAddress, zip, description);       // Call BusinessBuilding init constructor
        this.numAccounts = numAccounts;
    }
    
    @Override
    public String toString()
    // POST: FCTVAL == a String representation of the name, address, description and 
    //       number of accounts of this Bank
    {
        return super.toString() +"\nNumber of open accounts: " + numAccounts;
    }
}
