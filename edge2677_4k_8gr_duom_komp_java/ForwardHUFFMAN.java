
import my.*;

public class ForwardHUFFMAN implements Transformation
{
    //
    // Fields
    //
    Node[] tree = null;
    int nodeCount = 0;
    Symbol[] symbols = null;
    int[] freq = new int[256];
    int n = 0;
    byte[] xData = null;
    int xSize = 0;

    //
    // Private methods
    //
    void addCode(int root, int code)
    {
	if (tree[root].left == -1 && tree[root].right == -1)
	{
	    for (int i = 0; i < n; i++)
	    {
		if (symbols[i].c == tree[root].c)
		{
		    symbols[i].binCode[symbols[i].binCodeLength++] = (byte)code;
		}
	    }
	}
	else
	{
	    addCode(tree[root].left, code);
	    addCode(tree[root].right, code);
	}
    }

    //
    // Public methods
    //
    public byte[] execute(byte[] data)
    {
	//
	// Count character frequencies
	//
	for (int i = 0; i < data.length; i++)
	{
	    if (freq[data[i] + 128]++ == 0)
	    {
		n++;
	    }
	}

	//
	// Build character to symbol map
	//
	symbols = new Symbol[n];

	int k = 0;
	for (int i = 0; i < 256; i++)
	{
	    if (freq[i] > 0)
	    {
		symbols[k] = new Symbol();
		symbols[k].binCode = new byte[n];
		symbols[k].binCodeLength = 0;
		symbols[k].c = (byte)(i - 128);
		k++;
	    }
	}

	//
	// Build initial tree
	//
	tree = new Node[2 * n - 1];

	for (int i = 0; i < 256; i++)
	{
	    if (freq[i] > 0)
	    {
		tree[nodeCount] = new Node();
		tree[nodeCount].left = -1;
		tree[nodeCount].right = -1;
		tree[nodeCount].used = false;
		tree[nodeCount].weight = freq[i];
		tree[nodeCount].c = (byte)(i - 128);

		nodeCount++;
	    }
	}

	//
	// Build whole tree
	//
	while (nodeCount < 2 * n - 1)
	{
	    int min = Integer.MAX_VALUE;
	    int min1_i = 0;
	    int min2_i = 0;

	    for (int i = 0; i < nodeCount; i++)
	    {
		if (!tree[i].used && tree[i].weight < min)
		{
		    min = tree[i].weight;
		    min1_i = i;
		}
	    }

	    min = Integer.MAX_VALUE;
	    tree[min1_i].used = true;

	    for (int i = 0; i < nodeCount; i++)
	    {
		if (!tree[i].used && tree[i].weight < min)
		{
		    min = tree[i].weight;
		    min2_i = i;
		}
	    }

	    tree[min2_i].used = true;

	    tree[nodeCount] = new Node();
	    tree[nodeCount].left = min1_i;
	    tree[nodeCount].right = min2_i;
	    tree[nodeCount].used = false;
	    tree[nodeCount].weight = tree[min1_i].weight + tree[min2_i].weight;

	    nodeCount++;

	    addCode(min1_i, 0);
	    addCode(min2_i, 1);
	}

	//
	// Calculate code length
	//
	for (int i = 0; i < n; i++)
	{
	    xSize += symbols[i].binCodeLength * freq[symbols[i].c + 128];
	}

	int lastByteVal = xSize % 8;

	xSize /= 8; // size in bytes

	xSize += lastByteVal > 0 ? 1 : 0; // is partially used byte needed?

	xSize++; // the last byte

	xData = new byte[1024 + xSize]; // freq table + code size + last byte

	if (lastByteVal == 0)
	{
	    lastByteVal = 8;
	}

	xData[xData.length - 1] = (byte)lastByteVal;

	//
	// Write frequencies
	//
	int t = 0;
	for (int i = 0; i < 256; i++)
	{
	    for (int j = 0; j < 4; j++)
	    {
		xData[i * 4 + j] |= (freq[i] >> j * 8) & 0xff;
	    }
	    t += freq[i];
	}

	//
	// Write codes
	//
	int xOffset = 1024;
	int bitOffset = 0;

	for (int i = 0; i < data.length; i++)
	{
	    for (int j = 0; j < n; j++)
	    {
		if (symbols[j].c == data[i])
		{
		    for (k = symbols[j].binCodeLength - 1; k >= 0; k--)
		    {
			if (bitOffset == 8)
		        {
			    bitOffset = 0;
			    xOffset++;
			}

			if (symbols[j].binCode[k] == 0)
			{
			    xData[xOffset] &= ~(1 << bitOffset);
			}
			else
			{
			    xData[xOffset] |= 1 << bitOffset;
			}

			bitOffset++;
		    }
		}
	    }
	}

	return xData;
    }
}
