// Name:        Brian Golden
// Section:     001
// Date:        09/19/11
// Program:     Lab 08: LibraryCD Class
// Description: Implementation for a library CD class; LibraryCD objects have a title, artist,
//              genre, call number, availability, and due date.
//              Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/notes-polymorph.php
//              Modified to add DAYS_DUE class member, changed CheckOut() accordingly, added default and
//              init constructor, CheckIn() and toString() methods

public class LibraryCD extends LibraryObject
{
    private static final int DAYS_DUE = 7;          // Number of days from checkout the CD is due
    private static final double LATE_FEE = 0.5;     // Late fee per day, in dollars
    
    private String artist;                          // Musical artist who recorded the CD
    private String genre;                           // Genre of music, e.g., "rock," "folk," "hip-hop"
    
    public LibraryCD()
    // POST: new LibraryBook constructed such that title, call_number, artist and genre are set to 
    //       blank strings and isAvail is true
    {
        super();        // Call LibraryObject default constructor
        artist = "";
        genre = "";
    }
    
    public LibraryCD(String title, String call_number, String artist, String genre)
    // PRE:  title, call_number, artist and genre are initialized
    // POST: new LibraryObject constructed such that title, call_number, artist and genre are set to  
    //       the values passed the corresponding parameters and isAvail is true
    {
        super(title, call_number);
        this.artist = artist;
        this.genre = genre;
    }
    
    @Override
    public void CheckOut()
    // PRE:  class member isAvail == true
    // POST: isAvail is set to false and monthDue and dateDue are reset to DAYS_DUE days in the future
    {
        isAvail = false;
        ResetDueDate(DAYS_DUE);        // CD is due DAYS_DUE days from now
    }
    
    @Override
    public String CheckIn()
    // PRE:  class member isAvail == false, monthDue, dateDue and title are initialized
    // POST: isAvail is set to true; return string that the CD was returned and what the late fee was 
    //       if the CD was late
    {
        isAvail = true;
        if (curDate > dateDue)
            return String.format("CD '%s' returned, Late fee: $%.2f", title, (curDate - dateDue)*LATE_FEE);
        else
            return "CD '" +  " returned";
    }
    
    @Override
    public String toString()
    // FTCVAL == string representation of a LibraryObject
    {
        return "CD: '" + title + "' by " + artist + ", " + genre + "; " + call_number + GetStatus();
    }
}