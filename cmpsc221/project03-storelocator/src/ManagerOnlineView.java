// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Manager Applet - ManagerOnlineView class
// Description: Panel for the interface for a manager to update store times

import java.awt.GridLayout;
import java.awt.event.*;
import java.sql.*;
import javax.swing.*;

public class ManagerOnlineView extends JPanel implements ActionListener, ItemListener
{
    private static final int VALID_LENGTH = 5;      // valid length of a time, in 24-hour format "00:00"
    private static final int MAX_HOURS = 23;        // max allowed value for hours in a time
    private static final int MAX_MINUTES = 59;      // max allowed value for minutes in a time
    private static final String[] DAYS =            // day options for dayDropDown, in "MTWRFSY" format
                         {"M", "T", "W", "R", 
                          "F", "S", "Y"};
    
    private Connection myConnection;                // connection to the database course_db
    private Statement stmt;                         // statement of sql code to execute
    private ResultSet results;                      // set of all records retrieved
    private JButton managerLogoffBtn;               // button for manager to log off the system
    private JComboBox storeDropDown;                // drop down list of all stores in store table
    private JComboBox dayDropDown;                  // drop down list of all days in the week
    private LabeledTextField openField;             // field for opening time of a store
    private LabeledTextField closeField;            // field for closing time of a store
    private JButton saveBtn;                        // button to save updated store hours
    private JPanel first;                           // panel containing managerLogoffBtn
    private JPanel second;                          // panel containing storeDropDown and dayDropDown
    private JPanel third;                           // panel containing openField and closeField
    private JPanel fourth;                          // panel containing saveBtn
    private ManagerView parent;                     // parent ManagerView of this ManagerOnlineView
    
