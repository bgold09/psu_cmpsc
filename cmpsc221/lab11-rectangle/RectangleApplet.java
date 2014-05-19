// Name:        Brian Golden
// Section:     001
// Date:        10/06/11
// Program:     Lab 11: Rectangle Applet
// Description: Displays rectangle filled with user-input color and name inside the rectangle

import java.awt.FlowLayout;
import java.awt.Graphics;
import javax.swing.*;
import java.awt.Color;

public class RectangleApplet extends JApplet
{
	private static final int BORDER = 50;	// amount of whitespace between 
											// each side of the rect and the window

    private int redChannel;             // Number value of the red channel in RGB system (0-255 scale)
    private int greenChannel;           // Number value of the green channel in RGB system (0-255 scale)
    private int blueChannel;            // Number value of the blue channel in RGB system (0-255 scale)
	
	@Override
    public void init()
    {
		setLayout(new FlowLayout());
		
		redChannel = getChannel("red");			// Get number value for red channel
		greenChannel = getChannel("green");		// Get number value for green channel
		blueChannel = getChannel("blue");		// Get number value for blue channel
    }
	
    @Override
    public void paint(Graphics g)                        
    {
		int width = getWidth();					// current width of this applet
		int height = getHeight();				// current height of this applet
		int rectWidth = width - 2*BORDER;		// width of the rectangles to draw
		int rectHeight = height - 2*BORDER;		// height of the rectangles to draw
		
		super.paint(g);
		
		g.setColor(new Color(redChannel, greenChannel, blueChannel));  // Draw filled rectangle with
        g.fillRect(BORDER, BORDER, rectWidth, rectHeight);  		   // user-input color channels
		
		g.setColor(new Color(120, 50, 200));			   // Draw rectangle such that it creates a  
		g.drawRect(BORDER, BORDER, rectWidth, rectHeight); // border around the filled rectangle
		
		g.setColor(new Color(255-redChannel, 255-greenChannel, 255-blueChannel));	
		g.drawString("Brian Golden",width-width/2, height-height/2); // Set color to inverse of 
																	 // user-input color and print 
    																 // text inside the rectangle
	}
	private int getChannel(String colorName)
	// PRE: colorName.equals("red") || colorName.equals("green") || colorName.equals("blue")
	// POST: FCTVAL == number value of an RGB channel given by colorName
	{
		int result;		// user-input channel value (0-255 scale)
		
		try
		{
			do		// get color value, continue while entered value is outside of valid range [0, 255]
			{
				// display dialog box and get user-input color number value
				result = Integer.parseInt(JOptionPane.showInputDialog("Enter " 
							+ colorName + " channel (0-255):"));
			}
			while (result < 0 || result > 255);
		}
		catch(NumberFormatException nfe)		// Catch exception if user enters 
		{										// something other than a number
			JOptionPane.showMessageDialog(null, "Entered invalid number format");
			return getChannel(colorName);		// call getChannel() method again
		}
		
		return result;
	}
}