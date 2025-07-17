namespace JellyEngine;

public class SceneManager
{
    static Dictionary<string, Scene> scenes = new Dictionary<string, Scene>();

    public SceneManager()
    {

    }

    public static void SetCurrentScene(int sceneId)
    {
        JellyCore.Instance!.SetActiveScene(sceneId);
    }
}
