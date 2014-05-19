// Name:        Brian Golden
// Section:     001
// Date:		08/30/11
// Program:     Lab 02: Random Numbers
// Description: Generates normally-distributed numbers and sorts them

import java.util.Arrays;

class Transform
{
	public static final int MAX_SIZE = 20;			// amount of random values to compute

	public static void main (String[] args)
	{
		double[] myArray = new double[MAX_SIZE];	// Normally-distributed random numbers
		
		//PROCESS
		
		// PURPOSE: Generate normally-distrubuted random numbers to fill myArray, 
		//          amount of values == MAX_SIZE
		// REASONING: Loop is determinate because it may only generate numbers equal to physical size of
		//            myArray; indices important to enter values into the array at appropriate locations
		for (int i = 0; i < MAX_SIZE-1; i+=2)
		{
			boxMullerTransform(myArray, i);
		}
		
		// OUTPUT
		System.out.print("Normally-distributed numbers: \n");
		// PURPOSE:  Print all elements in the array
		// REASONING: Loop is determinate because it will only run until the end of the myArray,
		//            size known because the array was fully populated in the previous loop; 
		//            indices are unimportant because physical and logical size of myArray are equal 
		// 			  and all elements will be printed. Enhanced for is best
		for (double num : myArray)
		{
			System.out.println(num);
		}
		
		// PROCESS (2)
		Arrays.sort(myArray);						// Sorts elements in ascending order
		
		// OUTPUT (2)
		System.out.print("\n\nSorted normally-distributed numbers: \n");
		// PURPOSE:  Print all elements in the array in ascending order
		// REASONING: Loop is determinate because it will only run until the end of the myArray,
		//            size known because the array was fully populated in the previous loop; 
		//            indices are unimportant because physical and logical size of myArray are equal 
		// 			  and all elements will be printed. Enhanced for is best
		for (double num : myArray
		{
			System.out.println(num);	
		}
	}
	
	public static void boxMullerTransform(double[] myArray, int index)
	// PRE:  index < myArray.length-1 && index >= 0 
	// POST: The numbers at index and index+1 in myArray will contain standard 
	//       normally-distributed random numbers
	{
		// Box-Muller transform reference: (http://mathworld.wolfram.com/Box-MullerTransformation.html)
		double u1;					    	// First uniformly-distributed number for computations
		double u2;							// Second uniformly-distributed number
		double r;				    		// Value used in Box-Muller transform [r^2 = -2*ln(u1)]
		double theta;						// Other transform value (theta = 2*PI*u2)
		double z1;					    	// First normally-distributed number to be generated
		double z2;							// Second normally-distributed number to be generated
		
		u1 = Math.random();				// Generate two uniformly-distributed numbers between 0 and 1
		u2 = Math.random();
		
		r = Math.sqrt(-2 * Math.log(u1));
		theta = 2 * Math.PI * u2;
		
		z1 = r * Math.cos(theta);		   // Compute the normally-distributed numbers based on the Box-
		z2 = r * Math.sin(theta);		   //  Muller transform algorithm
		
		myArray[index] = z1;			   // Enter the two numbers into myArray
		myArray[index+1] = z2;
	}

}