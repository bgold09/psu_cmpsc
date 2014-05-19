// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Manager Applet
// Description: Main manager interface allowing managers to logon with credentials
//              and update store hours in store_locator database

import java.awt.FlowLayout;
import javax.swing.JApplet;

public class ManagerApplet extends JApplet
{
    private ManagerView view;               // JPanel that holds the different panels 
                                            //  for the manager interface
    
    @Override
    public void init()                      // set up the GUI
    {
        setLayout(new FlowLayout());        // set the layout
        
        view = new ManagerView();           // initialize the JPanel
        
        add(view);                          // add the JPanel to the GUI
    }
}