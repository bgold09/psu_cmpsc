// Name:        Brian Golden
// Section:     001
// Date:        09/13/11
// Program:     Lab 07: Building class
// Description: Building class, used to represent different buildings in the same city (hence address is 
//              simplified). Intended to be subclassed. 
//              Downloaded from http://php.scripts.psu.edu/djh300/cmpsc221/Building.java; 
//              Class members streetAddress and zip modified to protected access

public class Building
{
   protected String streetAddress;              // street address of building, e.g. "123 Any St."
   protected int zip;                           // ZIP code of address, 5 digits

   public Building()
   // POST: A default Building has been constructed with blank streetAddress and zip of 0
   {
      this (" ", 0);        // Call init constructor
   }

   public Building(String streetAddress, int zip)
   // PRE:  streetAddress is initialized; zip is a valid 5-digit US ZIP code
   // POST: A Building object has been constructed with data members streetAddress and zip 
   //       set the values passed in the corresponding parameters
   {
      this.streetAddress = streetAddress;
      this.zip = zip; 
   }

   @Override
   public String toString()
   // POST: FCTVAL == a String representation of the address of this Building
   {
      return streetAddress + ", " + zip; 
   }
}