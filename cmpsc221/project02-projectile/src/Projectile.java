// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     Projectile class
// Description: Implementation for an abstract Projectile class; Projectile objects have 
//				a position, side length and color

import java.awt.Color;
import java.awt.Graphics;

public abstract class Projectile
{
	protected Point position;				// position of the top-left corner of this Projectile,
											//  in percentages of the width and height, respectively 
	protected double sideLength;			// length of each side of this Projectile, in percentages 
											//  of the width and height for horizontal and 
											//  vertical sides, respectively
	protected Color color;					// color of this Projectile
	
	public Projectile(Point position, double sideLength, Color color)
	// PRE: position and color are initialized, sideLength > 0 
	// POST: A Projectile object has been created with position, sideLength and color 
	//       set to their corresponding input parameters
	{
		this.position = position;
		this.sideLength = sideLength;
		this.color = color;
	}
		
	public abstract void DrawProjectile(Graphics g, double width, double height);
	// PRE: g is the current graphics context, width and height are of the applet, in pixels
	// POST: this Projectile has been drawn to the applet
		
	public void SetPosition(Point point)
	// PRE: point is initialized
	// POST: position has been set to point
	{
		position = point;
	}
	
	public double GetSideLength()
	// POST: FCTVAL == sideLength
	{
		return sideLength;
	}
	
	public Point GetPosition()
	// POST: FCTVAL == position
	{
		return position;
	}
}