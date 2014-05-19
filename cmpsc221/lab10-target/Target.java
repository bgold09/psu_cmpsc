// Name:        Brian Golden
// Section:     001
// Date:        10/04/11
// Program:     Lab 10: Target
// Description: Displays a circular bullseye centered over the window
//              with alternating random colors for each equally-spaced ring

import javax.swing.JFrame;
import java.awt.Graphics;
import java.awt.Color;
import javax.swing.JPanel;

public class Target extends JPanel
{
	private static final int NUM_RINGS = 15;		// Number of rings to draw in the window
		
	public Target()
	// POST: A default Target object is created with background color set to white
	{
		super();
		setBackground(Color.WHITE);
	}
	
	public void paintComponent(Graphics g)
	{
		int width = getWidth();		  // width of this Target
		int height = getHeight();	  // height of this Target
		int xCenterPos = width/2;	  // x coordinate of the center of this Target
		int yCenterPos = height/2;	  // y coordinate of the center of this Target
		int maxSideLength;			  // max length of each side of the largest bounding
									  //  rectangle for biggest oval, length will be same 
									  //  for width and height to draw a circle
		int curSideLength;			  // current length of each side of bounding rect for current circle
		int lengthToDecreaseBy;		  // length to decrease by for side of 
									  //  bounding rectangle of a drawn oval
		Color color1;				  // first random RGB color
		Color color2;				  // second random RGB color
		
		super.paintComponent(g);
		
		// Set both colors to random RGB colors
		color1 = new Color((int)(Math.random()*256),(int)(Math.random()*256), (int)(Math.random()*256));
		color2 = new Color((int)(Math.random()*256),(int)(Math.random()*256), (int)(Math.random()*256));
		
		if (height > width)				// if height > width of the window, set lengthToDecreaseBy to
		{								// width to avoid drawing ovals outside of the window
			maxSideLength = width;
		}
		else							// otherwise set lengthToDecreaseBy to width
		{
			maxSideLength = height;
		}

		lengthToDecreaseBy = maxSideLength/NUM_RINGS;  // set such that each decrease in sideLength will  
													   // be the same and produce equally spaced rings
		
		for (int i = 0; i < NUM_RINGS; i++)			// draw NUM_RINGS rings of alternating colors, 
		{											//  equally spaced apart
			curSideLength = maxSideLength - lengthToDecreaseBy*i; // set curSideLenght such that oval is
																  // drawn underneath the previous one
			if (i % 2 == 0)				// if i is even, set color to color1
				g.setColor(color1);
			else						// if i is odd, set color to color2
				g.setColor(color2);
			
			// draw filled-in oval over the center of the window
			g.fillOval(xCenterPos - curSideLength/2, yCenterPos - curSideLength/2, 
						curSideLength, curSideLength);
		}
		
		g.setColor(Color.BLACK);						// Set color to black and print centered  
		g.drawString("Brian Golden", width/2-37, 30);	// name at the top of the window
	}
	
	public static void main(String[] args)
	{
		Target target = new Target();                            	  // window for drawing
        JFrame application = new JFrame();                            // the program itself
        
        application.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);   // set frame to exit
                                                                      // when it is closed
        application.add(target);           
		
        application.setSize(500, 400);     		// window initially 500 pixels wide, 400 high
        application.setVisible(true);
	}
}