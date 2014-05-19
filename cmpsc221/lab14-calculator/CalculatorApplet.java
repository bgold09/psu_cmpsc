// Name:        Brian Golden
// Section:     001
// Date:        10/18/11
// Program:     Lab 14: Calculator Front-End
// Description: Front-end setup for a calculator applet

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Graphics;
import java.awt.event.*;
import javax.swing.*;

public class CalculatorApplet extends JApplet
{
	private static final int NUM_BUTTONS = 16;	// number of JButtons in the applet
	
	private BorderLayout layout;				// layout of the applet
	private JButton[] buttons;					// all buttons that make up the calculator
	private JTextField inputField;				// text field for input from user
	private JPanel buttonPanel;					// panel containing all JButtons in buttons	
	
	@Override
	public void init()		// set up the GUI
	{
		layout = new BorderLayout(0, 20);					// set up layout
		setLayout(layout);
		
		buttons = new JButton[NUM_BUTTONS];					// initialize GUI elements
		inputField = new JTextField(20);
		buttonPanel = new JPanel();
		buttonPanel.setLayout(new GridLayout(4, 4, 5, 5));	// initialize buttonPanel as 4x4 GridLayout
		
		buttons[0] = new JButton("7");						// initialize all JButtons in buttons
		buttons[1] = new JButton("8");
		buttons[2] = new JButton("9");
		buttons[3] = new JButton("/");
		buttons[4] = new JButton("4");
		buttons[5] = new JButton("5");
		buttons[6] = new JButton("6");
		buttons[7] = new JButton("*");
		buttons[8] = new JButton("1");
		buttons[9] = new JButton("2");
		buttons[10] = new JButton("3");
		buttons[11] = new JButton("-");
		buttons[12] = new JButton("0");
		buttons[13] = new JButton(".");
		buttons[14] = new JButton("=");
		buttons[15] = new JButton("+");
		
		for (int i = 0; i < NUM_BUTTONS; i++)			// add all elements of buttons to buttonPanel
		{
			buttonPanel.add(buttons[i]);
		}
		
		add(inputField, BorderLayout.NORTH);			// add elements to the applet
		add(buttonPanel, BorderLayout.CENTER);
	}
}