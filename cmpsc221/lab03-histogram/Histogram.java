// Name:        Brian Golden
// Section:     001
// Date:        09/02/11
// Program:     Lab 3: Random Numbers & Histogram
// Description: Generates a histogram for normally-distributed random numbers;
//              numbers generated according to Box-Muller transform

import java.util.Arrays;

public class Histogram
{
	public static final int MAX_SIZE = 300;			// Number of random values to compute
	public static final int LOW = -3;				// Lower bound for histogram
	public static final int HIGH = 3;				// Upper bound for histogram
	public static final int CATEGORIES = 24;		// Number of categories for the histogram
	
	public static void generateHistogram(double[] myArray, double low, double high, int categories)
	// PRE: myArray has values for all indices, low < high, categories > 0
	// POST: Displays histogram of values in myArray within the range [low, high), 
	//       # of categories == categories parameter. Histogram is formatted such that the 
	//       lower and upper bounds of each category are displayed first, followed by a '*' character for
	//       each occurrence in the category, followed then by the number of occurrences in parentheses
	//       Ex:  -2.00 to  -1.75: *******(7)
	{
		double interval;			// Interval of each category in the histogram
		double[] categoryBounds;	// Bounds for the categories
		int[] frequencyTable;		// Occurrences for each category, semi-parallel array with 
									// categoryBounds & myArray; for any i, the category at 
									// frequencyTable[i] has lower bound == categoryBounds[i] and 
									// upper bound == categoryBound
		int occurrences;			// Number of data points in the given range in the freq table
		int freqTableIndex;			// Index of frequencyTable
		double intervalNum;			// Numerator of computation to find interval between bounds
		double intervalDenom;		// Denominator of interval computation
		
		// PROCESS
		interval = (high-low) / categories;			// Compute bound intervals
		frequencyTable = new int[categories];		// Size of frequency table == categories
		categoryBounds = new double[categories+1];	// There will be one more bound than categories
		
		// PURPOSE: Determine bounds of the categories
		// REASONING: Loop is determinate because there are a known number of categories to compute; 
		//			  care about indices to set bounds to appropriate indices in categoryBounds
		for (int i = 0; i < categories+1; i++)	
		{
			categoryBounds[i] = low + interval*i;	// Set next category bound
		}
		
		Arrays.fill(frequencyTable, 0);		// Initialize each data point in frequencyTable to zero 
											// so they can be incremented
		// PURPOSE: Find appropriate category for each data point
		// REASONING: Loop is determinate because all data in myArray are to be analyzed  
		//            and myArray has been fully populated by the Arrays.fill() method call; don't care 
		//            about indices because only the data point's value is important, not its index
		for (double num : myArray)
		{
			freqTableIndex = 0;			// Check categories, start at beginning of frequency table
			// PURPOSE: Look for appropriate category for num
			// REASONING: Loop is determinate because there are a known number of categories to check; 
			//			  care about indices in order to properly work with the semi-parallel arrays
			for (int c = 0; c < categories; c++)
			{
				if (num >= categoryBounds[c] && num < categoryBounds[c+1])
				{
					frequencyTable[freqTableIndex]++;	// If within bounds of category, 
				}										// increment occurrences by one
				freqTableIndex++;					// Check next category in the frequency table
			}
		}
		
		// OUTPUT
		freqTableIndex = 0;							// Print each category from the beginning
		System.out.println("\nHISTOGRAM:");
		// PURPOSE: Print each category and # of occurrences
		// REASONING: Loop is determinate because there are a known number of categories, 
		//			  categoryBounds has been fully populated by previous loop;
		//			  care about indices in order to access specific bounds in categoryBounds
		for (int k = 0; k < categoryBounds.length-1; k++)
		{
			System.out.printf("%6.2f to %6.2f: ", categoryBounds[k], categoryBounds[k+1]);
			// PURPOSE: Print '*' for each occurrence
			// REASONING: Loop is determinate because there are a known number of occurrences, 
			//			  value stored in frequencyTable at corresponding index;
			//            indices are important to properly access each data point in frequencyTable
			for (int d = 0; d < frequencyTable[freqTableIndex]; d++)
			{
				System.out.print("*");
			}
			System.out.println("(" + frequencyTable[freqTableIndex] + ")");	// Print # of occurrences
			
			freqTableIndex++;
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
	
	public static void main (String[] args)
	{
		double[] myArray = new double[MAX_SIZE];	   // Normally-distributed random numbers
		double[] sortedArray;                          // Sorted normally-distributed random numbers
		
		//PROCESS
		
		// PURPOSE: Generate normally-distributed random numbers to fill myArray, # values == MAX_SIZE
		// REASONING: Loop is determinate because there MAX_SIZE numbers are to be generated;
		//            care about indices to properly fill myArray
		for (int i = 0; i < MAX_SIZE-1; i+=2)
		{
			boxMullerTransform(myArray, i);
		}
		
		sortedArray = myArray;			// Set sortedArray to all elements in myArray and sort
		Arrays.sort(sortedArray);
		
		// OUTPUT
		System.out.println("\nSorted Data Points:");
		// PURPOSE: Print sorted values
		// REASONING: Loop is determinate because myArray has been filled according to 
		//			  the Box-Muller Transform in the previous loop; don't about indices because only 
		//            the data point is important, not its location
		for (double num : sortedArray)        
		{
			System.out.printf("%.2f\t", num);
		}
		
		generateHistogram(myArray, LOW, HIGH, CATEGORIES);	
	}
}