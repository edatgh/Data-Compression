
import java.io.*;
import my.*;

public class Data
{
    //
    // Fields
    //
    byte[] data = null;

    //
    // Methods
    //

    // Constructor
    public Data()
    {
	// emtpy
    }

    public int getSize()
    {
	return data.length;
    }

    void readFromFile(String fileName)
    {
	FileInputStream fis = null;

	try
	{
	    fis = new FileInputStream(fileName);
	}
	catch (FileNotFoundException fnfe)
	{
	    System.out.println("File not found: " + fileName);
	    return;
	}

	try
        {
	    data = new byte[fis.available()];
	    fis.read(data);
	    fis.close();
	}
	catch (IOException ioe)
	{
	    System.out.println("IO Error: " + ioe.getMessage());
	}
    }

    void transform(Transformation t)
    {
	if (data == null)
	{
	    System.out.println("There is nothing to be transformed");
	    return;
	}

	data = t.execute(data);
    }

    void writeToFile(String fileName)
    {
	FileOutputStream fos = null;

	try
	{
	    fos = new FileOutputStream(fileName);
	}
	catch (FileNotFoundException fnfe)
	{
	    System.out.println("File not found: " + fileName);
	    return;
	}

	if (data == null)
	{
	    System.out.println("Nothing to be written");
	    return;
	}

	try
        {
	    fos.write(data);
	    fos.close();
	}
	catch (IOException ioe)
	{
	    System.out.println("IO Error: " + ioe.getMessage());
	}
    }
}
