// Name:        Brian Golden
// Section:     001
// Date:        10/14/11
// Program:     Lab 12: Drawing Applet
// Description: An applet for drawing either a square or message on the screen in several locations, 
//				in color or not

import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.event.*;
import java.awt.Color;
import javax.swing.*;

public class DrawApplet extends JApplet implements ActionListener, ItemListener
{
	private static final int PADDING = 100;		// vertical space taken up by GUI components, in pixels
	private static final int SIDE_LENGTH = 300;	// width of the drawn square, in pixels
	
	private JRadioButton radioSquare;			// user choice to draw a square
	private JRadioButton radioMessage;			// user choice to draw a user-input message
	private JButton drawBtn;					// button to draw either square or message when clicked
	private JTextField messageField;			// field to collect user-input message
	private JCheckBox colorCheckBox;			// user choice to draw in color
	private JComboBox locationDropDown;			// location options for placement of square or message
	private int width;							// current width of the applet, in pixels
	private int height;							// current height of the applet, pixels
	private int xPosSquare;						// x coordinate of top-left corner of square
	private int yPosSquare;						// y coordinate of top-left corner of square
	private int xPosMessage;					// x coordinate of top-left corner of message
	private int yPosMessage;					// y coordinate of top-left corner of message
	private Color curColor;						// color of drawn square or message
	private String message;						// user-input message to draw
	private boolean drawSquare;					// true when user choses to draw a square
	private boolean drawMessage;				// true when user choses to draw message
	
	@Override
	public void init()				// set up the GUI
	{	
		JPanel panel_1;					// panel to include radioSquare, radioMessage, and messageField
		JPanel panel_2;					// panel to include label, drop-down menu, check box and button 
		ButtonGroup drawGroup;			// group to link radio buttons for drawing options
		
		// initialize all GUI components
		radioSquare = new JRadioButton("Draw square");
		radioMessage = new JRadioButton("Write message");
		drawBtn = new JButton("Draw it!");
		messageField = new JTextField(20);
		colorCheckBox = new JCheckBox("Draw in color");
		locationDropDown = new JComboBox(new String[]{"", "Top Left", "Pick random location"});
		drawGroup = new ButtonGroup();
		panel_1 = new JPanel();
		panel_2 = new JPanel();
		
		setLayout(new FlowLayout());
		
		// add listeners to GUI components
		drawBtn.addActionListener(this);	
		radioSquare.addItemListener(this);
		radioMessage.addItemListener(this);
		colorCheckBox.addItemListener(this);
		locationDropDown.addItemListener(this);
		
		panel_1.add(radioSquare);			// add the radio buttons and 
		panel_1.add(radioMessage);			// message field to the first panel
		panel_1.add(messageField);
		
		panel_2.add(new JLabel("Select where to draw: "));	 // add the label, drop-down menu, 
		panel_2.add(locationDropDown);						 // check box and button to the second panel
		panel_2.add(colorCheckBox);
		panel_2.add(drawBtn);
		
		add(panel_1);						// add both panels to the GUI
		add(panel_2);
		
		drawGroup.add(radioSquare);			// link radio buttons together
		drawGroup.add(radioMessage);
	}
	
	public void paint(Graphics g)		// draws square or message in the applet 
	{
		width = getWidth();						// get current width of applet
		height = getHeight();					// get current height of applet
		message = messageField.getText();		// get message text from field
		
		super.paint(g);
		
		g.setColor(curColor);
		if (drawSquare)		// if user has chosen to draw the square
		{
			g.fillRect(xPosSquare, yPosSquare, SIDE_LENGTH, SIDE_LENGTH);
		}
		if (drawMessage)	// if user has chosen to draw a message
		{
			g.drawString(message, xPosMessage, yPosMessage);
		}
	}
	
	public void actionPerformed(ActionEvent e)
	// POST: Actions on drawBtn have been processed
	{
		repaint();
	}
	
