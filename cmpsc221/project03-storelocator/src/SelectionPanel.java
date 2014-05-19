// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Customer Applet - Selection Panel
// Description: Panel for selection of search method and store listing

import java.awt.FlowLayout;
import java.awt.BorderLayout; 
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.TableColumn;

import java.sql.*;

public class SelectionPanel extends JPanel implements ActionListener
{
    private static final double SM_COL_WIDTH = .07;                  // width of "Store ID," "State"
                                                                     //  and "ZIP Code" columns in
                                                                     //  table, in percentage of
                                                                     //  width of applet
    private static final double MD_COL_WIDTH = .14;                  // width of each column in
                                                                     //  table, in percentage of
                                                                     //  width of the applet
    private static final double LG_COL_WIDTH = .18;                  // width of "Street Address"
                                                                     //  column in table, in
    private static final int STORE_ID_COL = 0;                       // column index of store_id in table
    private static final int STREET_COL = 2;                         // column index of streed_address                                                             // in table
    private static final int STATE_COL = 4;                          // column index of state in table
    private static final int ZIP_COL = 5;                            // column index of ZIP code in table
    private static final int NUM_ROWS = 20;                          // number of rows for table
    private static final int NUM_COLS = 7;                           // number of columns for table
    private static final String[] COLS = {"Store ID", "Store Type",  // column labels for listing 
                                         "Street Address", "City",   //  stores in a JTable
                                         "State", "ZIP code", 
                                         "Distance"};
    
    private static final String[] COLS2 = {"Day", "Open Time", 
                                           "Close_Time"};
    
    private CustomerView parent;		// CustomerView that contains this SelectionPanel
    private JButton stateButton;              	// button initiating state search menu
    private JButton zipButton;                	// button initiating ZIP code search menu
    private JButton hoursButton;                // button to show store hours
    private JPanel buttonPanel;               	// panel for selecting how to search
    private JTable table;                     	// table displaying stores within given area
    private JTable hoursTable;                  // table displaying store hours
    private JPanel tablePanel;                	// panel containing table of store information
    private JPanel hoursPanel;                  // panel containing listing of store hours
    
    public SelectionPanel(CustomerView cv)
    // PRE:  cv is initialized
    // POST: a SelectionPanel object is created; intializes stateButton, 
    //        zipButton, buttonPanel and tablePanel and adds them to this SelectionPanel
    {
        parent = cv;
        
        stateButton = new JButton("State");                     // initialize GUI components
        zipButton = new JButton("ZIP Code");
        hoursButton = new JButton("Display Store Hours");
        buttonPanel = new JPanel(new FlowLayout());
        tablePanel = new JPanel(new BorderLayout());
        hoursPanel = new JPanel(new BorderLayout());
        table = new JTable                                      // create NUM_ROWS by NUM_COLS array
                        (new Object[NUM_ROWS][NUM_COLS], COLS); 
        
        hoursTable = new JTable(new Object[7][3], COLS2);

        table.setFillsViewportHeight(true);                     // table will use entire 
                                                                //  height of tablePanel
        hoursTable.setFillsViewportHeight(true);
        
        table.setSelectionMode
                (ListSelectionModel.SINGLE_SELECTION);

        stateButton.addActionListener(this);                    // add listeners to GUI components
        zipButton.addActionListener(this);
        hoursButton.addActionListener(this);
        
        setLayout(new BorderLayout());

        buttonPanel.add(new JLabel("Search for UPS "
                                    + "Locations by: "));
        buttonPanel.add(stateButton);
        buttonPanel.add(zipButton);
        
        hoursPanel.add(hoursTable.getTableHeader(), BorderLayout.NORTH);
        hoursPanel.add(hoursTable, BorderLayout.CENTER);

        tablePanel.add(table.getTableHeader(), 
                       BorderLayout.NORTH);
        tablePanel.add(table, BorderLayout.CENTER);

        add(buttonPanel, BorderLayout.NORTH);                   // add panels to this SelectionPanel
        
    }
    
