
import my.*;

public class InverseHUFFMAN implements Transformation
{
    //
    // Fields
    //
    Node[] tree = null;
    int nodeCount = 0;
    int[] freq = new int[256];
    int n = 0;
    byte[] xData = null;
    int xSize = 0;

    public byte[] execute(byte[] data)
    {
	for (int i = 0; i < 256; i++)
	{
	    for (int j = 0; j < 4; j++)
	    {
		freq[i] |= (data[i * 4 + j] << j * 8) & (0xff << j * 8);
	    }

	    if (freq[i] > 0)
	    {
		xSize += freq[i];
		n++;
	    }
	}

	xData = new byte[xSize];

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
	}

	//
	// Decode
	//
	int offset = 1024;
	int bitOffset = 0;
	int root = tree.length - 1;
	int k = 0;

	for (;;)
	{
	    while (tree[root].left != -1 && tree[root].right != -1)
	    {
		if (bitOffset == 8)
		{
		    bitOffset = 0;
		    offset++;
		}

		if (((data[offset] >> bitOffset++) & 1) == 0)
		{
		    root = tree[root].left;
		}
		else
		{
		    root = tree[root].right;
		}
	    }

	    xData[k++] = tree[root].c;
	    root = tree.length - 1; // return to the root node

	    if (offset >= data.length - 2 && data[data.length - 1] == bitOffset)
	    {
		break;
	    }
	}

	return xData;
    }
}
