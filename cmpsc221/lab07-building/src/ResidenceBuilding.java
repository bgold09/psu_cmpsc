// Name:        Brian Golden
// Section:     001
// Date:        09/13/11
// Program:     Lab 07: Residence Building class
// Description: ResidenceBuilding class, used to represent a residence building with type, number of
//              residents and address of the building

public class ResidenceBuilding extends Building
{
    private String type;            // Kind of residence
    private int numResidents;       // Number of people living in the residence
    
    public ResidenceBuilding()
    // POST: A default ResidenceBuilding has been constructed with blank streetAddress and type;
    //       zip and numResidents of 0
    {
       this(" ", 0, " ", 0);
    }
    
    public ResidenceBuilding(String streetAddress, int zip, String type, int numResidents)
    // PRE:  streetAddress, type are initialized; zip is a valid 5-digit US ZIP code;
    //       numResidents >= 0
    // POST: A ResidenceBuilding object has been constructed with data members streetAddress, zip 
    //       type and numResidents set the values passed in the corresponding parameters    
    {
        super(streetAddress, zip);          // Call Building init Constructor
        this.type = type;
        this.numResidents = numResidents;
    }
    
    @Override
    public String toString()
    // POST: FCTVAL == a String representation of the address, type and number of residents
    //       of this ResidenceBuilding
    {
        return super.toString() + "\nType of residence: " + type + "\nNumber of residents: " 
                + numResidents;
    }
}
