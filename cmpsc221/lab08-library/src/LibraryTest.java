// Name:        Brian Golden
// Section:     001
// Date:        09/19/11
// Program:     Lab 08: Library Test Driver
// Description: Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/notes-polymorph.php
//              Modified to test all added methods and constructors

public class LibraryTest
{
    public static void main(String[] args)
    {
        LibraryObject[] lib = new LibraryObject[8];     // library

        lib[0] = new LibraryBook();             // create some items
        lib[1] = new LibraryDVD();
        lib[2] = new LibraryBook();
        lib[3] = new LibraryDVD();
        lib[4] = new LibraryCD();
        lib[5] = new LibraryBook("Blink", "BF448.G53 2005", "Malcolm Gladwell");
        lib[6] = new LibraryDVD("Fight Club", "PN1997.F54 2002", "Psychological Thriller");
        lib[7] = new LibraryCD("Echo", "Disc MCD00-297", "Tom Petty & The Heartbreakers", "Rock");

        System.out.println("Initial Library: ");
        for(LibraryObject cur : lib)            // go through whole library
        {
            System.out.println(cur.GetStatus());
        }

        lib[0].CheckOut();                      // check out some items
        lib[1].CheckOut();
        lib[3].CheckOut();
        lib[4].CheckOut();
        lib[5].CheckOut();
        lib[6].CheckOut();
        lib[7].CheckOut();

        System.out.println("\nLibrary after check outs:");
        for(LibraryObject cur : lib)            // go through whole library
        {
            System.out.println(cur.GetStatus());
        }
        
        LibraryObject.ResetDate(9, 22);
        
        System.out.println("\nCheck in items:");
        System.out.println(lib[0].CheckIn());                      // check in some items
        System.out.println(lib[1].CheckIn());
        System.out.println(lib[3].CheckIn());
        System.out.println(lib[4].CheckIn());
        System.out.println(lib[5].CheckIn());
        System.out.println(lib[6].CheckIn());
        System.out.println(lib[7].CheckIn());
        
        System.out.println("\nLibrary after check ins:");
        for(LibraryObject cur : lib)            // go through whole library
        {
            System.out.println(cur.GetStatus());
        }
    }
}