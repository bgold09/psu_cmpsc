// Name:        Zach Sloane, Brian Golden, Mike DeCristafaro
// Section:     001
// Date:        12/08/11
// Program:     ZIP code preprocessor
// Description: Parses a comma-seperated list on ZIP codes with their corresponding 
//              city, state, latitude and longitude and adds them to the zip_code table 
//              in the store_locator database

import java.io.File;
import java.io.FileNotFoundException;
import java.sql.*; 
import java.util.Scanner;

public class Preprocessor
{
    private static final String ZIP_CODE_FILE = "zip.txt";      // file name for ZIP codes file
    private static final int START_ZIP_INDEX = 6;               // starting index of ZIP code in a line
    private static final int START_CITY_INDEX = 9;              // starting index of city in a line
    private static final int DIST_BETWEEN_DATA = 3;             // number of characters between 
                                                                //  each piece of data
    
    public static void main(String[] args)
    {       
	Connection myConnection;                // connection to the database course_db
	String testQuery;                       // SQL code to be executed
	Statement stmt;                         // statement of SQL code to execute
	Scanner input;                          // Object for reading from file
        String line;                            // a single line from ZIP_CODE_FILE
        String zip;                             // 5-digit ZIP code from line
        String city;                            // city from line
        String state;                           // state from line
        double latitude;                        // latitude from line, in degrees
        double longitude;                       // longitude from line, in degrees	
	int endCityIndex;                       // index of line where city ends
        int endStateIndex;                      // index of line where state ends
        int endLatIndex;                        // index of line where latitude ends
      
        try
        {
            Class.forName                                                   // load the client driver
                  ("org.apache.derby.jdbc."
                   + "ClientDriver").newInstance(); 
           
           
            myConnection = DriverManager.getConnection                      // connect to the database
                           ("jdbc:derby://localhost:1527/"
                            + "store_locator", "root", "root");
            
            input = new Scanner(new File(ZIP_CODE_FILE));
        
            while(input.hasNext())                                          // Read from the file, while 
            {                                                               //  end of file has not
                                                                            //  been reached
                line = input.nextLine();

                zip = line.substring(1, START_ZIP_INDEX);                   // get ZIP code
                
                endCityIndex = line.indexOf("\"", START_CITY_INDEX);        // compute locations of 
                endStateIndex = line.indexOf("\"", endCityIndex             // data in line
                                                   + DIST_BETWEEN_DATA);
                endLatIndex = line.indexOf("\"", endStateIndex 
                                                 + DIST_BETWEEN_DATA);
                
                city = line.substring(START_CITY_INDEX, endCityIndex);      // get city from line
                state = line.substring(endCityIndex + DIST_BETWEEN_DATA,    // get state from line
                                       endStateIndex);
                latitude =                                                  // get latitiude from line
                    Double.parseDouble(line.substring(endStateIndex 
                                                      + DIST_BETWEEN_DATA, 
                                                      endLatIndex));
                longitude =                                                 // get longitiude from line
                    Double.parseDouble(line.substring(endLatIndex 
                                                      + DIST_BETWEEN_DATA, 
                                                      line.indexOf("\"", 
                                                      endLatIndex 
                                                      + DIST_BETWEEN_DATA)));     
                
                testQuery = String.format                                   // format the input 
                                ("insert into zip_code "                    //  into SQL code
                                 + "(ZIP_code, city, state, latitude, longitude) " 
                                 + "values('%s', '%s', '%s', %f, %f)",
                                 zip, city, state, latitude, longitude);
                
		stmt = myConnection.createStatement();                      // close open objects
                stmt.execute(testQuery);
                stmt.close();
                myConnection.close();
            }
	}
        catch(ClassNotFoundException cnfe)
        {
            System.out.println("Error loading database driver.");
        }
        catch(SQLException sqle)
        {
            System.out.println("Error connecting to databaase or communicating with database.");
        }
        catch(FileNotFoundException fnfe)
        {
            System.out.println(ZIP_CODE_FILE + " not found.");
        }
        catch(Exception e)
	{
            System.out.println("Something else went wrong.");
        }
    } 
}