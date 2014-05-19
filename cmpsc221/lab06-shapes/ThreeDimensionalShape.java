// Name:        Brian Golden
// Section:     001
// Date:        09/08/11
// Program:     Lab 06: Shapes
// Description: Implementation for a three dimensional shape class;	ThreeDimensionalShape objects have 
//				a name and (x, y, z) coordinate for its center of mass

public class ThreeDimensionalShape extends Shape
{
	protected double xCenterPos;	// x position of the 3D shape's center of mass
	protected double yCenterPos;	// y position of the 3D shape's center of mass
	protected double zCenterPos;	// z position of the 3D shape's center of mass
	
	public ThreeDimensionalShape(double xCenterPos, double yCenterPos, double zCenterPos, String name)
	// PRE: xCenterPos, yCenterPos, zCenterPos are all in units of the 3-dimensional Cartesian plane;
	//      Assigned(name)
	// POST: class member xCenterPos has been set to xCenterPos, class member yCenterPos set to 
	//		 yCenterPos, class member zCenterPos has been set to zCenterPos, and class member name 
	//		 set to name
	{
		super();						// Call Shape constructor
		this.xCenterPos = xCenterPos;
		this.yCenterPos = yCenterPos;
		this.zCenterPos = zCenterPos;
		this.name = name;
	}
	
	public String toString()
	// POST: FCTVAL == String representation of ThreeDimensionalShape object
	{
		return super.toString() + " --> center of mass = (" + xCenterPos + ", " + yCenterPos + ", " + zCenterPos + ")";
	}
}