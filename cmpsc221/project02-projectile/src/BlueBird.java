// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     BlueBird class
// Description: Implementation for an BlueBird class; BlueBird objects have a position, side length
//				and color; BlueBird is a subclass of Projectile

import java.awt.Color;
import java.awt.Graphics;

public class BlueBird extends Projectile
{
	private static final Color BB_COLOR = Color.BLUE;	// color of all BlueBird objects
	private static final double SIDE_LENGTH = .02;		// length of each side of this BlueBird, in 
														//  percentages of the width and height for 
														//  horizontal and vertical sides, respectively
	
	public BlueBird(Point position)
	// PRE: position is initialized
	// POST: A BlueBird object has been created class member position set to 
	//		 input parameter position, sideLength set to SIDE_LENGTH and color set to BB_COLOR
	{
		super(position, SIDE_LENGTH, BB_COLOR);
	}
	
	@Override
	public void DrawProjectile(Graphics g, double width, double height)
	// PRE: g is the current graphics context, width and height are of the applet, in pixels
	// POST: this BlueBird has been drawn to the applet
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

		g.fillRect((int)(position.GetXCoordinate()*width), 
				   (int)(position.GetYCoordinate()*height), 
				   (int)(sideLength*minDimension), 
				   (int)(sideLength*minDimension));
	}
}