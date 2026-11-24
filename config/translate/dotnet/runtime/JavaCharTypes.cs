// Java's Character.getType, by Java's numbers and Java's Unicode version: .NET's own tables number the categories differently and follow another version.
public static class JavaCharTypes
{
    static readonly ushort[] Runs =
    {
/*JAVA_CHAR_TYPES*/    };

    public static int Of(char c)
    {
        int low = 0, high = Runs.Length / 2;
        while (high - low > 1)
        {
            int middle = low + (high - low) / 2;
            if (Runs[middle * 2] <= c) low = middle; else high = middle;
        }
        return Runs[low * 2 + 1];
    }
}
