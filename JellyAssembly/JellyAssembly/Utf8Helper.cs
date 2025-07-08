namespace JellyAssembly;

internal static class Utf8Helper
{
    internal unsafe delegate void Utf8StringAction(sbyte* ptr);
    
    public static unsafe void WithUtf8String(string str, Utf8StringAction action)
    {
        ArgumentNullException.ThrowIfNull(str);

        var bytes = System.Text.Encoding.UTF8.GetBytes(str + '\0');
        fixed (byte* bytePtr = bytes)
        {
            action((sbyte*)bytePtr);
        }
    }
    
    public static byte[] GetUtf8NullTerminatedBytes(string str)
    {
        ArgumentNullException.ThrowIfNull(str);

        return System.Text.Encoding.UTF8.GetBytes(str + '\0');
    }
}
