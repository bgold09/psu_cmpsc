// Name:        Brian Golden
// Section:     001
// Date:        09/27/11
// Program:     Lab 09: Exception Handling
// Description: Takes in a .txt file containing some numbers, handles possible exceptions
//				Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/Lab9.java
//				Modified to add try...catch structure for exception handling

import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException; 

public class Lab9
{
   public static final int MAX_SIZE = 5;         // Max number of data points to store

   public static void main(String[] args)
   {
      double[] data;                      // Numbers read from file
	  int logSize;                        // Logical size of data  
	  Scanner input;                      // Object for reading from file
      
	  data = new double[MAX_SIZE]; 
	  logSize = 0;
	  
      try
	  {
		  input = new Scanner(new File("input.txt"));

		  while(input.hasNext())                   // Read from the file
		  {
			  data[logSize] = input.nextDouble(); 

			  System.out.println(data[logSize]);
			  logSize++; 
		  }
      }
	  catch(FileNotFoundException fnfe)
	  {
		  System.out.println("File 'input.txt' was not found.");
	  }
	  catch (InputMismatchException ime)
	  {
	      System.out.println("'input.txt' must contain numbers only.");
	  }
	  catch(ArrayIndexOutOfBoundsException aioobe)
	  {
		  System.out.println("Data array out of bounds.");
	      System.out.println("Please enter between 0 and " + (data.length-1) + " numbers.");
	  }
	  catch (Exception e)
	  {
	      System.out.println("Unknown error occurred.");
	  }
   }   
}