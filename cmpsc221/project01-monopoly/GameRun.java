// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Monopoly game main driver
// Description: Driver for all Monopoly gameplay

public class GameRun 
{
    public static void main(String[] args)
    {
        Player[] players = {new Player("Car"), new Player("Thimble"),     // Players in the game
                            new Player("Dog")};
        Game monopolyGame = new Game(players);                            // Monopoly game object
        String[] preTurnOptions = {"Buy or sell houses/hotels",           // Option a player has at 
            "Roll the dice", "View all game details", "End the game\n"};  //  beginning of his/her turn
        int option;                                                       // Option chosen by user
        boolean rolledDoubles;                        // True if player rolled two of the same number    
        
        monopolyGame.SetupDemoMode();               // Give the players several properties to start
        monopolyGame.PopUpAllBoardInfo();
        System.out.println("\nThe Players are: " + monopolyGame.GetPlayerNames());
        System.out.println("First player is " + monopolyGame.GetCurrentPlayer().GetToken());
                
        do 
        {
            rolledDoubles = false;
            // Precede each turn by displaying where all players are on the board, 
            // how much money each player has, and a list of each players' properties
            System.out.println("\n" + monopolyGame.GetPlayerInfo());
            
            // Begin each turn by giving the options to manage houses/hotels, roll the dice, 
            // view all details of the board, or end the game.
            do
            {
                System.out.println("Current player is " + monopolyGame.GetCurrentPlayer().GetToken());
                
                option = ActionsMenu.runActionsMenu(preTurnOptions);
                
                switch(option)
                {
                    case 0:                 // buy or sell houses/hotels
                        BuyOrSellHouses(monopolyGame);
                        break;

                    case 1:                 // roll the dice and move player
                        rolledDoubles = RollDiceAndMove(monopolyGame);
                        break;   

                    case 2:                 // View all board details
                        monopolyGame.PopUpAllBoardInfo();
                        break;
                }
                
                if (rolledDoubles)
                    System.out.println("\n" + monopolyGame.GetCurrentPlayer().GetToken() 
                            + " rolled doubles, so it is " + monopolyGame.GetCurrentPlayer().GetToken() 
                            + "'s turn again");
            }
            while((option != 1 || rolledDoubles) && option != 3);    // continue until user doesn't roll 
                                                                     // doubles or wants to end the game
            monopolyGame.ChangeCurrentPlayer();     // next player gets his/her turn
        }
        while(option != 3);                        // continue until player want to end the game
        
        monopolyGame.PrintBoardLocations();
        System.out.println("\n" + monopolyGame.GetPlayerInfo());
        System.out.println("Thank you for playing!");
    }
        
