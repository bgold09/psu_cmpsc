// Name:        Brian Golden
// Section:     001
// Date:        09/08/11
// Program:     Lab 06: Shapes
// Description: Implementation for a sphere class;	Sphere objects have a name, (x, y, z) coordinate for
//				its center of mass and radius

public class Sphere extends ThreeDimensionalShape
{
	private double radius;		// Radius of this sphere
	
	public Sphere(double xCenterPos, double yCenterPos, double zCenterPos, double radius, String name)
	// PRE: xCenterPos, yCenterPos, zCenterPos are all in units of the 3-dimensional Cartesian plane; 
	//		radius > 0, in units of Cartesian plane; Assigned(name)
	// POST: class member xCenterPos has been set to xCenterPos, class member yCenterPos set to 
	//		 yCenterPos, class member zCenterPos has been set to zCenterPos, class member radius set to
	//		 radius and class member name set to name
	{
		super(xCenterPos, yCenterPos, zCenterPos, name);  // Call ThreeDimensionalShape init contructor
		this.radius = radius;
	}
	
	public String toString()
	// POST: FCTVAL == String representation of Sphere object 
	{
		return super.toString() + " with radius = " + radius;
	}
}