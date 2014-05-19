// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Customer Applet - StatePanel class
// Description: Panel for store search by state

import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class StatePanel extends JPanel implements ActionListener
{
    private static final int NUM_ROWS = 20;     // number of rows for table
    private static final int NUM_COLS = 7;      // number of columns for table
    
    private CustomerView parent;		// CustomerView that contains this StatePanel
    private JButton stateOkButton;            	// button initiating the store search by state
    private JButton stateBackButton;          	// button moving user to previous screen from state
    private JComboBox stateDropDown;          	// dropdown list containing all 50 states to select from

    public StatePanel(CustomerView cv)
    // PRE:  cv is initialized
    // POST: a StatePanel object is created; intializes stateOkButton, 
    //        stateBackButton and stateDropDown and adds them to this StatePanel
    {
        try
        {
            Class.forName                                       // load the client driver
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
        stateOkButton = new JButton("OK");                      // initialize GUI components
        stateBackButton = new JButton("Back");
        stateDropDown = new JComboBox(new String[]
                                {"", "AL - Alabama", "AK - Alaska", "AZ - Arizona", "AR - Arkansas", 
                                 "CA - California", "CO - Colorado", "CT - Connecticut", 
                                 "DE - Delaware", "FL - Florida", "GA - Georgia", "HI - Hawaii", 
                                 "ID - Idaho", "IL - Illinois", "IN - Indiana", "IA - Iowa", 
                                 "KS - Kansas",  "KY - Kentucky", "LA - Louisianna", "ME - Maine", 
                                 "MD - Maryland", "MA - Massachusetts", "MI - Michigan",
                                 "MN - Minnesota","MS - Mississippi","MO - Missouri","MT - Montana",
                                 "NE - Nebraska", "NV - Nevada", "NH - New Hampshire", 
                                 "NJ - New Jersey", "NM - New Mexico", "NY - New York", 
                                 "NC - North Carolina", "ND - North Dakota", "OH - Ohio", 
                                 "OK - Oklahoma",  "OR - Oregon", "PA - Pennsylvania", 
                                 "RI - Rhode Island", "SC - South Carolina",  "SD - South Dakota", 
                                 "TN - Tenessee", "TX - Texas", "UT - Utah", "VT - Vermont", 
                                 "VA - Virginia",  "WA - Washington", "WV - West Virginia", 
                                 "WI - Wisconsin", "WY - Wyoming"});

        stateOkButton.addActionListener(this);                  // add listeners to GUI components
        stateBackButton.addActionListener(this);

        add(new JLabel("State: "));
        add(stateDropDown);
        add(stateOkButton);
        add(stateBackButton);
    }
	
    @Override
    public void actionPerformed(ActionEvent e)			// handle actions on stateOkButton 
    {                                                           //  and stateBackButton
        
        String stateText;                                       // abbreviation of chosen state 
                                                                // from stateDropDown
        
        stateText = new String();

        if(e.getSource() == stateOkButton)                      // if stateOkButton was pressed
        {
            
            if (stateDropDown.getSelectedIndex() == 0)          // if no state was selected
            {
                JOptionPane.showMessageDialog
                        (null, "Please select a state.");
            }
            else                                                // otherwise a state was selected
            {
                stateText = (String)stateDropDown.getSelectedItem();
                stateText = stateText.substring(0, 2);
            
                clearTable();
            
                getStoresInState(stateText);
            
                parent.switchToSelection();
            }
        }
        if(e.getSource() == stateBackButton)                    // if stateBackButton was pressed
        {
            parent.switchToSelection();
        }
        
        repaint();
    }
    
    private void getStoresInState(String stateText)
    // PRE:  stateText is a valid 2-character USA state abbreviation
    // POST: all stores in state stateText have been added to table
    {
        Connection myConnection;                // connection to store_locator database
        Statement stmt;                         // SQL statement to retrieve records
        ResultSet results;                      // set of all records with state matching stateText
        int row;                                // index of current row in table
        
        row = 0;
        
        try
        {
            myConnection = DriverManager.getConnection      // connect to the database
                           ("jdbc:derby://localhost:1527/store_locator", "root", "root");
            
            stmt = myConnection.createStatement();
            results = 
                stmt.executeQuery("select s.store_id, s.type, s.street_address, "
                                  + "z.city, z.state, z.ZIP_code from store s "
                                  + "inner join ZIP_code z on s.ZIP_code = z.ZIP_CODE "
                                  + "where s.ZIP_code in " 
                                  + "(select ZIP_code from ZIP_code where state = '" 
                                  + stateText + "')");            

            while(results.next())                                           // iterate through results
            {
                parent.SetValueAt(results.getInt("store_id"), row, 0);
                parent.SetValueAt(results.getString("type"), row, 1);
                parent.SetValueAt(results.getString("street_address"), row, 2);
                parent.SetValueAt(results.getString("city"), row, 3);
                parent.SetValueAt(results.getString("state"), row, 4);
                parent.SetValueAt(results.getString("ZIP_code"), row, 5);

                row++;
            }
            
            stmt.close();                                                   // close open objects
            results.close();
            myConnection.close();
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog(null, "Error connecting to or "
                               + "communicating with database.");
        }
        catch(Exception ex)
        {
            JOptionPane.showMessageDialog(null, "Something else went wrong.");
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