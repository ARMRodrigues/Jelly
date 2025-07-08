using System;
using System.Runtime.InteropServices;

namespace JellyAssembly;

internal static unsafe class JellyInterop
{
    private static nint _libHandle;

    internal static delegate* unmanaged[Cdecl]<JellyHandle*> JellyCreate;
    internal static delegate* unmanaged[Cdecl]<JellyHandle*, int, int, bool, sbyte*, sbyte*, int> JellyInitialize;
    internal static delegate* unmanaged[Cdecl]<JellyHandle*, int> JellyIsRunning;
    internal static delegate* unmanaged[Cdecl]<JellyHandle*, void> JellyPollEvents;
    internal static delegate* unmanaged[Cdecl]<JellyHandle*, void> JellyRender;
    internal static delegate* unmanaged[Cdecl]<JellyHandle*, void> JellyDestroy;
    
    public static void Load(string libPath)
    {
        //_libHandle = NativeLibrary.Load(libPath);
        if (OperatingSystem.IsWindows())
        {
            _libHandle = NativeLibrary.Load(Path.Combine(AppContext.BaseDirectory, "Jelly.dll"));
        }
        else if (OperatingSystem.IsLinux())
        {
            _libHandle = NativeLibrary.Load(Path.Combine(AppContext.BaseDirectory, "libJelly.so"));
        }

        JellyCreate = (delegate* unmanaged[Cdecl]<JellyHandle*>)GetSymbol("jelly_create");
        JellyInitialize = (delegate* unmanaged[Cdecl]<JellyHandle*, int, int, bool, sbyte*, sbyte*, int>)GetSymbol("jelly_initialize");
        JellyIsRunning = (delegate* unmanaged[Cdecl]<JellyHandle*, int>)GetSymbol("jelly_is_running");
        JellyPollEvents = (delegate* unmanaged[Cdecl]<JellyHandle*, void>)GetSymbol("jelly_poll_events");
        JellyRender = (delegate* unmanaged[Cdecl]<JellyHandle*, void>)GetSymbol("jelly_render");
        JellyDestroy = (delegate* unmanaged[Cdecl]<JellyHandle*, void>)GetSymbol("jelly_destroy");
    }
    
    private static nint GetSymbol(string name)
    {
        if (!NativeLibrary.TryGetExport(_libHandle, name, out var symbol))
            throw new InvalidOperationException($"Failed to load symbol: {name}");
        return symbol;
    }
}