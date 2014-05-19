// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Customer View Applet - ZIP code Panel
// Description: Panel that contains interface for searching for stores 
//              within a range around given ZIP code; ZipPanel objects have an "OK"  
//              and "Back" button, ZIP code and radius text field and its parent CustomerView

import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class ZipPanel extends JPanel implements ActionListener
{
    private static final double EARTH_RADIUS = 3956.55;     // radius of the Earth, in miles
    private static final int NUM_ROWS = 20;                 // number of rows for table
    private static final int NUM_COLS = 7;                  // number of columns for table
    
    private CustomerView parent;		// CustomerView containing this ZipPanel
    private JButton zipOkButton;              	// button initiating the store search by ZIP code
    private JButton zipBackButton;            	// button moving user to previous screen from zip
    private JTextField zipField;              	// field allowing input of ZIP code by the user
    private JTextField radiusField;           	// field allowing radius input by the user in miles
	
    public ZipPanel(CustomerView cv)
    // PRE:  cv is initialized
    // POST: a ZipPanel object has been created to provide an interface 
    //        for a user to enter a ZIP code and radius to search by
    {
        try
        {
            Class.forName                                   // load the client driver
                  ("org.apache.derby.jdbc.ClientDriver").newInstance();
        }
        catch(ClassNotFoundException cnfe)
        {
            JOptionPane.showMessageDialog
                    (null, "Error loading database driver.");
        }
        catch(Exception e)
	{
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }        
        
        parent = cv;
        
        zipOkButton = new JButton("OK");                            // initialize GUI components
        zipBackButton = new JButton("Back");
        zipField = new JTextField(10);
        radiusField = new JTextField(10);

        zipOkButton.addActionListener(this);                        // add listeners to GUI components
        zipBackButton.addActionListener(this);
        
        add(new JLabel("ZIP Code: "));
        add(zipField);
        add(new JLabel("Radius (in miles): "));
        add(radiusField);
        add(zipOkButton);
        add(zipBackButton);
    }
    
    @Override
    public void actionPerformed(ActionEvent e)              // handle actions on zipOkButton 
    {                                                       //  and zipBackButton
        
        String zipFieldText;                                // text of zipField
        double radius;                                      // user-chosen radius around chosen 
                                                            //  ZIP code, in miles
        zipFieldText = new String();
        radius = 0;

        if(e.getSource() == zipOkButton)                    // if zipOkButton was pressed 
        {
            if(zipField.getText().isEmpty())                // if zipField is empty
            {
                JOptionPane.showMessageDialog
                        (null, "Please enter a ZIP code.");
            }
            
            if(radiusField.getText().isEmpty())             // if radiusField is empty
            {
                JOptionPane.showMessageDialog
                        (null, "Please enter a radius.");
            }
            
            if(!(zipField.getText().isEmpty())              // if both fields are not empty
               && !(radiusField.getText().isEmpty()))
            {
                zipFieldText = zipField.getText();
                
                if (isValidRadius(radiusField.getText())    // if user-input radius and 
                    && isValidZIP(zipFieldText))            //  ZIP code are both valid
                {
                    radius = Double.parseDouble
                                (radiusField.getText());

                    clearTable();
                    
                    getStoresInRange(zipFieldText, radius);

                    parent.switchToSelection();
                }
            }
        }

        if(e.getSource() == zipBackButton)                  // if zipBackButton was pressed
        {
            parent.switchToSelection();
        }

        repaint();
    }
    
    private void getStoresInRange(String zipFieldText, double radius)
    // PRE:  zipFieldText is a valid 5-digit ZIP code; radius >= 0, in miles
    // POST: all stores within given radius around zipFieldText ZIP code are added to table;
    //       distances calculated using great circle computations
    {
        // great circles reference: <http://mathworld.wolfram.com/GreatCircle.html>
        
        Connection myConnection;                        // connection to store_locator database
        Statement stmt;                                 // SQL statement to retrieve records
        ResultSet stores;                               // all records in store table
        ResultSet chosenZIP;                            // record of ZIP code chosen by user
        double chosenZIPLat;                            // latitude of user-chosen ZIP code, in degrees
        double chosenZIPLong;                           // longitude of user-chosen ZIP code, in degrees
        int row;                                        // current row in table
        String query;                                   // text of SQL query to run
        String distanceQuery;                           // subquery to compute distances
        
        chosenZIPLat = 0;
        chosenZIPLong = 0;
        row = 0;
        query = new String();
        distanceQuery = new String();
        
        try
        {
            myConnection = DriverManager.getConnection              // connect to the database
                           ("jdbc:derby://localhost:1527/store_locator", "root", "root");
            stmt = myConnection.createStatement();

            query = "select latitude, longitude from ZIP_code "     // get record of given ZIP code
                    + "where ZIP_code = '" + zipFieldText + "'";
            
            chosenZIP = stmt.executeQuery(query);
            
            while(chosenZIP.next())                                 // get latitude and longitude of
            {                                                       //  chosen ZIP code
                chosenZIPLat = chosenZIP.getDouble("latitude");     // get latitude, convert to radians
                chosenZIPLong = chosenZIP.getDouble("longitude");   // get longitude, convert to radians               
            }
            
            stmt.close();                                           // close open objects
            chosenZIP.close();
            
            distanceQuery = String.format
                ("%f * acos(cos(%f * PI()/180) * cos(z.LATITUDE * PI()/180) * "
                 + "cos((%f - z.longitude)*PI()/180) + sin(%f * PI()/180) * "
                 + "sin(z.latitude * PI()/180))", 
                 EARTH_RADIUS, chosenZIPLat, chosenZIPLong, chosenZIPLat);
            
            query = String.format
                        ("select s.store_ID, s.type, s.STREET_ADDRESS, "
                        + "z.CITY, z.STATE, z.ZIP_CODE, z.LATITUDE, z.LONGITUDE, "
                        + "%s distance "
                        + "from store s inner join ZIP_code z on s.ZIP_code = z.ZIP_code "
                        + "where %s <= %f "
                        + "order by distance", 
                        distanceQuery, distanceQuery, radius);
            
            stmt = myConnection.createStatement();
            stores = stmt.executeQuery(query);
            
            while(stores.next())                                    // iterate through stores
            {
                parent.SetValueAt(stores.getInt("store_id"), row, 0);
                parent.SetValueAt(stores.getString("type"), row, 1);
                parent.SetValueAt(stores.getString("street_address"), row, 2);
                parent.SetValueAt(stores.getString("city"), row, 3);
                parent.SetValueAt(stores.getString("state"), row, 4);
                parent.SetValueAt(stores.getString("ZIP_code"), row, 5);
                parent.SetValueAt(stores.getString("distance"), row, 6);
                
                row++;
            }
            
            stores.close();                                         // close open objects
            stmt.close();
            myConnection.close();
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to or "
                     + "communicating with database.");
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }
    }
    
    private boolean isValidZIP(String zipText)
    // PRE:  zipText is initialized
    // POST: returns true if zipText is a valid 5-digit ZIP code
    {
        Connection myConnection;            // connection to store_locator database
        Statement stmt;                     // SQL statement to retrieve records
        ResultSet results;                  // all ZIP codes that have the same first digit as zipText
        String curZIP;                      // ZIP code of current record in results
        
        curZIP = new String();   
        
        try
        {
            myConnection = DriverManager.getConnection              // connect to the database
                           ("jdbc:derby://localhost:1527/"
                            + "store_locator", "root", "root");
            
            stmt = myConnection.createStatement();
            
            results = stmt.executeQuery("select * from ZIP_code "   // get all ZIP codes that have same
                                        + "where ZIP_code like '"   //  first digit as zipText to get 
                                        + zipText.substring(0, 1)   //  smaller group of ZIP codes
                                        + "%'");                    //  to check
            
            while(results.next())                                   // iterate through results
            {
                curZIP = results.getString("ZIP_code");
                
                if(zipText.equals(curZIP))                          // if zipText matches a record
                {
                    return true;
                }
            }
            
            stmt.close();
            results.close();
            myConnection.close();
            
            JOptionPane.showMessageDialog
                    (null, zipText + " is an invalid ZIP code.");
        }
        catch (SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to or "
                     + "communicating with database.");
        }
        catch (Exception e)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }
        
        return false;
    }
    
    private boolean isValidRadius(String radiusText)
    // PRE:  radiusText is initialized
    // POST: returns true if radiusText is a valid radius format
    {        
        try
        {            
            if(Double.parseDouble(radiusText) > 0)                  // if radius is positive
            {
                return true;
            }
            else                                                    // otherwise radius is invalid value
            {
                JOptionPane.showMessageDialog
                        (null, "Radius must be a positive value.");
                
                return false;
            }
        }
        catch(NumberFormatException nfe)                            // Catch exception if user enters 
        {                                                           // something other than a number
            JOptionPane.showMessageDialog
                    (null, "Entered invalid number "
                     + "format for radius");
            
            return false;
        }
    }
    
    private void clearTable()
    // POST: contents of table have been cleared
    {
        for (int i = 0; i < NUM_ROWS; i++)                  // go through all rows
        {
            for (int j = 0; j < NUM_COLS; j++)              // go through all columns
            {
                parent.SetValueAt("", i, j);                // set cell to blank text
            }
        }
    }
}