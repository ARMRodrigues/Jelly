namespace JellyAssembly;

public sealed class Jelly : IDisposable
{
    private readonly unsafe JellyHandle* _jellyHandle;

    internal unsafe Jelly(JellyHandle* jellyHandle)
    {
        _jellyHandle = jellyHandle;
    }
    
    public bool Initialize(int width, int height, bool vsync, string title, string apiName)
    {
        unsafe
        {
            var titleBytes = Utf8Helper.GetUtf8NullTerminatedBytes(title);
            var apiBytes   = Utf8Helper.GetUtf8NullTerminatedBytes(apiName);

            fixed (byte* titleRaw = titleBytes)
            fixed (byte* apiRaw = apiBytes)
            {
                var titlePtr = (sbyte*)titleRaw;
                var apiPtr   = (sbyte*)apiRaw;

                return JellyInterop.JellyInitialize(_jellyHandle, width, height, vsync, titlePtr, apiPtr) == 1;
            }
        }
    }
    
    public bool IsRunning
    {
        get
        {
            unsafe { return JellyInterop.JellyIsRunning(_jellyHandle) != 0; }
        }
    }

    public void PollEvents()
    {
        unsafe { JellyInterop.JellyPollEvents(_jellyHandle); }
    }

    public void Render()
    {
        unsafe { JellyInterop.JellyRender(_jellyHandle); }
    }

    public void Dispose()
    {
        unsafe { JellyInterop.JellyDestroy(_jellyHandle); }
    }
}