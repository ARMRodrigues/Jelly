namespace JellyAssembly;

public static class JellyNative
{
    static JellyNative()
    {
        JellyInterop.Load("libJelly");
    }

    public static Jelly Create()
    {
        unsafe
        {
            var handle = JellyInterop.JellyCreate();

            if (handle == null)
            {
                throw new Exception("Failed to create Jelly Engine");
            }

            return new Jelly(handle);
        }
    }
}
