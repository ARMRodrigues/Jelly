using System.Runtime.InteropServices;

namespace JellyAssembly;

[StructLayout(LayoutKind.Sequential)]
internal struct JellyHandle
{
    private nint _ptr;
}