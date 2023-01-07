
import java.util.*;
import my.*;

public class Main
{
    public static void main(String[] args)
    {
	//
	// Check program command line arguments
	//
	if (args.length != 3)
	{
	    System.out.println("USAGE: <[cd]> <input_file_name> <output_file_name>");
	    return; // exit program
	}

	//
	// Parse command line
	//
        String op = args[0];
	String inputFileName = args[1];
	String outputFileName = args[2];

	Data data = new Data();

	Transformation[] t = new Transformation[3];

	//
	// Create appropriate transformations
	//
	if (op.equals("c"))
	{
	    t[0] = new ForwardBWT();
	    t[1] = new ForwardMTF();
	    t[2] = new ForwardHUFFMAN();
	}
	else if (op.equals("d"))
	{
	    t[0] = new InverseHUFFMAN();
	    t[1] = new InverseMTF();
	    t[2] = new InverseBWT();
	}
	else
	{
	    System.out.println("Invalid operation");
	    return;
	}

	data.readFromFile(inputFileName);

	for (int i = 0; i < t.length; i++)
	{
	    System.out.println("Running transformation: " +
			       t[i].toString().subSequence(0, t[i].toString().indexOf("@")));
	    data.transform(t[i]);
	}

	data.writeToFile(outputFileName);

	System.out.println("Done.");
    }
}
