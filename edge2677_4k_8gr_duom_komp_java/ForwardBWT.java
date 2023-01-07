
import java.util.*;
import my.*;

public class ForwardBWT implements Transformation
{
    public byte[] execute(byte[] data)
    {
	Integer[] indexes = new Integer[data.length];

	for (int i = 0; i < data.length; i++)
	{
	    indexes[i] = new Integer(i);
	}

	Arrays.sort(indexes, new MyComparator(data));

	byte[] xData = new byte[4 + data.length];
	int primaryIndex = 0;

	for (int i = 0; i < data.length; i++)
        {
	    xData[4 + i] = data[(indexes[i].intValue() + data.length - 1) % data.length];
	}

	for (int i = 0; i < data.length; i++)
	{
	    if (indexes[i].intValue() == 1)
	    {
		primaryIndex = i;

		break;
	    }
	}

	//
	// Save primary index
	//
	for (int i = 0; i < 4; i++)
	{
	    xData[i] = (byte)((primaryIndex >> i * 8) & 0xff);
	}

	return xData;
    }
}

class MyComparator implements Comparator
{
    //
    // Fields
    //
    private byte[] data;

    //
    // Methods
    //

    // Constructor
    public MyComparator(byte[] data)
    {
	this.data = data;
    }

    public int compare(Object o1, Object o2)
    {
	int s1 = ((Integer)o1).intValue();
	int s2 = ((Integer)o2).intValue();
	int le = data.length;

	while (data[s1] == data[s2])
	{
	    if (++s1 == data.length)
	    {
		s1 = 0;
	    }

	    if (++s2 == data.length)
	    {
		s2 = 0;
	    }

	    if (--le == 0)
	    {
		return 0;
	    }
	}

	if (data[s1] > data[s2])
	{
	    return 1;
	}
	else
        {
	    return -1;
	}
    }
}
