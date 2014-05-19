// Name:        Brian Golden
// Section:     001
// Date:        09/06/11
// Program:     Lab 05: Video Game Enemy Class
// Description: Implementation for an Enemy class;
//				Enemy objects have strength level, current vertical position, 
//				defense level and active/inactive state

public class Enemy
{
	private static final int THREAT_THRESHOLD = 15;	// The level where the enemy poses no threat to 
													// the player when strength is less than this
	private static boolean isActive = true;			// True when enemies are active, false when inactive
	
	private int currentLevel;		// Current vertical level
	private int strength;			// Current strength level, measured from 0 to 100
	private final int defenseLevel;	// Amount of damage a hit from player does, random value in 
									// range 1 to 20
									
	public Enemy()
	// POST: A default Enemy object is created with strength set to 100,
	//		 currentLevel set to 10 and defenseLevel set a random value from 1 to 20
	{
		this(10, (int)(20 * Math.random()) + 1);  // Make defense level random value between 1 and 20						
	}
	
	public Enemy(int startingLevel, int defenseLevel)
	// PRE: 0 <= startingLevel; 0 < defenseLevel <= 20
	// POST: An Enemy object is created with strength set to 100, currentLevel set to startingLevel
	//       and the class member defenseLevel set to defenseLevel
	{
		strength = 100;						// Start enemy at full strength
		currentLevel = startingLevel;
		this.defenseLevel = defenseLevel;
	}
	
	public void AttackEnemy()
	// POST: Damage is dealt to enemy according to class member defenseLevel
	{
		if (strength > defenseLevel)  // More strength than dealt damage, simply subtract from strength
			strength = strength - defenseLevel;
		else
			strength = 0;
	}
	
	public static void MakeEnemiesInactive()
	// POST: All Enemy objects will be made inactive; set isActive to false
	{
		isActive = false;	
	}
	
	public static void MakeEnemiesActive()
	// POST: All Enemy objects will be made active; set isActive to true
	{
		isActive = true;	
	}
	
	public int GetStrength()
	// POST: FCTVAL == strength
	{
		return strength;
	}
	
	public int GetDangerLevel(int playerLevel)
	// PRE: playerLevel >=0
	// POST: FTCVAL == danger this Enemy poses to a player on playerLevel
	{
		if (isActive && strength >= THREAT_THRESHOLD)
		// When enemy is active and strength >= 15, danger is defined as the strength - distance between
		// player and enemy
			return (strength - Math.abs(currentLevel - playerLevel));	
		else
			return 0;				// No danger if strength is below THREAT_THRESHOLD
	}
	
	public boolean IsStronger(Enemy compareTo)
	// POST: Returns true when strength of this is greater than compareTo, false when strength of 
	//       compareTo is greater than or equal to this
	{
		return (strength > compareTo.GetStrength());
	}
	
	public String toString()
	// POST: Returns a string format for the Enemy object
	{
		String aString = new String();
		
		if (isActive)
			aString = "Enemy is active; ";
		else
			aString = "Enemy is inactive; ";
		aString = "strength = " + strength + "; current level = " + currentLevel 
				  + "; defense level = " + defenseLevel;
		
		return aString;
	}	
}