    public ManagerOnlineView(ManagerView managerView)
    // PRE:  managerView is initialized
    // POST: a ManagerOnlineView object has been created with parent set to managerView, 
    //        storeDropDown containing listing of all stores, dayDropDown containing all days of the week
    {
        String[] stores;                                    // all stores in store table of 
                                                            //  store_locator db
        int numStores;                                      // number of stores in store table
        int storeCounter;                                   // counts the index for insertion 
                                                            //  into STORES array
        
        parent = managerView;                               // set the parent to ManagerView
        stores = new String[0];
        numStores = 0;
        storeCounter = 0;                                   // initialize the storeCounter to 0
         
        setLayout(new GridLayout(4,1));                     // set the layout to a 4x1 grid layout
        
        first = new JPanel();                               // initializes the JPanels
        second = new JPanel();
        third = new JPanel();
        fourth = new JPanel();
        
        try
        {
            Class.forName                                       // load the client driver
                  ("org.apache.derby.jdbc.ClientDriver").newInstance(); 
           
           
            myConnection = DriverManager.getConnection          // connect to the database
                           ("jdbc:derby://localhost:1527/"
                            + "store_locator", "root", "root");
            
            stmt = myConnection.createStatement();
            results = stmt.executeQuery
                    ("select count(store_id) num_stores "
                     + "from store"); 
            
            while(results.next())                               // iterate through results
            {
                numStores =  results.getInt("num_stores");
            }
            
            stmt.close();
            results.close();
            
            stores = new String[numStores];
            
            stmt = myConnection.createStatement();
            results = stmt.executeQuery
                        ("select store_id, street_address "
                         + "from store");   
            
            while(results.next())                               // iterate through results
            {
                stores[storeCounter] = 
                        results.getInt("store_id") + ": " 
                        + results.getString("street_address");  
                storeCounter++;
            }
            
            stmt.close();                                       // close open objects  
            results.close();
            myConnection.close();
	}
        catch(ClassNotFoundException cnfe)
        {
            JOptionPane.showMessageDialog
                    (null, "Error loading database driver.");
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to database or "
                        + "communicating with database.");
        }
        catch(Exception e)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }

        managerLogoffBtn = new JButton("Log off");              // initialize the GUI components
        storeDropDown = new JComboBox(stores);
        dayDropDown = new JComboBox(DAYS);
        openField = new LabeledTextField("Open:", 5);
        closeField = new LabeledTextField("Close:", 5);
        saveBtn = new JButton("Save");
         
        managerLogoffBtn.addActionListener(this);               // add listeners to GUI components
        saveBtn.addActionListener(this);
        storeDropDown.addItemListener(this);
        dayDropDown.addItemListener(this);
        
        first.add(managerLogoffBtn);                            // add GUI components to 
        second.add(new JLabel("Store:"));                       //  their respective panels
        second.add(storeDropDown);
        second.add(new JLabel("Day:"));
        second.add(dayDropDown);
        third.add(openField);
        third.add(closeField);
        fourth.add(saveBtn);
        
        add(first);                                             // add panels to ManagerOnlineView panel
        add(second);
        add(third);
        add(fourth);
        
        getStoreTimes();                                        // update the open and close store times  
                                                                //  for the default store and day 
                                                                //  comboBox values
    }

    public void getStoreTimes()
    // POST: the text of openField and closeField have been set to the open and close time, 
    //        respectively, of the currently selected store and day
    {
        try
        {
            myConnection = DriverManager.getConnection                  // connect to the database
                           ("jdbc:derby://localhost:1527/"
                            + "store_locator", "root", "root");
              
            stmt = myConnection.createStatement();
            results = stmt.executeQuery
                        ("select open_time, close_time "
                         + "from store_hours "
                         + "where store_id = " + 
                         ((String)storeDropDown.getSelectedItem()).substring(0,1) 
                         + " and day = '" + (String)dayDropDown.getSelectedItem() 
                         + "'");  
            
            while(results.next())                                       // iterate through results
            {
                openField.setText(results.getString("open_time"));
                closeField.setText(results.getString("close_time"));
            }
            
            stmt.close();                                               // close open objects
            results.close();
            myConnection.close();
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to database or "
                     + "communicating with database.");
        }
        catch(Exception e)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }
    }
    
    public void updateStoreTimes()
    // POST: the store_hours record of the store and day from their corresponding drop down menus
    //        to the new open and close times from their corresponding fields
    {
        try
        {
            myConnection = DriverManager.getConnection                  // connect to the database
                           ("jdbc:derby://localhost:1527/"
                            + "store_locator", "root", "root");
            stmt = myConnection.createStatement();
            
            stmt.execute
                    ("update store_hours set open_time = '" 
                     + openField.getText() + "', close_time = '" 
                     + closeField.getText() + "' where store_id = " 
                     + ((String)storeDropDown.getSelectedItem()).substring(0,1) 
                     + " and day = '" + (String)dayDropDown.getSelectedItem() 
                     + "'");  
            
            stmt.close();                                               // close open objects
            myConnection.close();
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to database "
                     + "or communicating with database.");
        }
        catch(Exception e)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
        }
    }
    
    @Override
    public void actionPerformed(ActionEvent ae)                 // handle actions on saveBtn 
    {                                                           //  and managerLogoffBtn
        if(ae.getSource() == saveBtn)                           // if saveBtn is pressed
        {            
            if (isValidTime(openField.getText())                // if open and close time entered 
                && isValidTime(closeField.getText()))           //  are both valid
            {
                if (isBefore(openField.getText(),               // if open time is earlier than close 
                    closeField.getText())                       //  time or both times are "00:00"
                    || openField.getText().equals("00:00") 
                       && closeField.getText().equals("00:00"))
                {
                    updateStoreTimes();
                    JOptionPane.showMessageDialog
                        (null, "Update Successful");
                }
                else
                {
                    JOptionPane.showMessageDialog
                        (null, "Update failed. Open time "
                         + "must be before close time.");
                }
            }
        }        
        if(ae.getSource() == managerLogoffBtn)                  // if managerLogoffBtn is pressed
        {
            JOptionPane.showMessageDialog
                    (null, "Log out Successful");

            parent.viewLogon();
            
            validate();
        }
    }

    @Override
    public void itemStateChanged(ItemEvent ie)      // handle actions on storeDropDown and dayDropDown
    {
        getStoreTimes();
        
        repaint();
    }
    
    public boolean isBefore(String firstTime, String secondTime)
    // PRE:  firstTime and secondTime are in 24-hour time, in format '00:00'
    // POST: returns true if firstTime is earlier than secondTime
    {
        int firstHours;                                 // hours of firstTime, in 24-hour time
        int firstMinutes;                               // minutes of firstTime
        int secondHours;                                // hours of secondTime, in 24-hour time
        int secondMinutes;                              // minutes of secondTime
        
        firstHours = Integer.parseInt
                        (firstTime.substring(0, 2));
        firstMinutes = Integer.parseInt
                        (firstTime.substring(3, 5));
        secondHours = Integer.parseInt
                        (secondTime.substring(0, 2));
        secondMinutes = Integer.parseInt
                        (secondTime.substring(3, 5));
        
        if (firstHours < secondHours )                  // if the hours of firstTime 
        {                                               //  is less than hours of secondTime
            return true;
        }
        
        if (firstHours == secondHours                   // if hour is the same and firstTime minutes 
            && firstMinutes < secondMinutes)            //  is less than secondTime minutes
        {
            return true;
        }
        
        return false;                                   // otherwise firstTime is not before secondTime
    }
    
    public boolean isValidTime(String time)
    // PRE:  time is initialized
    // POST: returns true if time is a valid time, in 24-hour time
    {
        int hours;                                                  // hours part of time
        int minutes;                                                // minutes part of time
        
        hours = 0;
        minutes = 0;
        
        if (time.length() != VALID_LENGTH)                          // if time is not VALID_LENGTH long
        {
            JOptionPane.showMessageDialog
                    (null, "Invalid time format. Please enter time "
                     + "in 24-hour time, in the format 'hh:mm'");
            
            return false;
        }
        
        if ( !(time.substring(2, 3).equals(":")) )                  // if the third character 
        {                                                           //  is not a colon
            JOptionPane.showMessageDialog
                    (null, "Invalid time format. Please enter time "
                     + "in 24-hour time, in the format 'hh:mm'");
            
            return false;
        }
        
        try
        {
            hours = Integer.parseInt(time.substring(0, 2));
            minutes = Integer.parseInt(time.substring(3));
            
            if ((hours < 0 || hours > MAX_HOURS) ||                 // if hours or minutes are 
                (minutes < 0 || minutes > MAX_MINUTES))             //  outside valid range
            {
                JOptionPane.showMessageDialog
                    (null, "Invalid time format. Please enter time "
                     + "in 24-hour time, in the format 'hh:mm'");
                
                return false;
            }
        }
        catch(NumberFormatException nfe)
        {
            JOptionPane.showMessageDialog
                    (null, "Invalid time format. Please enter time "
                     + "in 24-hour time, in the format 'hh:mm'");
            
            return false;
        }
        
        return true;
    }
}