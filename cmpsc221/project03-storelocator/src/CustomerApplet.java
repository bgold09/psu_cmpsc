// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     Customer Applet
// Description: Customer interface allowing search for nearby stores by ZIP code or state

import java.awt.FlowLayout;
import java.awt.Graphics;
import javax.swing.*;

public class CustomerApplet extends JApplet
{						
    private CustomerView customerView;              // panel to hold all other panels for UI display
    
    @Override
    public void init()                              // set up GUI
    {
        setLayout(new FlowLayout());

        customerView = new CustomerView();          // initialize customerView
        
        add(customerView);                          // add customerView to the GUI
    }
    
    @Override
    public void paint(Graphics g)                   // set column widths for table
    {
        super.paint(g);
        
        customerView.setColumnWidths(getWidth());
    }
}