// Name:        Brian Golden
// Section:     001
// Date:		08/25/11
// Program:     Lab 01: Scaling Input Data
// Description: Takes user-input numbers and scales them according to the largest magnitude.

import java.util.Scanner;

public class Scale
{
	public static void main (String[] args)
	{ 
		double[] inputData = new double[50];	// User-input data
		double maxMagnitude;					// Largest value in input data
		int arraySize;							// Logical size of inputData
		double[] outputData;					// Scaled values of inputData according to max
												//  magnitude; outputData[i] == inputData[i]/maxMagnitude,
												//  for any i
		int count;								// Current index of outputData
		String inputLabel = new String();		// Label of the input data
		Scanner input = new Scanner(System.in); // Input reading object
		
		// INPUT
		System.out.print("Type a description for your data and hit Enter: ");
		inputLabel = input.nextLine();
		
		System.out.println("Enter data points, separated by whitespace. Type Ctrl+D to end input.");
		
		arraySize = 0;
		// PURPOSE: Get user-input data, enter into inputData
		// REASONING: Loop is indeterminate because amount of input numbers is determined by user,
		//		      at least one iteration is not guaranteed because user may choose to input no data
		while( input.hasNextDouble() && arraySize < 50 )  // User is still entering data and size < 50
		{
			 inputData[arraySize] = input.nextDouble(); 
			 arraySize++;
		}
		
		// PROCESS
		maxMagnitude = inputData[0];   // Initialize to first data point to have something to compare to
		// PURPOSE: Check data to find max magnitude
		// REASONING: Loop is determinate because it should run to the logical size of inputData, 
		// 			  logical size determined by number of iterations the previous loop went through; 
		//			  indices important because array is not necessarily completely filled with data
		for (int i = 1; i < arraySize; i++)
		{
			if (Math.abs(inputData[i]) > maxMagnitude)
				maxMagnitude = inputData[i];		// If |current data point| > max, set as max		
		}
		
		outputData = new double[arraySize]; 	// array only needs to be logical size of inputData
		// PURPOSE: Scale data and enter into outputData
		// REASONING: Loop is determinate because it is dependent on logical size of inputData, 
		// 			  logical size determined by number of iterations the first loop went through;
		//            indices important to work with parallel arrays properly
		for (int j = 0; j < arraySize; j++)
		{
			if (maxMagnitude != 0)								// Don't divide if maxMagnitude == 0
				outputData[j] = Math.abs(inputData[j])/maxMagnitude;// Scale data according to max value
		}
		
		// OUTPUT
		count = 0;			// Start with first data point
		System.out.println("Scaled form of '" + inputLabel + "':");
		// PURPOSE: Print scaled data
		// REASONING: Loop is determinate because it runs through all points in outputData;
		//			  indices unimportant because physical and logical size of outputData are equal
		for (double num : outputData)
		{
			System.out.printf("Point " + count + ": %.2f\n", num);	// Print with two decimal places
			count++;											  // Move on to next point in outputData
		}
	}
}