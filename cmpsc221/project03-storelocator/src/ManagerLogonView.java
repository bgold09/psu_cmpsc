// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Manager Applet - ManagerLogonView
// Description: Panel that contains interface for a manager to logon

import java.awt.GridLayout;
import java.awt.event.*;
import java.sql.*;
import javax.swing.*;

public class ManagerLogonView extends JPanel implements ActionListener
{
    private static final String INVALID_CHARS = ";',";
    
    private LabeledTextField userNameField;      // text field to enter the manager_id
    private LabeledPasswordField passwordField;  // password field to enter the manager's password
    private JButton logonButton;                 // button to log on to the ManagerOnlineView
    private ManagerView parent;                  // ManagerView panel that this panel is held in
    private String managerName;                  // name of manager currently logged on
    
    public ManagerLogonView(ManagerView managerView)
    // PRE:  managerVeiw is initialized
    // POST: creates a ManagerLogonView with parent = managerView, and initializes the managerID and 
    //        managerPassword arrays. Also initializes and adds the GUI to the panel
    {
        parent = managerView;                                   // set the parent to ManagerView
        managerName = new String();
        
        super.setLayout(new GridLayout(3,1));                   // set the layout to a 3x1 GridLayout
        
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
        
        userNameField = new LabeledTextField("User:", 20);      // initialize GUI Components
        passwordField = new LabeledPasswordField("Pass:", 20);
        logonButton = new JButton("Log on");
        
        logonButton.addActionListener(this);                    // add listener to the logonButton

        add(userNameField);                                     // add the GUI components to the panel
        add(passwordField);
        add(logonButton);                                           
    }
    
    @Override
    public void actionPerformed(ActionEvent ae)                 // handle actions 
    {                                                           //  on logonButton  
        boolean logonSucceeded;                                 // true if the logon attempt succeeded
        
        logonSucceeded = false;
        
        if ((!userNameField.getText().isEmpty())                // if userNameField and passwordField 
             && (!passwordField.getText().isEmpty()))           //  are not empty
        {
            if (isValidCredentials(userNameField.getText(),     // if provided credentials are valid
                passwordField.getText()))
            {
                JOptionPane.showMessageDialog
                            (null, "Log on Successful. "
                             + "Welcome, " + managerName + ".\n"
                             + "Edit store hours here. "
                             + "Properly formatted time is 'hh:mm' "
                             + "in 24-hour time. \n"
                             + "'00:00' in both fields represents "
                             + "a closed store.");
                
                logonSucceeded = true;
                
                parent.viewOnline();                            // switch to online view
                validate();
            }
        }
        
        if(!logonSucceeded)                                     // if logon attempt failed
        {
            JOptionPane.showMessageDialog
                    (null, "Invalid credentials provided. "
                     + "Log on Failed.");
        }
        
        userNameField.setText("");                              // clear both fields
        passwordField.setText("");
        
        repaint();                                           
    }
    
    public boolean isValidCredentials(String managerIDText, String password)
    // PRE:  managerID and password are initialized
    // POST: returns true if the managerID and password provided match a  
    //        record in the manager_logon table of the store_locator database
    {
        Connection myConnection;                // connection to the database course_db
        Statement stmt;                         // statement of sql code to execute
        ResultSet results;                      // set of all database records retreived
        int managerID;                          // ID number of manager attempting to log on
        
        managerID = 0;
        
        if (containsInvalidCharacters(password))
        {
            JOptionPane.showMessageDialog
                    (null, "Password contains invalid characters.");
            
            return false;
        }
        
        try
        {
            managerID = Integer.parseInt(managerIDText);
            
            myConnection = DriverManager.getConnection                  // connect to the database
                       ("jdbc:derby://localhost:1527/store_locator", 
                        "root", "root");

            stmt = myConnection.createStatement();
            
            results = stmt.executeQuery
                    ("select * from manager_logon "
                     + "where manager_id = " + managerID 
                     + " and password = '" + password + "'");
            
            
            
            while(results.next())                                       // iterate through results
            {
                return true;                                            // if credentials provided 
            }                                                           //  are valid
        }
        catch(NumberFormatException nfe)
        {
            JOptionPane.showMessageDialog
                    (null, "Username is invalid. "
                     + "Usernames must be a number.");
            
            return false;
        }
        catch(SQLException sqle)
        {
            JOptionPane.showMessageDialog
                    (null, "Error connecting to database "
                     + "or communicating with database.");
            
            return false;
        }
        catch(Exception e)
        {
            JOptionPane.showMessageDialog
                    (null, "Something else went wrong.");
            
            return false;
        }
        
        return false;
    }
    
    public boolean containsInvalidCharacters(String text)
    // PRE:  text is initialized
    // POST: returns true if text contains any character in INVALID_CHARS
    {
        for (int i = 0; i < text.length(); i++)                     // check each character in text
        {
            for (int index = 0; index < 3; index++)                 // check against each char in INVALID_CHARS
            {
                if (text.charAt(i) == INVALID_CHARS.charAt(index))  // if match found
                {
                    return true;
                }
            }
        }
        
        return false;
    }
}