// Name:        Brian Golden
// Section:     001
// Date:        11/03/11
// Program:     Point class
// Description: Implementation for a Point wrapper class; Point objects have an x and y coordinate

public class Point
{
	private double xCoordinate;			// horizontal position, in units of the Cartesian plane
	private double yCoordinate;			// vertical position, in units of the Cartesian plane
	
	public Point()
	// POST: A default Point object has been created with 
	//       xCoordinate and yCoordinate set to zero
	{
		this(0, 0);
	}
	
	public Point(double xCoordinate, double yCoordinate)
	// PRE: xCoordinate >= 0, yCoordinate >= 0; both in units of the Cartesian plane
	// POST: A Point object has been created with xCoordinate and yCoordinate set to 
	//       their corresponding input parameters
	{
		this.xCoordinate = xCoordinate;
		this.yCoordinate = yCoordinate;
	}
	
	public double GetXCoordinate()
	// POST: FCTVAL == xCoordinate
	{
		return xCoordinate;
	}
	
	public double GetYCoordinate()
	// POST: FCTVAL == yCoordinate
	{
		return yCoordinate;
	}
}