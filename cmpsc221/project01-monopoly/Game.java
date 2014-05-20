// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Game class
// Description: Implementation for a Game class; a Game object has board locations, 
//              participating players, an index of current player and the current sum of 2 6-sided dice

import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;

public class Game 
{    
    private static final int NUM_SPACES = 40;       // Number of spaces on a Monopoly board
    
    private BoardLocation[] gameBoard;      // all the BoardLocation objects that make up the board
    private Player[] players;               // players participating in the game
    private int curPlayerIndex;             // index of the current player in players
    private int curDiceSum;                 // current sum of 2 6-sided dice rolled by curPlayer
    
    public Game()
    // POST: A default Game object is created
    {        
        gameBoard = CreateBoardLocations();
    }
    
    public Game(Player[] players)
    // POST: A Game object is created with class member players set to players and  
    //       curPlayerIndex set to a randomly selected index for players
    {
        gameBoard = CreateBoardLocations();
        curPlayerIndex = (int)(Math.random()*players.length);
        this.players = players;
    }
    
    public BoardLocation[] CreateBoardLocations()
    // POST: returns all of the BoardLocation objects in this Game
    {
        int[] medAveRents = {2, 10, 30, 90, 160, 230};
        int[] balticAveRents = {4, 20, 60, 180, 320, 450};
        int[] orientalAveRents = {6, 30, 90, 270, 400, 550};
        int[] vermontAveRents = {6, 30, 90, 270, 400, 550};
        int[] connAveRents = {8, 40, 100, 300, 450, 600};
        int[] stCharlesRents = {10, 50, 150, 450, 625, 750};
        int[] statesAveRents = {10, 50, 150, 450, 625, 750};
        int[] virginiaAveRents = {12, 60, 180, 500, 700, 900};
        int[] stJamesRents = {14, 70, 200, 550, 750, 950};
        int[] tennAveRents = {14, 70, 200, 550, 750, 950};
        int[] nyAveRents = {16, 80, 220, 600, 800, 1000};
        int[] kentuckyAveRents = {18, 90, 250, 700, 875, 1050};
        int[] indianaAveRents = {18, 90, 250, 700, 875, 1050};
        int[] illinoisAveRents = {20, 100, 300, 750, 925, 1100};
        int[] atlanticAveRents = {22, 110, 330, 800, 975, 1150};
        int[] ventnorAveRents = {22, 110, 330, 800, 975, 1150};
        int[] marvinGardensRents = {24, 120, 360, 850, 1025, 1200}; 
        int[] pacificAveRents = {26, 130, 390, 900, 1100, 1275};
        int[] noCarolinaAveRents = {26, 130, 390, 900, 1100, 1275};
        int[] pennAveRents = {28, 150, 450, 1000, 1200, 1400};
        int[] parkPlaceRents = {35, 175, 500, 1100, 1300, 1500};
        int[] boardwalkRents = {50, 200, 600, 1400, 1700, 2000}; 
        
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
        Railroad pennRR = new Railroad("Pennsylvania Railroad", 15, 200);
        Lot stJamesPlace = new Lot("St. James Place", 16, 180, "orange", 100, stJamesRents);
        CardSquare commChest2 = new CardSquare("Community Chest", 17);
        Lot tennAve = new Lot("Tennessee Avenue", 18, 180, "orange", 100, tennAveRents);
        Lot nyAve = new Lot("New York Avenue", 19, 200, "orange", 100, nyAveRents);
        CornerSquare freeParking = new CornerSquare("Free Parking", 20);
        Lot kentuckyAve = new Lot("Kentucky Avenue", 21, 220, "red", 150, kentuckyAveRents);
        CardSquare chance2 = new CardSquare("Chance", 22);
        Lot indianaAve = new Lot("Indiana Avenue", 23, 220, "red", 150, indianaAveRents);
        Lot illinoisAve = new Lot("Illinois Avenue", 24, 240, "red", 150, illinoisAveRents);
        Railroad boRR = new Railroad("B & O Railroad", 25, 200);
        Lot atlanticAve = new Lot("Atlantic Avenue", 26, 260, "yellow", 150, atlanticAveRents);
        Lot ventnorAve = new Lot("Ventnor Avenue", 27, 260, "yellow", 150, ventnorAveRents);
        Utility waterWorks = new Utility("Water Works", 28, 150);
        Lot marvinGardens = new Lot("Marvin Gardens", 29, 280, "yellow", 150, marvinGardensRents);
        CornerSquare goToJail = new CornerSquare("Go to Jail", 30);
        Lot pacificAve = new Lot("Pacific Avenue", 31, 300, "green", 200, pacificAveRents);
        Lot noCarolinaAve = new Lot("North Carolina Avenue", 32, 300, "green", 200, noCarolinaAveRents);
        CardSquare commChest3 = new CardSquare("Community Chest", 33);
        Lot pennsylvaniaAve = new Lot("Pennsylvania Avenue", 34, 320, "yellow", 200, pennAveRents);
        Railroad shortLineRR = new Railroad("Short Line Railroad", 35, 200);
        CardSquare chance3 = new CardSquare("Chance", 36);
        Lot parkPlace = new Lot("Park Place", 37, 350, "dark blue", 200, parkPlaceRents);
        TaxSquare luxuryTax = new TaxSquare("Luxury Tax", 38, 75);
        Lot boardwalk = new Lot("Boardwalk", 39, 400, "dark blue", 200, boardwalkRents);
        
        BoardLocation[] temp = {goSpace, medAve, commChest1, balticAve, incomeTax, readingRR, 
                                orientalAve, chance1, vermontAve, connAve, jail, stCharlesAve, 
                                electComp, statesAve, virginiaAve, pennRR, stJamesPlace, commChest2,  
                                tennAve, nyAve, freeParking, kentuckyAve, chance2, indianaAve, 
                                illinoisAve, boRR, atlanticAve, ventnorAve, waterWorks, marvinGardens,  
                                goToJail, pacificAve, noCarolinaAve, commChest3, pennsylvaniaAve, 
                                shortLineRR, chance3, parkPlace, luxuryTax, boardwalk};        
        return temp;
    }
    
