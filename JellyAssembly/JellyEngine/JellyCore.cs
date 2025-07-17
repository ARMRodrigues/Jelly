using JellyAssembly;

namespace JellyEngine;

public static class JellyCore
{
    public static Jelly? Instance { get; private set; }

    public static void Initialize()
    {
        if (Instance != null) return;

        Instance = JellyAssembly.JellyNative.Create();
    }
}
