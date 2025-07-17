using JellyAssembly;

namespace JellyEngine;

public class JellyApplication
{
    public JellyApplication()
    {
        JellyCore.Initialize();

        JellyCore.Instance!.Initialize(1280, 720, true, "JellyEngine", "Vulkan");

    }

    public void Play()
    {
        var jelly = JellyCore.Instance!;

        var testScene = new Scene("TestScene");

        SceneManager.SetCurrentScene(testScene.SceneId);

        while (jelly.IsRunning)
        {
            jelly.PollEvents();
            jelly.Render();
        }

        jelly.Dispose();
    }
}
