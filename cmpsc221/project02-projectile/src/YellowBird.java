// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     YellowBird class
// Description: Implementation for an YellowBird class; YellowBird objects have a position, side length
//				and color; YellowBird is a subclass of Projectile

import java.awt.Color;
import java.awt.Graphics;

public class YellowBird extends Projectile
{
	private static final Color YB_COLOR = Color.YELLOW;	// color of all YellowBird objects
	private static final double SIDE_LENGTH = .028;		// length of each side of this YellowBird, in 
														//  percentages of the width and height for 
														//  horizontal and vertical sides, respectively
	
	public YellowBird(Point position)
	// PRE: position is initialized
	// POST: A YellowBird object has been created class member position set to 
	//		 input parameter position, sideLength set to SIDE_LENGTH and color set to YB_COLOR
	{
		super(position, SIDE_LENGTH, YB_COLOR);
	}
	
	@Override
	public void DrawProjectile(Graphics g, double width, double height)
	// PRE: g is the current graphics context, width and height are of the applet, in pixels
	// POST: this YellowBird has been drawn to the applet
	{
		double minDimension;								// the smaller of the two dimensions,
															//   either width or height

		if (width < height)									// if width is the smaller dimension
		{
			minDimension = width; 
		}
		else												// otherwise set minDimension to height
		{
			minDimension = height;
		}

		g.setColor(color);

		g.fillOval((int)(position.GetXCoordinate()*width), 
				   (int)(position.GetYCoordinate()*height), 
				   (int)(sideLength*minDimension), 
				   (int)(sideLength*minDimension));
	}
}