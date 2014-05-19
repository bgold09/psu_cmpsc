// Name:        Brian Golden and Garrett Gitterman
// Section:     001
// Date:        09/29/11
// Program:     Project 01 - Front end monopoly test driver
// Description: Front-end test driver for Monopoly game setup

public class FrontEndTest
{
    public static void main(String[] args)
    {
        Player[] players = {new Player("Car"), new Player("Thimble"),
                            new Player("Dog")};
        Game monopolyGame = new Game(players);                          
        monopolyGame.PrintBoardLocations();
        System.out.println("\nThe Players are: " + monopolyGame.GetPlayerNames());
        System.out.println("First player is " + monopolyGame.GetCurrentPlayer().GetToken());
        monopolyGame.SetupDemoMode();
        System.out.println("\n" + monopolyGame.GetPlayerInfo());
    }
}