// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     Target class
// Description: Implementation for a Target class; Target objects have vertical and horizontal length, 
// 				coordinates of its top-left corner, color, number and current state (dead or alive)

import java.awt.Color;
import java.awt.Graphics;

public class Target
{	
	private static int numAliveTargets = 0;				// number of Target objects that are alive
	
	private double vertLength;							// vert length of Target object, in pixels
	private double horizLength;							// horiz length of Target object, in pixels
	private boolean isAlive;							// true if this Target has not been hit
	private Point targetCoordinates;					// coordinates of top-left corner of this Target
	private Color curColor;								// current color of this Target
	private int targetNum;								// number of this Target on the stack of Target
														// objects, to help in determining coordinates
	
	public Target()
	// POST: numAliveTargets has been incremented by one; A default Target object is created with 
	// 		 targetNum set to numAliveTargets, curColor to Constants.COLORS[0]; vertLength and 
	//		 horizLength set to TARGET_SIZE_SCALE; isAlive set to true
	{
		this(Constants.COLORS[0]);
	}
		
	public Target(Color color)
	// PRE: color is initialized
	// POST: numAliveTargets has been incremented by one; A Target object is created with targetNum set 
	// 		 to numAliveTargets, curColor to color; vertLength and yPos set to scaled values based 
	//		 on height; horizLength and xPos set to scaled values based on width; isAlive set to true
	{
		numAliveTargets++;
		targetNum = numAliveTargets;
		curColor = color;
		vertLength = Constants.TARGET_SIZE_SCALE; 
		horizLength = Constants.TARGET_SIZE_SCALE;		
		isAlive = true;
		// targetCoordinates is initialized in DrawTarget because it cannot be properly calculated until
		//  the applet has finished loading, giving the constructor proper width and height values
	}
	
	public void DrawTarget(Graphics g, double width, double height)
	// PRE: g is the current graphics context of the applet; 
	//      width > 0, height > 0, both in units of the Cartesian plane
	// POST: vertLength and yPos set to scaled values based on height, horizLength and xPos set to 
	// 		 scaled values based on width; this Target has been drawn to the applet if isAlive == true
	{
		vertLength = Constants.TARGET_SIZE_SCALE; 
		horizLength = Constants.TARGET_SIZE_SCALE;
		
		targetCoordinates = new Point(Constants.TARGET_X_POS, 
				((1-Constants.GROUND_SCALE) - vertLength*targetNum));
		
		if (isAlive)														// if this Target is alive
		{
			g.setColor(curColor);
			g.fillRect((int)(targetCoordinates.GetXCoordinate()*width), 	// draw this Target
						(int)(targetCoordinates.GetYCoordinate()*height), 
						(int)(horizLength*width), (int)(vertLength*height));	
		}
	}
	
	public boolean isHit(Point projectilePos, double width, double height, Projectile proj)
	// PRE: projectilePos is initialized
	// POST: returns true if this Target is alive and the current point intersects this Target
	{
		double projSideLength;					// side length of current projectile, in percentage of 
												//  width or height, whichever is smaller
		double projTopLeftX;					// x coordinate of the projectile's top-left corner
		double projTopLeftY;					// y coordinate of the projectile's top-left corner
		double projTopRightX;					// x coordinate of the projectile's top-right corner
		double projTopRightY;					// y coordinate of the projectile's top-right corner
		double projBottomLeftX;					// x coordinate of the projectile's bottom-left corner
		double projBottomLeftY;					// y coordinate of the projectile's bottom-left corner
		double projBottomRightX;				// x coordinate of the projectile's bottom-right corner
		double projBottomRightY;				// y coordinate of the projectile's bottom-right corner
		double targetLowerX;					// lower x bound of this Target
		double targetUpperX;					// upper x bound of this Target
		double targetLowerY;					// lower y bound of this Target
		double targetUpperY;					// upper y bound of this Target
		
		projSideLength = proj.GetSideLength();
		
		projTopLeftX = projectilePos.GetXCoordinate();
		projTopLeftY = projectilePos.GetYCoordinate();
		projTopRightX = projectilePos.GetXCoordinate() + 
						projSideLength;
		projTopRightY = projectilePos.GetYCoordinate();
		projBottomLeftX = projectilePos.GetXCoordinate();
		projBottomLeftY = projectilePos.GetYCoordinate() + 
						  projSideLength;
		projBottomRightX = projectilePos.GetXCoordinate() + 
						   projSideLength;
		projBottomRightY = projectilePos.GetYCoordinate() + 
						   projSideLength;
		
		targetLowerX = targetCoordinates.GetXCoordinate();
		targetUpperX = targetLowerX + horizLength;
		targetUpperY = targetCoordinates.GetYCoordinate();
		targetLowerY = targetUpperY + vertLength;
		
		if (isAlive)											// if this Target is alive
		{
			if((projTopLeftX >= targetLowerX && 				// if top-left corner of projectile 
				projTopLeftX <= targetUpperX) && 				//  intersects this Target
				(projTopLeftY <= targetLowerY && 
				projTopLeftY >= targetUpperY))
			{
				return true;
			}
			
			if((projTopRightX >= targetLowerX && 				// if top-right corner of projectile
				projTopRightX <= targetUpperX) && 				//  intersects this Target
				(projTopRightY <= targetLowerY && 
				projTopRightY >= targetUpperY))
			{
				return true;
			}
			
			if((projBottomLeftX >= targetLowerX && 				// if bottom-left corner of projectile
				projBottomLeftX <= targetUpperX) && 			//  intersects this Target
				(projBottomLeftY <= targetLowerY && 
				projBottomLeftY >= targetUpperY))
			{
				return true;
			}
			
			if((projBottomRightX >= targetLowerX && 			// if bottom-right corner of projectile
				projBottomRightX <= targetUpperX) && 			//  intersects this Target
				(projBottomRightY <= targetLowerY && 
				projBottomRightY >= targetUpperY))
			{
				return true;
			}
			
			return false;
		}

		return false;											// otherwise this Target hasn't been hit
	}
	
	public void Kill()
	// PRE: isAlive = true
	// POST: this Target has been killed, i.e. isAlive set to false
	{
		isAlive = false;
	}
	
	public void Revive()
	// PRE: isAlive = false
	// POST: this Target has been revived, i.e. isAlive set to true
	{
		isAlive = true;
	}
}