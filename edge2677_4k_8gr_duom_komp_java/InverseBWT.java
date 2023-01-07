
import my.*;

public class InverseBWT implements Transformation
{
    public byte[] execute(byte[] data)
    {
	byte[] aux = new byte[data.length - 4];
	int[] indexes = new int[data.length - 4];
	int[] buckets = new int[256];
	int primaryIndex = 0;
	byte[] xData = new byte[data.length - 4];

	for (int i = 0; i < 4; i++)
	{
	    primaryIndex |= (data[i] << 8 * i) & (0xff << 8 * i);
	}

	for (int i = 0; i < 256; i++)
	{
	    buckets[i] = 0;
	}

	for (int i = 0; i < data.length - 4; i++)
	{
	    buckets[data[4 + i] + 128]++;
	}

	int k = 0;
	for (int i = 0; i < 256; i++)
	{
	    for (int j = 0; j < buckets[i]; j++)
	    {
		aux[k++] = (byte)i;
	    }
	}

	int j = 0;
	for (int i = 0; i < 256; i++)
	{
	    while (j < (data.length - 4) && i > (aux[j] < 0 ? aux[j] + 256 : aux[j]))
	    {
		j++;
	    }

	    buckets[i] = j;
	}

	for (int i = 0; i < data.length - 4; i++)
	{
	    indexes[buckets[data[4 + i] + 128]++] = i;
	}

	j = primaryIndex;
	for (int i = 0; i < data.length - 4; i++)
        {
	    xData[i] = data[4 + j];
	    j = indexes[j];
	}

	return xData;
    }
}
