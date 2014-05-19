// Name:        Brian Golden
// Section:     001
// Date:        10/12/11
// Program:     Lab 12: GUI Elements
// Description: Draws several GUI elements to set up the front-end for this drawing applet

import java.awt.FlowLayout;
import java.awt.Graphics;
import javax.swing.*;
import java.awt.Color;

public class ElementsApplet extends JApplet
{
	private JRadioButton radioSquare;		// radio button for user choice to draw a square
	private JRadioButton radioMessage;		// radio button for user choice to draw a user-input message
	private JButton drawBtn;				// button to draw either square or message when clicked
	private JTextField messageField;		// field to collect user-input message
	private JCheckBox colorCheckBox;		// check box to allow user choice to draw in color
	private JComboBox locationDropDown;		// pull-down box for location options for 
											//  placement of the square or message
	
	@Override
	public void init()			// set up the GUI
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
		locationDropDown = new JComboBox(new String[]{"", "Center", "Pick random location"});
		drawGroup = new ButtonGroup();
		panel_1 = new JPanel();
		panel_2 = new JPanel();
		
		setLayout(new FlowLayout());
		
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
}