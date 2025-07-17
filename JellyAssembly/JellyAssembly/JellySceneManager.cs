using System.Runtime.InteropServices;

namespace JellyAssembly;

public sealed partial class Jelly : IDisposable
{
    public int CreateScene(string name)
    {
        unsafe
        {
            var ptr = Marshal.StringToHGlobalAnsi(name);
            int sceneId = JellyInterop.JellySceneManagerCreateScene(_jellyHandle, (sbyte*)ptr);
            Marshal.FreeHGlobal(ptr);
            return sceneId;
        }
    }

    public void DestroyScene(int sceneId)
    {
        unsafe { JellyInterop.JellySceneManagerDestroyScene(_jellyHandle, sceneId); }
    }

    public void SetActiveScene(int sceneId)
    {
        unsafe { JellyInterop.JellySceneManagerSetActiveScene(_jellyHandle, sceneId); }
    }
}
