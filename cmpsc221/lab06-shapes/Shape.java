// Very simple Shape class to be extended in Inheritance exercises.

import java.lang.String;

public class Shape
{
   protected String name;           // user-chosen name for the shape

   public Shape()
   // POST: A default Shape is created with name set to an empty string
   {
      name = "";
   }

   public void SetName(String name)
   // PRE:  Assigned(name)
   // POST: class member name has been set to name
   {
      this.name = name;
   }

   public String toString()
   // POST: FCTVAL == String representation of Shape object
   {
      return "Shape " + name;
   }
}