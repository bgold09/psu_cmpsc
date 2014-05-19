// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     Trajectory class
// Description: Implementation for a Trajectory class;
//				essentially serves as calculator for trajectory of a projectile;
//				Target objects have a starting angle, velocity and position, current width and height 
//				of the applet it is contained in, and the number of points that are calculated until 
// 				a projectile's ending time

public class Trajectory
{
	private double angle;					// user-input launch angle, in degrees
	private double velocity;				// user-input magnitude of the projectile's velocity,
											//  in meters per second 
	private Point startingPoint;			// initial coordinates of the projectile
	private double width;					// width of the applet, in pixels
	private double height;					// height of the applet, in pixels
	private int numPoints;					// number of points that will be 
											//  calculated until ending time
	
	public Trajectory()
	// POST: A default Trajectory object has been created with angle and velocity set to zero, 
	//		 startingPoint set to the point with coordinates (0,0)
	{
		this(0, 0, new Point(), 0, 0);
	}
	
	public Trajectory(double angle, double velocity, Point startingPoint, double width, double height)
	// PRE: angle is in degrees; velocity > 0 in meters/second; both coordinates of startingPoint >= 0
	// POST: A Trajectory object has been created with angle, velocity, startingPoint, 
	//       width and height set to their corresponding input parameters
	{
		this.angle = angle;
		this.velocity = velocity;
		this.startingPoint = startingPoint;
		this.width = width;
		this.height = height;
		numPoints = 0;
	}
	
	public Point GetPoint(double time)
	// PRE: time >= 0 in seconds
	// POST: returns current position of the projectile based on current time 
	// 		 and physics of the motion of a projectile
	{
		// Physics reference: (http://en.wikipedia.org/wiki/Trajectory_of_a_projectile)
		
		Point result;										// current position of the projectile 
															//  based on inputs
		double angleInRadians;								// input angle in radians
		double velocityX;									// horizontal component of velocity
		double velocityY;									// vertical component of velocity
		double currentX;									// x coordinate of the projectile at time
		double currentY;									// y coordinate of the projectile at time
		
		angleInRadians = 2*Math.PI - angle*Math.PI/180;		// convert input angle from deg to radians
		velocityX = velocity*Math.cos(angleInRadians)/		// compute x component of the velocity
				    width*Constants.VELOCITY_ADJUST;  
		velocityY = velocity*Math.sin(angleInRadians)/		// compute x component of the velocity
				    height*Constants.VELOCITY_ADJUST; 
		
		currentX = velocityX*time + 						// compute x coordinate 
				   startingPoint.GetXCoordinate();			//  of the projectile					
		currentY = 0.5*Constants.GRAVITY*time*time +		// compute y coordinate 
				   velocityY*time + 						//  of the projectile
				   startingPoint.GetYCoordinate();	
		
		result =  new Point(currentX, currentY);
		
		return result;
	}
	
	public double GetVerticalVelocity()
	// POST: Returns vertical component of velocity
	{
		double velocityY;									// vertical component of velocity
		double angleInRadians;								// input angle in radians
		
		angleInRadians = 2*Math.PI - angle*Math.PI/180;		// convert input angle from deg to radians
		
		velocityY = velocity*Math.sin(angleInRadians)/		// compute x component of the velocity
			    height*Constants.VELOCITY_ADJUST; 
		
		return velocityY;
	}
	
	public Point GetStartingPoint()
	// POST: FCTVAL == startingPoint
	{
		return startingPoint;
	}
	
	public void SetNumPoints(int numPoints)
	// POST: class member numPoints has been set to input parameter numPoints
	{
		this.numPoints = numPoints;
	}
	
	public int GetNumPoints()
	// POST: FTCVAL == numPoints 
	{
		return numPoints;
	}
}