// Name:        Brian Golden
// Section:     001
// Date:        8/23/11
// Program:     Lab 00: Hello World
// Description: Prints two alternating "Hello" messages to the user in dialog boxes

import javax.swing.JOptionPane;

public class Hello
{
    public static void main(String[] args)
    {
		int maxCount = Integer.parseInt(args[0]);	  // Number of messages to print

		for (int i = 0; i <= maxCount; i++)		  // Print maxCount number of messages
		{
			if (i%2 == 0)							  // Even, print "Hello, World!" and current iteration
			{
				JOptionPane.showMessageDialog(null, "Hello, World! (" + i + ")");
			}
			else 								      // Odd, print "Hello, Doug!" and current iteration
			{
				JOptionPane.showMessageDialog(null, "Hello, Doug! (" + i + ")");
			}
		}
    }
}
