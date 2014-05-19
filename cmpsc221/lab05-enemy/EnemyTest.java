// Name:        Brian Golden
// Section:     001
// Date:        09/06/11
// Program:     Lab 05: Enemy test driver
// Description: Test program for Enemy class

public class EnemyTest
{
	public static void main(String[] args)
	{
		Enemy enemy1 = new Enemy();
		Enemy enemy2 = new Enemy(5, 10);
		Enemy enemy3 = new Enemy(8, 15);
		int playerLevel = 3;
		
		System.out.println("Player level = " + playerLevel);
		System.out.println("Enemy 1 --> " + enemy1.toString());
		System.out.println("Enemy 1 threat level = " + enemy1.GetDangerLevel(playerLevel));
		System.out.println("Enemy 2 --> " + enemy2.toString());
		System.out.println("Enemy 2 threat level = " + enemy2.GetDangerLevel(playerLevel));
		System.out.println("Enemy 3 --> " + enemy3.toString());
		System.out.println("Enemy 3 threat level = " + enemy3.GetDangerLevel(playerLevel));
		
		if (enemy1.IsStronger(enemy2))
			System.out.println("Enemy 1 is stronger than Enemy 2");
		else
			System.out.println("Enemy 1 is not stronger than Enemy 2");
		
		System.out.println("\nAttack Enemy 1 and 2 once, attack Enemy 3 six times: ");
		enemy1.AttackEnemy();
		enemy2.AttackEnemy();
		for (int i = 0; i < 6; i++)
		{
			enemy3.AttackEnemy();
		}
		
		System.out.println("Enemy 1 strength = " + enemy1.GetStrength());
		System.out.println("Enemy 1 threat level = " + enemy1.GetDangerLevel(playerLevel));
		System.out.println("Enemy 2 strength = " + enemy2.GetStrength());
		System.out.println("Enemy 2 threat level = " + enemy2.GetDangerLevel(playerLevel));
		System.out.println("Enemy 3 strength = " + enemy3.GetStrength());
		System.out.println("Enemy 3 threat level = " + enemy3.GetDangerLevel(playerLevel));
		
		System.out.println("\nActivate invincibility!");
		Enemy.MakeEnemiesInactive();
		System.out.println("Enemy 1 threat level = " + enemy1.GetDangerLevel(playerLevel));
		System.out.println("Enemy 2 threat level = " + enemy2.GetDangerLevel(playerLevel));
		System.out.println("Enemy 3 threat level = " + enemy3.GetDangerLevel(playerLevel));
		
		System.out.println("\nInvincibility disabled!");
		Enemy.MakeEnemiesActive();
		System.out.println("Enemy 1 threat level = " + enemy1.GetDangerLevel(playerLevel));
		System.out.println("Enemy 2 threat level = " + enemy2.GetDangerLevel(playerLevel));
		System.out.println("Enemy 3 threat level = " + enemy3.GetDangerLevel(playerLevel));
	}	
}