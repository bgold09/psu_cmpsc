// Name:        Brian Golden
// Section:     001
// Date:        09/19/11
// Program:     Lab 08: LibraryDVD Class
// Description: Implementation for a library DVD class; LibraryDVD objects have a title, genre,
//              call number, availability, and due date.
//              Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/notes-polymorph.php
//              Modified to add DAYS_DUE class member, changed CheckOut() accordingly, added default and
//              init constructor, CheckIn() and toString() methods

public class LibraryDVD extends LibraryObject
{
    private static final int DAYS_DUE = 5;          // Number of days from checkout the DVD is due
    private static final double LATE_FEE = 0.75;    // Late fee per day, in dollars
    
    private String genre;                           // type of DVD, e.g., "movie," "educational"
    
    public LibraryDVD()
    // POST: new LibraryBook constructed such that title, call_number and genre are set to 
    //       blank strings and isAvail is true
    {
        super();         // Call LibraryObject default constructor
        genre = "";
    }
    
    public LibraryDVD(String title, String call_number, String genre)
    // PRE:  title, call_number and genre are initialized
    // POST: new LibraryObject constructed such that title, call_number and genre are set to the values 
    //       passed the corresponding parameters and isAvail is true
    {
        super(title, call_number);
        this.genre = genre;
    }
    
    @Override
    public void CheckOut()
    // PRE:  class member isAvail == true
    // POST: isAvail is set to false and monthDue and dateDue are reset to DAYS_DUE days in the future
    {
        isAvail = false;
        ResetDueDate(DAYS_DUE);        // DVD is due DAYS_DUE days from now
    }
    
    @Override
    public String CheckIn()
    // PRE:  class member isAvail == false, monthDue, dateDue and title are initialized
    // POST: isAvail is set to true; return string that the DVD was returned and what the late fee was 
    //       if the DVD was late
    {
        isAvail = true;
        if (curDate > dateDue)
            return String.format("DVD '%s' returned, Late fee: $%.2f", title, (curDate - dateDue)*LATE_FEE);
        else
            return "DVD '" +  " returned";
    }
        
    @Override
    public String toString()
    // FTCVAL == string representation of a LibraryObject
    {
        return "DVD: '" + title + ", " + genre + "; " + call_number + "; " + GetStatus();
    }
}