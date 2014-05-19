// Name:        Brian Golden
// Section:     001
// Date:        09/19/11
// Program:     Lab 08: LibraryObject Class
// Description: Implementation for a library object class; LibraryObject objects have a title, 
//              call number, availability, and due date. (Intended to be subclassed).
//              Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/notes-polymorph.php
//              Modified to add ResetDate(), CheckIn() and toString() methods and init constructor;
//              modified default constructor

public abstract class LibraryObject
{
    protected static int curMonth = 9;   // current month number
    protected static int curDate = 1;    // current day number

    protected String title;              // title of object
    protected String call_number;        // call number of object in library system
    protected boolean isAvail;           // availability indicator: true when object is 
                                         //   not checked out, false when it is
    protected int monthDue;              // number of months until object is due back
    protected int dateDue;               // date of monthDue when object is due back

    public LibraryObject()
    // POST: new LibraryObject constructed such that 
    //       title and call_number are set to blank strings and isAvail is true
    {
        this("", "");
        isAvail = true;
    }
    
    public LibraryObject(String title, String call_number)
    // PRE:  title and call_number are initialized
    // POST: new LibraryObject constructed such that title and call_number are set to the values passed 
    //       the corresponding parameters and isAvail is true
    {
        this.title = title;
        this.call_number = call_number;
        isAvail = true;
    }

    public abstract void CheckOut();
    // PRE:  class member isAvail == true
    // POST: isAvail is set to false and monthDue and dateDue are reset
    //       according to what type of item this is
    
    public abstract String CheckIn();
    // PRE:  class member isAvail == false, monthDue, dateDue and title are initialized
    // POST: isAvail is set to true; return string that tells type of the item, that it was returned
    //       and what the late fee was if the item was late

    public void ResetDueDate(int numDays)
    // PRE:  numDays > 0
    // POST: monthDue and dateDue are adjusted numDays into the future
    {
        monthDue = curMonth; 
        dateDue = curDate + numDays;
    }
    
    public static void ResetDate(int newMonth, int newDate)    
    // PRE: 0 < newMonth <= 12  &&  0 < newDate <= 31
    // POST: curMonth and curDate are set to newMonth and newDate, respectively
    {
        curMonth = newMonth;
        curDate = newDate;
    }

    public String GetStatus()
    // POST: FCTVAL == string stating "Available" if item is available or
    //                 "Due Back [month]/[date]" is item is checked out
    {
        if(isAvail)                     // item is not checked out
        {
            return "Available";
        }
        else                            // item is checked out
        {
            return "Due Back " + monthDue + "/" + dateDue;
        }
    }
    
    @Override
    public String toString()
    // FTCVAL == string representation of a LibraryObject
    {
        return "'" + title + "'; " + call_number + "; " + GetStatus();
    }
}