    public static void BuyOrSellHouses(Game monopolyGame)
    // PRE: monopolyGame is initialized
    // POST: Handles processes for buying or selling houses on Lots
    {
        int action;                      // User choice, (0) buy or (1) sell a house
        Lot[] improvableLots;            // Lots that can be improved by the current Player
        Lot[] unimprovableLots;          // Lots that can be unimproved by the current Player
        int lotToImproveIndex;           // Index in improvableLots of Lot to improve
        int lotToUnimproveIndex;         // Index in unimprovableLots of Lot to unimprove
        String[] improvableLotNames;     // Names of Lots that can be improved by the current Player
        String[] unimprovableLotNames;   // Names of Lots that can be unimproved by the current Player
        Lot lotToUnimprove;              // Lot that a player wants to unimprove
        String[] buySellHouses = {"Improve a lot", "Unimprove a lot"};  // Options for player to improve
                                                                        // or unimprove a Lot
        action =  ActionsMenu.runActionsMenu(buySellHouses);
        if (action == 0)        // if player wants to improve a Lot
        {
            improvableLots = monopolyGame.GetCurrentPlayer().GetImprovableLots();
            improvableLotNames = monopolyGame.GetCurrentPlayer().GetImprovableLotNames();

            if (improvableLots.length == 0)     // if the current player has improvable Lots
            {
                System.out.println("\nYou do not have any lots that may be improved");
            }
            else                                // otherwise print improvable Lots
            {
                System.out.println("\nYou may improve the following lots:");
                lotToImproveIndex = ActionsMenu.runActionsMenu(improvableLotNames);
                
                // if player was able to improve that Lot
                if (monopolyGame.GetCurrentPlayer().GetImprovableLots()[lotToImproveIndex].BuildHouse())
                    System.out.println(monopolyGame.GetCurrentPlayer().
                            GetImprovableLots()[lotToImproveIndex]);
                // otherwise tell user that the player could not improve that Lot
                else       
                    System.out.println(improvableLots[lotToImproveIndex].GetName() 
                            + " could not be improved");
            }
        }
        else                               // if player wants to unimprove a Lot
        {
            unimprovableLots = monopolyGame.GetCurrentPlayer().GetUnimprovableLots();
            unimprovableLotNames = monopolyGame.GetCurrentPlayer().
                    GetUnimprovableLotNames();

            if (unimprovableLots.length == 0) // if current player has improvable Lots
            {
                System.out.println("\nYou do not have any lots that may be unimproved");
            }
            else                                // otherwise print unimprovable Lots
            {
                System.out.println("\nYou may unimprove the following lots:");
                
                lotToUnimproveIndex = ActionsMenu.runActionsMenu(unimprovableLotNames);
                lotToUnimprove = monopolyGame.GetCurrentPlayer().
                        GetUnimprovableLots()[lotToUnimproveIndex];

                // if player was able to unimprove that Lot
                if (monopolyGame.GetCurrentPlayer().
                        GetUnimprovableLots()[lotToUnimproveIndex].SellHouse())
                    System.out.println(lotToUnimprove.GetName() 
                            + " was unimproved");
                // otherwise tell user that the player could not unimprove that Lot
                else       
                    System.out.println(unimprovableLots[lotToUnimproveIndex].GetName() 
                            + " could not be unimproved");
            }
        }
        System.out.println("\n" + monopolyGame.GetCurrentPlayer() + "\n");
    }
    
