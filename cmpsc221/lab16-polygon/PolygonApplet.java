// Name:        Brian Golden
// Section:     001
// Date:        10/25/11
// Program:     Lab 16: Drawing Polygons
// Description: An applet that allows the user to click to draw a polygon, 
// 				choose a color, and fill in that polygon

import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.event.*;
import java.awt.Color;
import javax.swing.*;

public class PolygonApplet extends JApplet implements ActionListener, MouseListener
{
	private static final int MAX_POINTS = 30;	// max number of points to take from user clicks
	
	private JButton polyBtn;					// button for user choice to draw a polygon
	private int[] xVals; 						// x values for points clicked by user, parallel with yVals
	private int[] yVals;						// y values for points clicked by user, parallel with xVals
	private int numPoints;						// number of points in the polyline / polygon
	private Color polyColor;					// fill color of the polygon
	private boolean drawPolygon;				// true when user chooses to draw a polygon, not polyline
	private boolean isDrawn;					// true when polygon has been drawn
	
	@Override
	public void init()										// set up the GUI
	{		
		setLayout(new FlowLayout());						// set up layout
		
		polyBtn = new JButton("Draw polygon");				// initialize GUI components
		
		xVals = new int[MAX_POINTS];						// initialize state variables
		yVals = new int[MAX_POINTS];
		numPoints = 0;
		polyColor = new Color(255, 255, 255);				// set initially to default color of black
		drawPolygon = false;
		isDrawn = false;
		
		polyBtn.addActionListener(this);					// add listeners to GUI components
		addMouseListener(this);                        		// listen for mouse events
		
		add(polyBtn);										// add polyBtn to the applet
		
		JOptionPane.showMessageDialog(null, 				// display dialog box 
			"Click screen to choose points to draw polygon" // with directions for applet
			+ "\nThen click the button to fill the shape", 
			"Welcome", JOptionPane.PLAIN_MESSAGE);
	}
	
	@Override
	public void paint(Graphics g)							// handle drawing the message to the applet
	{
		super.paint(g);
		if (!drawPolygon)									// if user has not chosen to draw a polygon
		{
			g.drawPolyline(xVals, yVals, numPoints);		// draw a polyline with user-clicked points
		}
		else												// otherwise draw a filled polygon
		{
			if (!isDrawn)									// if user has not chosen to draw a polygon	
			{
				isDrawn = true;
			}
			g.setColor(polyColor);
			g.fillPolygon(xVals, yVals, numPoints);			// draw a filled polygon with chosen points
		}
	}
	
	public void actionPerformed(ActionEvent e)				// handle actions on polyBtn
	{
		drawPolygon = true;									// user chose to draw a polygon
		removeMouseListener(this);
		polyColor = JColorChooser.showDialog(this, 			// display color chooser
							"Choose a color", polyColor);
		repaint();
	}
		
	public void mouseClicked(MouseEvent e)  				// handle mouse clicks
	{
		if (numPoints < MAX_POINTS)							// if user has not chosen to draw polygon
		{
			xVals[numPoints] = e.getX();					// get x value of point clicked
			yVals[numPoints] = e.getY();					// get y value of point clicked
			numPoints++;									// polyline now has one more point
			repaint();
		}
	}
	
	// the following methods must have concrete forms and "implementation" because 
	// they are methods of the interface MouseListener, but do not actually need 
	// to be implemented because these events will not be handled
	public void mousePressed(MouseEvent e)  {}
	public void mouseReleased(MouseEvent e) {}
	public void mouseEntered(MouseEvent e)  {}
	public void mouseExited(MouseEvent e)   {}	
}