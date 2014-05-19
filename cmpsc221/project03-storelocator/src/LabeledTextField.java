// Name:        Mike DeCristofaro, Brian Golden, Zach Sloane
// Section:     001
// Date:        12/8/11
// Program:     LabledTextField Class
// Description: LabeledTextField is a subclass of JPanel that holds a JLabel next to a JTextField.
//              LabeledTextField is the LabeledTextField code from Problem 3 on the Fall 2011 CMPSC 221
//              exam. This code was downloaded from the course website solutions sections.
//              http://php.scripts.psu.edu/djh300/cmpsc221/quiz_exam.php
//              Edited by Mike DeCristofaro for use in Project 3

import java.awt.FlowLayout;
import javax.swing.*;

public class LabeledTextField extends JPanel
{
    private JTextField field;                   // text field
    
    public LabeledTextField(String prompt, int fieldWidth)
    // PRE: prompt is initialized, fieldWidth >= 0, in monospaced characters
    // POST: A LabeledTextField object is constructed with a label containing prompt preceding
    //        a text field of fieldWidth characters
    {
        setLayout(new FlowLayout());            // set the layout
        
        add(new JLabel(prompt));                // initialize and add the prompt JLabel infront of field
        
        field = new JTextField(fieldWidth);     // initialize field to fieldWidth columns
        
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