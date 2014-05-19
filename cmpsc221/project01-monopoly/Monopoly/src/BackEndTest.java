// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - BackEnd test driver
// Description: Test driver for all backend Monopoly classes

public class BackEndTest 
{
    public static void main (String[] args)
    {
        Player player1 = new Player("Car");
        
        int[] medAveRents = {2, 10, 30, 90, 160, 230};
        int[] balticAveRents = {4, 20, 60, 180, 320, 450};
        int[] orientalAveRents = {6, 30, 90, 270, 400, 550};
        int[] vermontAveRents = {6, 30, 90, 270, 400, 550};
        int[] connAveRents = {8, 40, 100, 300, 450, 600};
        int[] stCharlesRents = {10, 50, 150, 450, 625, 750};
        int[] statesAveRents = {10, 50, 150, 450, 625, 750};
        int[] virginiaAveRents = {12, 60, 180, 500, 700, 900};
                
        // Lot(name, address, purchaseCost, color, numHouses, houseCost, rentStructure)
        CornerSquare goSpace = new CornerSquare("GO", 0);
        Lot medAve = new Lot("Mediterranean Avenue", 1, 60, "dark purple", 50, medAveRents);
        CardSquare commChest1 = new CardSquare("Community Chest", 2);
        Lot balticAve = new Lot("Baltic Avenue", 3, 60, "dark purple", 50, balticAveRents);
        TaxSquare incomeTax = new TaxSquare("Income Tax", 4, 200);
        Railroad readingRR = new Railroad("Reading Railroad", 5, 200);
        Lot orientalAve = new Lot("Oriental Avenue", 6, 100, "light blue", 50, orientalAveRents);
        CardSquare chance1 = new CardSquare("Chance", 7);
        Lot vermontAve = new Lot("Vermont Avenue", 8, 100, "light blue", 50, vermontAveRents);
        Lot connAve = new Lot("Connecticut Avenue", 9, 120, "light blue", 50, connAveRents);
        CornerSquare jail = new CornerSquare("Jail", 10);
        Lot stCharlesAve = new Lot("St. Charles Avenue", 11, 140, "light purple", 100, stCharlesRents);
        Utility electComp = new Utility("Electric Company", 12, 150);
        Lot statesAve = new Lot("States Avenue", 13, 140, "light purple", 100, statesAveRents);
        Lot virginiaAve = new Lot("Virginia Avenue", 14, 160, "light purple", 100, virginiaAveRents);
        
        BoardLocation[] monopolyBoard = {goSpace, medAve, commChest1, balticAve, incomeTax, readingRR, 
                                          orientalAve, chance1, vermontAve, connAve, jail, stCharlesAve, 
                                          electComp, statesAve, virginiaAve};
        
        for (BoardLocation aLocation: monopolyBoard)
        {
            System.out.println(aLocation);
        }
        
        System.out.println("\n" + player1);
        player1.MovePlayer(3);
        System.out.println("\nPlayer 1 is on " + monopolyBoard[player1.GetPositon()].GetName());
        System.out.println("Available actions:");
        for (String temp : monopolyBoard[player1.GetPositon()].GetPossibleActions(player1, 3))
        {
            System.out.println(temp);
        }
        
        if (player1.PurchaseProperty(balticAve))
            System.out.println("\nPurchased Baltic Avenue. Player 1 now has $" + player1.GetMoney());
        else
            System.out.println("\nYou were not able to purchase Baltic Avenue");
        
        if (balticAve.BuildHouse())
            System.out.println("Built a house on Baltic Avenue");
        else
            System.out.println("Unable to build a house on Baltic Avenue");
        System.out.println(balticAve);
        
        System.out.print("\nPlayer 1 owns the following properties: \n" + player1.GetProperties());
        System.out.println("\nPlayer 1 owns " + player1.GetNumUtilities() + " utilities");
        System.out.println("Player 1 owns " + player1.GetNumRailroads() + " railroads");
        
        System.out.println("\nSome other player now lands on Baltic Avenue. He owes $" + balticAve.GetRentDue(3));
        
        player1.MovePlayer(4);
        System.out.println("\nPlayer 1 is on " + monopolyBoard[player1.GetPositon()].GetName());
        System.out.println("Player 1 owes $" + incomeTax.GetAmountDue());
    }
}