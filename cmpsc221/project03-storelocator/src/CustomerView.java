// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Customer Applet - Customer View Panel
// Description: Parent panel to hold the other panels and generally display a panel on the UI

import javax.swing.*;

public class CustomerView extends JPanel
{
    private SelectionPanel selectionPanel;      // SelectionPanel for selection of  
                                                //  search method and store list
    private StatePanel statePanel;              // StatePanel for store search by state
    private ZipPanel zipPanel;                  // ZipPanel for store search by ZIP code
    
    public CustomerView()
    // POST: a default CustomerView object has been created with selectionPanel, statePanel and zipPanel
    //        set to their new corresponding instances
    {
        selectionPanel = new SelectionPanel(this);
        statePanel = new StatePanel(this);
        zipPanel = new ZipPanel(this);
        
        add(selectionPanel);                    // add selectionPanel to this CustomerView
    }
    
    public void switchToSelection()
    // POST: selectionPanel has been added to this CustomerView, 
    //        statePanel and zipPanel have been removed
    {
        remove(statePanel);                     // remove statePanel from this CustomerView
        remove(zipPanel);                       // remove zipPanel from this CustomerView
        
        selectionPanel.AddTable();
        add(selectionPanel);                    // add selectionPanel to this CustomerView
        
        revalidate();                           // validate after adding and removing GUI components
    }
    
    public void switchToState()
    // POST: statePanel has been added to this CustomerView, 
    //        selectionPanel and zipPanel have been removed
    {
        remove(selectionPanel);                 // remove selectionPanel from this CustomerView
        remove(zipPanel);                       // remove zipPanel from this CustomerView
        add(statePanel);                        // add statePanel to this CustomerView
        
        revalidate();                           // validate after adding and removing GUI components
    }
    
    public void switchToZip()
    // POST: zipPanel has been added to this CustomerView, 
    //        statePanel and selectionPanel have been removed
    {
        remove(statePanel);                     // remove statePanel from this CustomerView
        remove(selectionPanel);                 // remove selectionPanel from this CustomerView
        add(zipPanel);                          // add zipPanel to this CustomerView
        
        revalidate();                           // validate after adding and removing GUI components
    }
    
    public void setColumnWidths(int width)
    // PRE:  width > 0, in pixels
    // POST: the widths of the columns of table in selectionPanel 
    //        have been adjusted according to width
    {
        selectionPanel.setColumnWidths(width);
    }
    
    public void SetValueAt(Object aValue, int row, int column)
    // PRE:  aValue is initialized, row >= 0, column >= 0
    // POST: the value of cell at row 'row' and column 'column'  
    //        in table in selectionPanel has been set to aValue
    {
        selectionPanel.SetValueAt(aValue, row, column);
    }
}