    public static boolean RollDiceAndMove(Game monopolyGame)
    // PRE: monopolyGame is initialized
    // POST: Handles processes for rolling the dice, moving player and performing any allowed actions;
    //       returns true if player rolled doubles
    {
        BoardLocation currentSpace;      // Space the current player is on
        int die1;                        // First six-sided die
        int die2;                        // Second six-sided die
        int diceSum;                     // Sum of the two six-sided dice
        int action;                      // Chosen action after landing on a space
        int amount;                      // Amount of money a player gains or loses, in Monopoly dollars
        String amountString;             // Amount of money due in Monopoly dollars, as a string
        Property tempProp;               // Temp Property, used to pay rent to its owner
        String[] possibleActions;        // Actions the player may perform on the current space
        String curPlayerName;            // Name of current Player
        int curPlayerPos;                // Position of the current Player on the board, 
                                         // number of spaces from "GO"
        
        curPlayerName = monopolyGame.GetCurrentPlayer().GetToken();
        
        die1 = (int)(Math.random()*6 + 1);         // roll the dice
        die2 = (int)(Math.random()*6 + 1);
        diceSum = die1 + die2;

        System.out.println("\n" + curPlayerName + " rolled " + die1 + " and " + die2);
        if (monopolyGame.GetCurrentPlayer().MovePlayer(diceSum)) // move player, if player passed "GO"
        {                                                        
            System.out.println(curPlayerName + " passed 'GO' and collects $200, now has $" 
                    + monopolyGame.GetCurrentPlayer().GetMoney());
            System.out.println();
        }
        curPlayerPos = monopolyGame.GetCurrentPosition();

        System.out.println(curPlayerName + " is now at position " + curPlayerPos
                + ", " + monopolyGame.GetCurrentSpaceName());

        currentSpace = monopolyGame.GetGameBoard()[monopolyGame.GetCurrentPlayer().GetPositon()];
        possibleActions = currentSpace.GetPossibleActions(monopolyGame.GetCurrentPlayer(), diceSum);

        action = ActionsMenu.runActionsMenu(possibleActions);
        
        if (possibleActions[action].contains("Purchase"))   // if player wants to purchase the property
        {
            // if able to purchase the property
            if (monopolyGame.GetCurrentPlayer().PurchaseProperty((Property)monopolyGame.GetGameBoard()
                    [monopolyGame.GetCurrentPlayer().GetPositon()]))
                System.out.println(curPlayerName + " purchased " + currentSpace.GetName()); 
            
            else
                System.out.println(curPlayerName + " was unable to purchase " + currentSpace.GetName());
        }
        else if (possibleActions[action].contains("Pay rent"))  // if player must pay rent
        {
            // index of '$' is 22 in this option, rent due is all chars after '$'
            amountString = possibleActions[action].substring(23);
            amount = Integer.parseInt(amountString);                    // convert amountString to int
            monopolyGame.GetCurrentPlayer().SubtractMoney(amount);      // player pays rent
            
            // get property the current player landed on
            tempProp = (Property)monopolyGame.GetGameBoard()[curPlayerPos];         
            
            // Add rent amount to the property owner's money
            ((Property)monopolyGame.GetGameBoard()[curPlayerPos]).GetOwner().              
                    AddMoney(((Property)monopolyGame.GetGameBoard()[curPlayerPos]).GetRentDue(diceSum));

            System.out.println(curPlayerName + " now has $" 
                    + monopolyGame.GetCurrentPlayer().GetMoney());
            
            System.out.println(((Property)monopolyGame.GetGameBoard()
                    [curPlayerPos]).GetOwner().GetToken() + " now has $" 
                    + ((Property)monopolyGame.GetGameBoard()[curPlayerPos]).
                    GetOwner().GetMoney() + "\n");
        }
        else if (possibleActions[action].contains("Tax"))    // if player must pay tax
        {
            // index of '$' is 28 in this option, rent due is all chars after '$'
            amountString = possibleActions[action].substring(29);
            amount = Integer.parseInt(amountString);                    // convert amountString to int
            monopolyGame.GetCurrentPlayer().SubtractMoney(amount);      // player pays rent
            
            System.out.println(curPlayerName + " now has $" 
                    + monopolyGame.GetCurrentPlayer().GetMoney() + "\n");
        }
        else if (possibleActions[action].contains("gain")) // if player gains money from a card square
        {
            // index of '$' is 9 in this option, rent due is all chars after '$'
            amountString = possibleActions[action].substring(10);
            amount = Integer.parseInt(amountString);                      // convert amountString to int
            monopolyGame.GetCurrentPlayer().AddMoney(amount);             // player gains money
            
            System.out.println(curPlayerName + " now has $" 
                    + monopolyGame.GetCurrentPlayer().GetMoney() + "\n");
        }
        else if (possibleActions[action].contains("lose")) // if player loses money from a card square
        {
            // index of '$'0 is 9 in this option, rent due is all chars after '$'
            amountString = possibleActions[action].substring(10);
            amount = Integer.parseInt(amountString);                      // convert amountString to int
            monopolyGame.GetCurrentPlayer().SubtractMoney(amount);        // player gains money
            
            System.out.println(curPlayerName + " now has $" 
                    + monopolyGame.GetCurrentPlayer().GetMoney() + "\n");
        }
        else if (possibleActions[action].contains("enough"))   // if player was unable to pay amount due
        {
            monopolyGame.GetCurrentPlayer().SubtractMoney(monopolyGame.GetCurrentPlayer().GetMoney());
            System.out.println(curPlayerName + " has gone bankrupt!");
        }

        return die1 == die2;        // true if player rolled doubles
    }
}