    public void SetupDemoMode()
    // PRE: number of players is 3
    // POST: Sets up a demo mode in which each player already owns three properties
    {
        players[0].PurchaseProperty((Lot)gameBoard[1]);
        players[0].PurchaseProperty((Lot)gameBoard[13]);
        players[0].PurchaseProperty((Lot)gameBoard[18]);
        players[1].PurchaseProperty((Railroad)gameBoard[5]);
        players[1].PurchaseProperty((Lot)gameBoard[8]);
        players[1].PurchaseProperty((Lot)gameBoard[24]);
        players[2].PurchaseProperty((Utility)gameBoard[12]);
        players[2].PurchaseProperty((Lot)gameBoard[6]);
        players[2].PurchaseProperty((Lot)gameBoard[19]); 
    }
    
    public void ChangeCurrentPlayer()
    // POST: curPlayerIndex is changed to index of next player to go in the game
    {
        if (curPlayerIndex < players.length-1)      // If current player is not the last one in players,
            curPlayerIndex++;                       //  simply increment the index
        else                                        // Otherwise if current player is last in players,
            curPlayerIndex = 0;                     //  set the index to the beginning of players
    }
    
    public void PrintBoardLocations()
    // POST: Prints out all the BoardLocation objects in this game
    {        
        for (BoardLocation temp : gameBoard)
        {
            System.out.println(temp);
        }
    }
    
    public Player GetCurrentPlayer()
    // POST: Returns the current player
    {
        return players[curPlayerIndex];
    }
    
    public String GetPlayerNames()
    // POST: Prints out the names of every player's token
    {
        String result = "";
        for (Player temp : players)
        {
            result += temp.GetToken() + "   ";
        }
        return result;
    }
    
    public String GetPlayerInfo()
    // POST: Returns an array of the players in this Game
    {
        String output = "";
        for (int i = 0; i < players.length; i++)
        {
            output += players[i].toString() + "\n";
        }
        return output;
    }
    
    public String GetCurrentSpaceName()
    // POST: FCTVAL == name of current player's location on the board
    {
        return gameBoard[players[curPlayerIndex].GetPositon()].GetName();
    }
        
    public int GetCurrentPosition()
    // POST: FCTVAL == position of the current player, number of spaces from "GO"
    {
        return players[curPlayerIndex].GetPositon();
    }
    
    public String GetBoardInfo()
    // POST: Returns string with information about all spaces on the board
    {
        String result = "";
        for (BoardLocation temp : gameBoard)        // add each BoardLocation's info to result
        {
            result += temp.toString() + "\n\n";
        }
        return result;
    }
    
   public void PopUpAllBoardInfo()
   // POST: A scrolling window with all information about gameBoard is displayed
   {
      String result;                                  // Output message displayed in the window
    
      JTextArea area;                                 // Text area to hold the message
      JScrollPane pane;                               // Window pane with scrollbar containing text area

      result = GetBoardInfo();                        // resulting output text
  
      area = new JTextArea(result); 
      area.setRows(40); 
      area.setColumns(58);
      pane = new JScrollPane(area);
      JOptionPane.showMessageDialog(null, pane, "Monopoly Board Info.", JOptionPane.PLAIN_MESSAGE);   
   } 
    
    public BoardLocation[] GetGameBoard()
    // POST: returns all locations on the board
    {
        return gameBoard;
    }
}