
import my.*;

public class InverseMTF implements Transformation
{
    public byte[] execute(byte[] data)
    {
	int n = data[0] < 0 ? data[0] + 256 : data[0];
	byte[] dict = new byte[n];

	for (int i = 0; i < n; i++)
	{
	    dict[i] = data[1 + i];
	}

	byte xData[] = new byte[data.length - n - 1];

	int k = 0;
	for (int i = n + 1; i < data.length; i++)
	{
	    int j = data[i] < 0 ? data[i] + 256: data[i];

	    xData[k++] = dict[j];

	    byte tmp = dict[j];
	    for (int l = j; l > 0; l--)
	    {
		dict[l] = dict[l - 1];
	    }
	    dict[0] = tmp;
	}

	return xData;
    }
}
