// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     RedBird class
// Description: Implementation for an RedBird class; RedBird objects have a position, side length
//				and inner and outer color; RedBird is a subclass of Projectile

import java.awt.Color;
import java.awt.Graphics;

public class RedBird extends Projectile
{
	private static final double SCALE = .003;			// scaling factor for inner rect of this RedBird
	private static final Color OUT_COLOR = Color.RED;	// color of outer square RedBird objects
	private static final Color IN_COLOR = Color.BLACK;	// color of inner square of RedBird objects
	private static final double SIDE_LENGTH = .023;		// length of each side of this RedBird, in 
														//  percentages of the width and height for 
														//  horizontal and vertical sides, respectively
	
	public RedBird(Point position)
	// PRE: position is initialized
	// POST: A RedBird object has been created with class member position set to 
	//		 input parameter position, sideLength set to SIDE_LENGTH and color set to OUT_COLOR
	{
		super(position, SIDE_LENGTH, OUT_COLOR);
	}
	
	@Override
	public void DrawProjectile(Graphics g, double width, double height)
	// PRE: g is the current graphics context, width and height are of the applet, in pixels
	// POST: this RedBird has been drawn to the applet
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

		g.setColor(OUT_COLOR);								// draw outer rectangle
		g.fillRect((int)(position.GetXCoordinate()*width), 
				   (int)(position.GetYCoordinate()*height), 
				   (int)(sideLength*minDimension), 
				   (int)(sideLength*minDimension));
		
		g.setColor(IN_COLOR);								// draw inner rectangle
		g.fillRect((int)(position.GetXCoordinate()*width + 
					minDimension*SCALE), 
				   (int)(position.GetYCoordinate()*height + 
					minDimension*SCALE), 
				   (int)(sideLength*minDimension - 
					2*minDimension*SCALE), 
				   (int)(sideLength*minDimension - 
					2*minDimension*SCALE));
	}
}