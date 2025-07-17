using JellyAssembly;

namespace JellyEngine;

public class Scene
{
    public int SceneId { get; }
    public string Name { get; }

    public Scene(string sceneName)
    {
        SceneId = JellyCore.Instance!.CreateScene(sceneName);
        Name = sceneName;

    }

    public override string ToString() => $"Scene[{SceneId}]: {Name}";
}
