// Name:        Brian Golden
// Section:     001
// Date:        09/19/11
// Program:     Lab 08: LibraryBook Class
// Description: Implementation for a library book class; LibraryBook objects have a title,  
//              author, call number, availability, and due date..
//              Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/notes-polymorph.php
//              Modified to add DAYS_DUE class member, changed CheckOut() accordingly, added default and
//              init constructor, CheckIn() and toString() methods

public class LibraryBook extends LibraryObject
{
    private static final int DAYS_DUE = 21;         // Number of days from checkout the book is due
    private static final double LATE_FEE = 0.25;    // Late fee per day, in dollars
    
    private String author;                          // author of the book
    
    public LibraryBook()
    // POST: new LibraryBook constructed such that title, call_number and author are set to 
    //       blank strings and isAvail is true
    {
        super();        // Call LibraryObject default constructor
        author = "";
    }
    
    public LibraryBook(String title, String call_number, String author)
    // PRE:  title, call_number and author are initialized
    // POST: new LibraryObject constructed such that title, call_number and author are set to the values 
    //       passed the corresponding parameters and isAvail is true
    {
        super(title, call_number);
        this.author = author;
    }

    @Override
    public void CheckOut()
    // PRE:  class member isAvail == true
    // POST: isAvail is set to false and monthDue and dateDue are reset to DAYS_DUE days in the future
    {
        isAvail = false;
        ResetDueDate(DAYS_DUE);      // book will be checked out for DAYS_DUE days
    }
    
    @Override
    public String CheckIn()
    // PRE:  class member isAvail == false, monthDue, dateDue and title are initialized
    // POST: isAvail is set to true; return string that the book was returned and what the late fee was 
    //       if the book was late
    {
        isAvail = true;
        if (curDate > dateDue)
            return String.format("Book '%s' returned, Late fee: $%.2f", title, (curDate - dateDue)*LATE_FEE);
        else
            return "Book '" +  " returned";
    }
    
    @Override
    public String toString()
    // FTCVAL == string representation of a LibraryObject
    {
        return "Book: '" + title + "' by " + author + "; " + call_number + "; " + GetStatus();
    }
}