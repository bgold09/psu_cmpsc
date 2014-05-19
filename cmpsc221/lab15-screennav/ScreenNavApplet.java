// Name:        Brian Golden
// Section:     001
// Date:        10/21/11
// Program:     Lab 15: Screen Navigation via Keyboard
// Description: Applet that allows a user to navigate around the screen in any of the 
//				four cardinal directions by 20 pixels at a time; displays a message in the form 
// 				"You are at (xPos, yPos)" at the user's current location

import java.awt.BorderLayout;
import java.awt.Graphics;
import java.awt.event.*;
import javax.swing.*;

public class ScreenNavApplet extends JApplet implements ActionListener, KeyListener
{
	private static final int DISTANCE = 20;		// distance to move the user's location 
												//  per key press, in pixels
	private static final int GUI_HEIGHT = 50;	// vertical space of JButtons in pixels, set such that a
												//  drawn string will not be printed over the buttons
												//  when reseting location to the top-left corner
	
	private int xPos;					// current x coordinate of the user's current position
	private int yPos;					// current y coordinate of the user's current position
	private String message;				// message in the form "You are at (xPos, yPos)," where xPos 
										//  and yPos are coordinates of the user's current location
	private JButton centerBtn;			// button to reset location to center of the screen
	private JButton topLeftBtn;			// button to reset location to top left corner of screen
	
	@Override
	public void init()									// set up the GUI
	{
		JPanel buttonPanel;								// panel to include centerBtn and topLeftBtn
		
		setLayout(new BorderLayout());					// set up layout
		
		centerBtn = new JButton("Reset location");		// initialize GUI components
		topLeftBtn = new JButton("Set to top-left");
		buttonPanel = new JPanel();
		
		xPos = getWidth()/2;							// initialize state variables,
		yPos = getHeight()/2;							//  start user at center of the applet
		message = new String();
		
		buttonPanel.add(centerBtn);						// add buttons to buttonPanel
		buttonPanel.add(topLeftBtn);
		
		add(buttonPanel, BorderLayout.NORTH);			// add buttonPanel to applet
		
		centerBtn.addActionListener(this);				// add listeners to GUI components
		topLeftBtn.addActionListener(this);
		addKeyListener(this);                        	// listen for keyboard events
	}
	
	@Override
	public void paint(Graphics g)						// draw message to the applet
	{
		super.paint(g);
		
		requestFocus();									// force applet to regain focus
		
		message = "You are at (" + xPos + ", " + yPos + ")";
		g.drawString(message, xPos, yPos);
	}
	
	public void actionPerformed(ActionEvent e)	// Handle actions on centerBtn and topLeftBtn
	{
		if (e.getSource() == centerBtn)			// if user chose to reset location to the center
		{
			xPos = getWidth()/2;				// reset user position to the 
			yPos = getHeight()/2;				// center of the applet
		}
		
		if (e.getSource() == topLeftBtn)		// if user chose to reset location to top-left corner
		{
			xPos = 0;							// reset user position to top-left 
			yPos = GUI_HEIGHT;					// corner of the applet
		}
		
		requestFocus();							// force applet to regain focus
		repaint();
	}
	
	public void keyTyped(KeyEvent e)			// handle typing on applet
	{
		switch(e.getKeyChar())
		{
			case 'h':							// move user's current location DISTANCE pixels left
				xPos -= DISTANCE;
				break;
			case 'j':							// move user's current location DISTANCE pixels down
				yPos += DISTANCE;
				break;
			case 'k':
				yPos -= DISTANCE;				// move user's current location DISTANCE pixels up
				break;
			case 'l':
				xPos += DISTANCE;				// move user's current location DISTANCE pixels right
		}
		
		repaint();
    }
	
	// the following methods must have concrete forms and "implementation" because 
	// they are methods of the interface KeyListener, but do not actually need 
	// to be implemented because these events will not be handled
	public void keyPressed(KeyEvent e)	{}
	public void keyReleased(KeyEvent e)	{}
}