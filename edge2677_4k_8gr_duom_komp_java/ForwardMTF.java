
import my.*;

public class ForwardMTF implements Transformation
{
    public byte[] execute(byte[] data)
    {
	// Create dictionary
	int freq[] = new int[256];
	byte dict[] = new byte[256];
	int n = 0;

	for (int i = 0; i < data.length; i++)
	{
	    if (freq[data[i] + 128]++ == 0)
	    {
		dict[n++] = data[i];
	    }
	}

	byte xData[] = new byte[1 + n + data.length];
	xData[0] = (byte)n;

	for (int i = 0; i < n; i++)
	{
	    xData[i + 1] = dict[i];
	}

	int k = 0;
	for (int i = 0; i < data.length; i++)
	{
	    for (int j = 0; j < n; j++)
	    {
		if (data[i] == dict[j])
		{
		    xData[1 + n + k++] = (byte)j;

		    byte tmp = dict[j];

		    for (int l = j; l > 0; l--)
		    {
			dict[l] = dict[l - 1];
		    }

		    dict[0] = tmp;
		}
	    }
	}

	return xData;
    }
}
