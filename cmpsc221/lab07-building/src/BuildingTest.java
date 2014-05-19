// Name:        Brian Golden
// Section:     001
// Date:        09/13/11
// Program:     Lab 07: Building Test Driver
// Description: Test program for Building class

public class BuildingTest 
{
    public static void main (String[] args)
    {
        Building[] myBuildings = new Building[8];
        
        myBuildings[0] = new Building("45 Burrowes Rd", 60827);
        myBuildings[1] = new BusinessBuilding("Home Depot", "165 Atherton St", 16802, 
                                                "Home improvement store");
        myBuildings[2] = new ResidenceBuilding("20 College Ave", 87732, "apartment", 4);
        myBuildings[3] = new Bank("PNC Bank", "14 Beaver Ave", 16801, "PNC Bank branch #208", 1395);
        myBuildings[4] = new Building();
        myBuildings[5] = new BusinessBuilding();
        myBuildings[6] = new ResidenceBuilding();
        myBuildings[7] = new Bank();
        
        for (Building temp : myBuildings)
        {
            System.out.println(temp + "\n");
        }
    }
}