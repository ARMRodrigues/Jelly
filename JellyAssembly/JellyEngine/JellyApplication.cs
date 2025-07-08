using JellyAssembly;

namespace JellyEngine;

public class JellyApplication
{
    Jelly _jellyEngine;
    
    public JellyApplication()
    {
        _jellyEngine = JellyNative.Create();
    }

    public void Play()
    {
        _jellyEngine.Initialize(1280, 720, true, "JellyEngine", "Vulkan");

        while (_jellyEngine.IsRunning)
        {
            _jellyEngine.PollEvents();
        }
        
        _jellyEngine.Dispose();
    }
}