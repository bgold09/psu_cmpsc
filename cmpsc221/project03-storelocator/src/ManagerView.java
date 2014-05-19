// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Manager Applet - ManagerView class
// Description: Panel that contains different views of ManagerApplet

import javax.swing.JPanel;

public class ManagerView extends JPanel
{
    private ManagerOnlineView online;           // JPanel showing the online view for the manager applet
    private ManagerLogonView logon;             // JPanel showing the logon view for the manager applet
    
    public ManagerView()
    // POST: A default ManagerView object is created with the logon and online view panels 
    //        and then adds the logon panel to the ManagerView
    {
       online = new ManagerOnlineView(this);    // initializes the online ManagerOnlineView panel
       logon = new ManagerLogonView(this);      // initializes the logon ManagerLogonView panel
       
       add(logon);                              // adds the logon panel to the GUI
    }
    
    public void viewOnline()
    // POST: removes the logon panel and adds the online panel then repaints the GUI
    {
        remove(logon);                          // removes the logon panel
        add(online);                            // adds the online panel
         
        revalidate();                           // validates the removal and adding of the panels
    }
    
    public void viewLogon()
    // POST: removes the online panel and adds the logon panel then repaints the GUI
    {
        remove(online);                         // removes the online panel
        add(logon);                             // adds the logon panel
        
        revalidate();                           // validates the removal and adding of the panels
    }
}