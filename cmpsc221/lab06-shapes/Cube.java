// Name:        Brian Golden
// Section:     001
// Date:        09/08/11
// Program:     Lab 06: Shapes
// Description: Implementation for a cube class; Cube objects have a name, (x, y, z) coordinate for its
//				center of mass and length of each edge

public class Cube extends ThreeDimensionalShape
{
	private double sideLength;		// Length of any edge of the cube
	
	public Cube(double xCenterPos, double yCenterPos, double zCenterPos, double sideLength, String name)
	// PRE: xCenterPos, yCenterPos, zCenterPos are all in units of the 3-dimensional Cartesian plane;
	//		sideLength > 0, in units of Cartesian plane; Assigned(name)
	// POST: class member xCenterPos has been set to xCenterPos, class member yCenterPos set to 
	//		 yCenterPos, class member zCenterPos has been set to zCenterPos, class member sideLength set
	//		 to sideLength and class member name set to name
	{
		super(xCenterPos, yCenterPos, zCenterPos, name);  // Call ThreeDimensionalShape init contructor
		this.sideLength = sideLength;
	}
	
	public String toString()
	// POST: FCTVAL == String representation of Cube object
	{
		return super.toString() + " with each side length = " + sideLength;
	}
}