    @Override
    public void actionPerformed(ActionEvent e)          // handle actions on stateButton and zipButton
    {        
        if(e.getSource() == stateButton)                // if stateButton was pressed   
        {
            parent.switchToState();                     // switch to state view
        }
        if(e.getSource() == zipButton)                  // if zipButton was pressed
        {
            parent.switchToZip();                       // switch to ZIP code view
        }
        
        if(e.getSource() == hoursButton)                // if hoursButton was pressed
        {
            if(table.getSelectedRow() != -1)            // if a row is selected
            {
                displayStoreHours();
            }
            else                                        // if no row is selected
            {
                JOptionPane.showMessageDialog(null, "Please select a store.");
            }
        }

        repaint();
    }
    
    public void displayStoreHours()
    {
        Connection myConnection;                        // connection to store_locator database
        ResultSet results;                              // set of all results retrieved
        Statement stmt;                                 // SQL statement to retrieve records
        int row;                                        // 
        
        row = 0;
        
        try
        {
            myConnection = DriverManager.getConnection                  // connect to the database
                       ("jdbc:derby://localhost:1527/store_locator", 
                        "root", "root");
            
            stmt = myConnection.createStatement();
            
            results = stmt.executeQuery
                        ("select day, open_time, close_time "
                         + "from store_hours "
                         + "where store_id = " 
                         + table.getValueAt(table.getSelectedRow(), 0));
            
            while(results.next())                                       // iterate through results
            {
                if (results.getString("open_time").equals("00:00")      // if store is closed on cur day
                    && results.getString("close_time").equals("00:00"))
                {
                    hoursTable.setValueAt(results.getString("day"), row, 0);
                    hoursTable.setValueAt("CLOSED", row, 1);
                    hoursTable.setValueAt("CLOSED", row, 2);
                }
                else                                                    // if store is open on cur day
                {
                    hoursTable.setValueAt(results.getString("day"), row, 0);
                    hoursTable.setValueAt(results.getString("open_time"), row, 1);
                    hoursTable.setValueAt(results.getString("close_time"), row, 2);
                }           
                  
                row++;
            }
            
            results.close();                                            // close open objects
            stmt.close();
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
        
        JOptionPane.showMessageDialog(null, hoursPanel, 
                                      "store hours", JOptionPane.PLAIN_MESSAGE);
    }
    
    public void AddTable()
    // POST: 
    {
        JPanel hoursPanel;
        
        hoursPanel = new JPanel(new FlowLayout());
        
        hoursPanel.add(hoursButton);
        
        add(tablePanel, BorderLayout.CENTER);
        add(hoursPanel, BorderLayout.SOUTH);
        
        revalidate();
    }

    public void SetValueAt(Object aValue, int row, int column)
    // PRE:  aValue is initialized, row >=0, column >= 0
    // POST: the value of table at row 'row' and column 'column' has been set to aValue
    {
        table.setValueAt(aValue, row, column);
    }
    
    public void setColumnWidths(int width)
    // PRE:  width > 0, in pixels
    // POST: widths of columns in table have been adjusted according to width
    {
        TableColumn column;                                     // supplies method for setting width
                                                                //  of a column
        
        column = null;

        for (int i = 0; i < NUM_COLS; i++)                      // set width of each column in table
        {
            column = table.getColumnModel().getColumn(i);       // get current column in table
            
            switch(i)
            {
                case STORE_ID_COL:
                case STATE_COL:
                case ZIP_COL:
                    column.setPreferredWidth                    // set width of col by SM_COL_WIDTH
                            ((int)(width*SM_COL_WIDTH));
                    break;
                case STREET_COL:
                    column.setPreferredWidth                    // set width of col by LG_COL_WIDTH
                            ((int)(width*LG_COL_WIDTH));
                    break;
                default:
                    column.setPreferredWidth                    // set width of col by MD_COL_WIDTH
                            ((int)(width*MD_COL_WIDTH));
                    break;
            }
        }
    }
}