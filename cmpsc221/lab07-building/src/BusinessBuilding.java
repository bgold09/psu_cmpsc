// Name:        Brian Golden
// Section:     001
// Date:        09/13/11
// Program:     Lab 07: Business Building class
// Description: BusinessBuilding class, used to represent a business building with name, description
//              and address of the building

public class BusinessBuilding extends Building
{
    protected String name;            // Name of the business
    protected String description;     // Description of the business
        
    public BusinessBuilding()
    // POST: A default BusinessBuilding has been constructed with blank streetAddress, name 
    //       and description; zip of 0
    {
        this(" ", " ", 0, " ");
    }
    
    public BusinessBuilding(String name, String streetAddress, int zip, String description)
    // PRE:  streetAddress, name, description are initialized; zip is a valid 5-digit US ZIP code
    // POST: A BusinessBuilding object has been constructed with data members streetAddress, zip 
    //       name and description set the values passed in the corresponding parameters    
    {
        super(streetAddress, zip);          // Call Building init constructor
        this.name = name;
        this.description = description;
    }
    
    @Override
    public String toString()
    // POST: FCTVAL == a String representation of the name, address and description
    //       of this BusinessBuilding
    {
        return name + "\n" + super.toString() + "\nDescription: " + description;
    }
}
