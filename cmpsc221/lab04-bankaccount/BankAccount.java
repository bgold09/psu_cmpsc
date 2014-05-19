// Name:        Brian Golden
// Section:     001
// Date:        09/01/11
// Program:     Lab 04: BankAccount Enhancement 
// Description: Implementation for a bank account class
//				BankAccount objects have a name, balance and account type

public class BankAccount
{
    private String name;      // name of account holder
    private double balance;   // how much money is in account, in dollars
	private String type;	  // type of account: "checking" or "savings"
	
    public BankAccount()
    // POST: A default BankAccount object is created with the name set to a blank,
	//		 type set to default "savings" and the balance set to $0.00
    {
       this(0.0); 
    }

    public BankAccount(double balance)
    // PRE:  balance >= 0.00 and is in dollars
    // POST: A BankAccount object is created with the name set to a blank,
	//		 account type set to default type of "savings" and the class member 
	//       balance set to balance
    {
       name = " ";
	   type = "savings";
      
       if(balance >= 0)              // validate proposed initial balance
			this.balance = balance; 
       else
			this.balance = 0;
    }
	
	public BankAccount(String type)
	// PRE: type.equals("checking") || type.equals("savings")
	// POST: A BankAccount object is created with the name set to a blank,
	//       the balance set to 0.00, and the class member type set to type
	{
		name = " ";
		balance = 0;
		
		type = type.toLowerCase();								// Make check case-insensitive
		if (type.equals("checking") || type.equals("savings"))	// Perform validation
			this.type = type;
		else
			this.type = "savings";								// Otherwise set default type "savings"
	}

    public void ResetAccount(String newName, double newBalance, String newType)
    // PRE:  newName has been assigned a value, newType is either "savings"
	//       or "checking" && newBalance >= 0.00 and is in dollars
    // POST: The account object is reset with the name set to newName,
    //       the balance set to newBalance and type set to newType
    {
        name = newName;            	  // Match up private variables with parameters
		
        if(balance >= 0)              // validate proposed initial balance
			this.balance = newBalance; 
        else
			this.balance = 0; 
		
		newType = newType.toLowerCase();	// Make check case-insensitive
		if (newType.equals("checking") || newType.equals("savings"))	// Set class member type to
			type = newType;									// newType if newType is valid type
		else								// Otherwise set to a default type "savings"
			type = "savings";
    }
	
    public boolean Withdraw(double amount)
    // PRE:  amount > 0, in dollars
    // POST: If amount <= balance, amount is deducted from the account balance 
	//		  and returns true; otherwise return false
    {
		if (amount <= balance)			// If amount is not greater than available balance
		{								// then amount can be withdrawn from the account
			balance = balance - amount;
			return true;
		}
		else 							// If amount is greater than available balance then
		{
			return false; 				// amount cannot be withdrawn from the account
		}
	}

    public double GetBalance() 
    // POST: Returns the current balance of the account
    {
        return balance;
    }

    public void DisplayBalance() 
    // POST: Displays the current balance of the account to the screen
    {
        System.out.printf("Your balance is currently $%.2f\n", balance); 
    }
	
	public String GetType()
	// POST: Returns the type of the account
	{
		return type;
	}
}