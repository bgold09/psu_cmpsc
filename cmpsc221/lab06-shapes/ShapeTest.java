// Name:        Brian Golden
// Section:     001
// Date:        09/08/11
// Program:     Lab 06: Shapes
// Description: Shape Test Driver

public class ShapeTest
{
	public static void main(String[] args)
	{
		Shape[] myShapes = new Shape[4]; 
			
		myShapes[0] = new ThreeDimensionalShape(0, 0, 0, "some_3d_shape");
		myShapes[1] = new Sphere(0.5, 1.7, 4.4, 3.5, "soccerball");
		myShapes[2] = new Sphere(3, 4, 5, 0.6, "baseball");
		myShapes[3] = new Cube(1, 2, 3, 9, "cardboard box");
		
		for (Shape temp : myShapes)
		{
			System.out.println(temp);
		}
	}
}