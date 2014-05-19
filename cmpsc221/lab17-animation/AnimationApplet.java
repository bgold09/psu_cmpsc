// Name:        Brian Golden
// Section:     001
// Date:        10/27/11
// Program:     Lab 17: Animation
// Description: Applet to move image across the screen 50 pixels at a time with a one-second delay 
//				between steps, plays a sound clip and stops the animation when the character reaches 
// 				the right edge of the applet,

import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.*;
import java.awt.Color;
import javax.swing.*;

public class AnimationApplet extends JApplet
{
	private static final int SLEEP_TIME = 1000;		// time to sleep after each animation step, 
													//  in milliseconds
	private static final int DISTANCE = 50;			// distance to move marioImg every 
													//  SLEEP_TIME milliseconds, in pixels
													
	private Image marioImg;							// Image object of Mario
	private JButton animateBtn;						// button to animate Mario
	private int width;								// current width of the applet
	private int height;								// current height of the applet
	private int imgWidth;							// current width of the applet
	private int imgHeight;							// current height of the applet
	private int imgX;								// x position of top-left corner of marioImg
	private int imgY;								// y position of top-left corner of marioImg
	private boolean buttonPressed;					// true when user has chosen to animate marioImg
	private boolean finishedDraw;					// true when marioImg has reached end of animation
	
	public void init()										// set up GUI
	{
		setLayout(new FlowLayout());						// set up layout
		
		marioImg = getImage(getDocumentBase(), 				// initialize marioImg
						"mario.jpg");
		
		animateBtn = new JButton("Animate");				// initialize GUI component
		
		imgX = 0;											// inintialize state variables
		buttonPressed = false;
		finishedDraw = false;
		// width, height, imgWidth, imgHeight and imgY are initialized in paint because 
		//  they need a graphics context to be properly initialized
		
		animateBtn.addActionListener(						// handle actions on animateBtn 
			new ActionListener()							//  via anonymous inner class
			{
				public void actionPerformed(ActionEvent e)	// handle button clicks
				{
					buttonPressed = true;
					repaint();
				}
			}
		);
		
		add(animateBtn);									// add animateBtn to the applet
	}
	
	public void paint(Graphics g)							// animate marioImg
	{	
		super.paint(g);
		
		width = getWidth();									// initialize state variables
		height = getHeight();
		imgWidth = marioImg.getWidth(this);	
		imgHeight = marioImg.getHeight(this);
		imgY = getHeight() - marioImg.getHeight(this);
		
		g.drawImage(marioImg, imgX, imgY, imgWidth, 		// draw marioImg at current position
					imgHeight, this);
					
		if (buttonPressed)									// if user has chosen to animate image
		{
			animateImage(g);
		}
	}
	
	public void animateImage(Graphics g)
	// PRE: g is the current graphics context
	// POST: marioImg has been moved DISTANCE pixels to the right of its current position
	// 		 and sleeps for one second
	{				
		if (imgX + imgWidth + DISTANCE < width)				// if next animation will not put marioImg 
		{													//  off the right side of the screen
			imgX += DISTANCE;
		}
		else if(imgX + imgWidth + DISTANCE >= width)		// if next animation will put marioImg 
															//  off the right side of the screen
		{
			play(getDocumentBase(), "mario1c.wav");			// play audio clip
			buttonPressed = false;
		}
		
		try
		{  
			Thread.sleep(SLEEP_TIME); 						// sleep for SLEEP_TIME ms
		}
		catch(InterruptedException ie) {}
		
		repaint();
	}	
}