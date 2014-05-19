// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     Game-wide constants
// Description: Values for constants needed for the game to function

import java.awt.Color;

public class Constants
{	
	public static final double GRAVITY = 9.8;				// magnitude of acceleration of gravity
															//   on Earth, in meters per second^2
	public static final double PROJ_X_SCALE = 0.05;			// scaling factor for x coordinate 
															//  of the projectile's starting point
	public static final double PROJ_Y_SCALE = 0.72;			// scaling factor for y coordinate 
															//  of the projectile's starting point
	public static final double SLINGSHOT_X_SCALE = .022;	// scaling factor for horizontal length
															//  of the slingshot 
	public static final double SLINGSHOT_Y_SCALE = .03;		// scaling factor for vertical length
															//  of the slingshot 
	public static final double GROUND_SCALE = .2;			// scaling factor for the ground
	public static final double TIME_INTERVAL = .005;		// time interval over which to compute 
															//  each position of the projectile
	public static final double TARGET_SIZE_SCALE = 0.07;	// scale to resize this Target by
	public static final double TARGET_X_POS = 0.8;			// x position of this Target
															//  before scaling
	public static final double BOUNCE_SCALE = .01;			// scaling factor for a YellowBird's 
															//  position after a bounce
	public static final int VELOCITY_ADJUST = 20;			// adjustment factor for velocity, 
															//  helps to make smaller 
															//  velocities reasonable
	public static final int NUM_TARGETS = 3;				// number of Target objects in the game
	public static final int NUM_PROJECTILES = 4;			// number of different images that may 
															//  represent the projectile
	public static final int GUI_HEIGHT = 100;				// vertical height of the GUI
															//  components, in pixels
	public static final int SCORE_ADD = 5;					// amount to increase player score by
															//  for each time a Target is hit
	public static final int MAX_ANGLE = 360;				// max angle value for angleSlider, 
															//  in degrees
	public static final int DOWN_ANGLE = 270;				// angle pointing downwards, in degrees
	public static final int ANGLE_MJ_TICK = 45;				// major tick mark spacing for angleSlider
	public static final int ANGLE_MN_TICK = 15;				// minor tick mark spacing for angleSlider
	public static final int MAX_VEL = 200;					// max velocity value for velocitySlider, 
															//  in meters/second
	public static final int VEL_MJ_TICK = 50;				// major tick mark spacing 
															//  for velocitySlider
	public static final int VEL_MN_TICK = 10;				// minor tick mark spacing 
															//  for velocitySlider
	public static final int MESSAGE_Y = 15;					// vertical position of score message, 
															//  in pixels
	public static final int FONT_SIZE = 12;					// size of the font for printing the score
	public static final int SLEEP_TIME = 1;					// time to wait before next animation step, 
															//  in milliseconds
	public static final Color[] COLORS = 					// color options for user to choose 
							{Color.BLACK, Color.RED, 		//  from for the projectile and 
							Color.ORANGE, Color.MAGENTA};	//  its trajectory
	public static final String[] COLOR_NAMES = 				// names of color options for user to 
							{"Black", "Red", 				//  choose from for the projectile and 
							"Orange", "Magenta"};			//  its trajectory	
	public static final Color SCORE_COLOR = 				// color to print score in to the applet
						new Color(142, 8, 209);
}