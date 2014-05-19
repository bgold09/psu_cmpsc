// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     LabeledPasswordField class
// Description: LabeledPasswordField is a subclass of JPanel that holds a JLabel next to a 
//              JPasswordField. LabeledPasswordField is the LabeledTextField code from Problem 3 on the 
//              Fall 2011 CMPSC 221 exam. This code was downloaded from the course website solutions 
//              sections, and then edited to be a JPasswordField. 
//              http://php.scripts.psu.edu/djh300/cmpsc221/quiz_exam.php
//              Edited from LabeledTextField by Mike DeCristofaro for use on Project 3.

import java.awt.FlowLayout;
import javax.swing.*;

public class LabeledPasswordField extends JPanel
{
    private JPasswordField field;               // password field
    
    public LabeledPasswordField(String prompt, int fieldWidth)
    // PRE:  prompt is initialized, fieldWidth >= 0, in monospaced characters
    // POST: A LabeledPasswordField is constructed with a label containing prompt preceding
    //        a password field of fieldWidth characters
    {
        setLayout(new FlowLayout());            // set the layout
        
        add(new JLabel(prompt));                // initialize and add the prompt JLabel infront of field
        
        field = new JPasswordField(fieldWidth); // initialize field to fieldWidth columns
        
        add(field);                             // add field to the GUI
    }
    
    public String getText()
    // POST: FCTVAL == text in field
    {
        return field.getText();
    }
    
    public void setText(String text)
    // PRE:  text is initialized
    // POST: the text of field has been set to input parameter text
    {
        field.setText(text);
    }
}