	public void itemStateChanged(ItemEvent e)
	// POST: Appropriate actions for radioSquare, radioMessage, colorCheckBox 
	//       and locationDropDown have been processed 
	{
		if (e.getSource() == radioSquare 					// if user chose to draw square
			&& e.getStateChange() == ItemEvent.SELECTED)	
		{
			drawSquare = true;		// draw square but not message
			drawMessage = false;
			
			if(locationDropDown.getSelectedItem().equals("")) // force user to choose from other options
			{
				drawSquare = false;
			}
			else if(locationDropDown.getSelectedItem().equals("Top left"))	// draw square in 
			{																// top-left corner
				xPosSquare = 0;
				yPosSquare = PADDING;
			}
			else if(locationDropDown.getSelectedItem().equals("Pick random location"))// draw square at
			{																		  // random location    
				setRandSquarePos();		// set random coordinates for square
			}
		}
		
		if(e.getSource() == radioMessage 					// if user chose to draw a message
			&& e.getStateChange() == ItemEvent.SELECTED)
		{
			drawSquare = false;		// draw message but not square
			drawMessage = true;
			
			if(locationDropDown.getSelectedItem().equals("")) // force user to choose from other options
			{
				drawMessage = false;
			}
			else if(locationDropDown.getSelectedItem().equals("Top left")) // draw message in 
			{															   // top-left corner
				xPosMessage = 0;
				yPosMessage = PADDING;
			}
			else if(locationDropDown.getSelectedItem().equals("Pick random location"))// draw square at   
			{																		  // random location
				setRandMessagePos();		// set random coordinates for message
			}
		}
		
		if(e.getSource() == colorCheckBox					// if user chose to draw in color
			&& e.getStateChange() == ItemEvent.SELECTED)
		{
			curColor = Color.BLUE;
		}
		
		
		if(e.getSource() == colorCheckBox 					// if user chose to draw in black
			&& e.getStateChange() == ItemEvent.DESELECTED)
		{																			   
			curColor = Color.BLACK;
		}
		
		if(e.getSource() == locationDropDown 					// force user to choose from 
			&& locationDropDown.getSelectedItem().equals(""))	// the two location options
		{
			drawSquare = false;		// don't draw either the square or the message
			drawMessage = false;
		}
		if(e.getSource() == locationDropDown 			// if user chose to draw in the top-left corner
			&& locationDropDown.getSelectedItem().equals("Top Left"))
		{
			if (radioSquare.isSelected())		// if user chose to draw the square
			{
				drawSquare = true;
				xPosSquare = 0;
				yPosSquare = PADDING;
			}
			else if (radioMessage.isSelected())	// if user chose to draw the message
			{
				drawMessage = true;
				xPosMessage = 0;
				yPosMessage = PADDING;
			}
		}
		
		if (e.getSource() == locationDropDown && 		// if user chose to draw in a random location
				locationDropDown.getSelectedItem().equals("Pick random location"))
		{
			if (radioSquare.isSelected())		// if user chose to draw the square
			{
				drawSquare = true;
				setRandSquarePos();		// set random coordinates for square
			}
			else if (radioMessage.isSelected())	// if user chose to draw the message
			{
				drawMessage = true;
				setRandMessagePos();	// set random coordinates for message
			}
		}
	}
	
	private void setRandSquarePos()
	// POST: xPosSquare and yPosSquare are set to random coordinates
	//       inside applet bounds and below GUI components
	{
		xPosSquare = (int)(Math.random()*(width-SIDE_LENGTH));
		yPosSquare = (int)(Math.random()*(height-SIDE_LENGTH)+PADDING);
	}
	
	private void setRandMessagePos()
	// POST: xPosMessage and yPosMessage are set to random coordinates 
	//       inside applet bounds and below GUI components
	{
		xPosMessage = (int)(Math.random()*width);
		yPosMessage = (int)(Math.random()*(height-PADDING)+PADDING);
	}
}