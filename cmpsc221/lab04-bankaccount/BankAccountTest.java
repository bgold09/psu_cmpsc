// Name:        Brian Golden
// Section:     001
// Date:        09/01/11
// Program:     Lab 4: BankAccount Test Driver
// Description: Test program for BankAccount class

public class BankAccountTest
{
	public static void main(String[] args)
	{
		BankAccount acct1 = new BankAccount();				// Some 3 bank accts
		BankAccount acct2 = new BankAccount("savings");
		BankAccount acct3 = new BankAccount(35.50);
		double withdrawAmount = 20.88;						// amount to withdraw from acct3
		
		// Print initial balances of each account
		System.out.printf("Account 1 initial balance: $%.2f\n", acct1.GetBalance());	
		System.out.printf("Account 2 initial balance: $%.2f\n", acct2.GetBalance());
		System.out.printf("Account 3 initial balance: $%.2f\n", acct3.GetBalance());
		
		// Print initial account types of each account
		System.out.println("\nAccount 1 type: " + acct1.GetType());
		System.out.println("Account 2 type: " + acct2.GetType());
		System.out.println("Account 3 type: " + acct3.GetType());
		
		acct2.ResetAccount("Brian Golden", 41.60, "checking");	// Reset class members of acct2
		System.out.println("\nAccount 2 current type: " + acct2.GetType());	
		System.out.printf("Account 2 current balance: $%.2f\n", acct2.GetBalance());
		
		if (acct3.Withdraw(withdrawAmount))		// withdrawl successful
			System.out.printf("\n$%.2f  successfully withdrawn from account 3", withdrawAmount);
		else									// withdrawl unsuccessful
			System.out.printf("\n$%.2f could not be withdrawn from account 3", withdrawAmount);
		
		System.out.printf("\nAccount 3 current balance: $%.2f\n", acct3.GetBalance());
	}
}