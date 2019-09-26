using System;
using System.Runtime.InteropServices;

#pragma warning disable CS0465

namespace uNvEncoder
{

public enum Format
{
    R8G8B8A8_UNORM = 28,
    B8G8R8A8_UNORM = 87,
    NV12 = 103,
    UNKNOWN = 0,
}

[StructLayout(LayoutKind.Sequential), Serializable]
public struct EncoderDesc
{
    [MarshalAs(UnmanagedType.I4)]
    public int width;
    [MarshalAs(UnmanagedType.I4)]
    public int height;
    [MarshalAs(UnmanagedType.I4)]
    public int frameRate;
    [MarshalAs(UnmanagedType.I4)]
    public int bitRate;
    [MarshalAs(UnmanagedType.I4)]
    public int maxFrameSize;
    [MarshalAs(UnmanagedType.I4)]
    public Format format;
}

public static class Lib
{
    public const string dllName = "uNvEncoder";

    // ---

    [DllImport(dllName, EntryPoint = "uNvEncoderCreate")]
    private static extern int CreateInternal(IntPtr desc);
    [DllImport(dllName, EntryPoint = "uNvEncoderDestroy")]
    public static extern int Destroy(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderIsValid")]
    public static extern bool IsValid(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderReconfigure")]
    private static extern void ReconfigureInternal(int id, IntPtr desc);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetWidth")]
    public static extern int GetWidth(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetHeight")]
    public static extern int GetHeight(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetFormat")]
    public static extern Format GetFormat(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetFrameRate")]
    public static extern int GetFrameRate(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderEncode")]
    public static extern bool Encode(int id, IntPtr texturePtr, bool forceIdrFrame);
    [DllImport(dllName, EntryPoint = "uNvEncoderEncodeSharedHandle")]
    public static extern bool EncodeSharedHandle(int id, IntPtr sharedHandle, bool forceIdrFrame);
    [DllImport(dllName, EntryPoint = "uNvEncoderCopyEncodedData")]
    public static extern void CopyEncodedData(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetEncodedDataCount")]
    public static extern int GetEncodedDataCount(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetEncodedDataSize")]
    public static extern int GetEncodedDataSize(int id, int index);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetEncodedDataBuffer")]
    public static extern IntPtr GetEncodedDataBuffer(int id, int index);
    [DllImport(dllName, EntryPoint = "uNvEncoderGetError")]
    private static extern IntPtr GetErrorInternal(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderHasError")]
    public static extern bool HasError(int id);
    [DllImport(dllName, EntryPoint = "uNvEncoderClearError")]
    public static extern void ClearError(int id);

    public static int Create(EncoderDesc desc)
    {
        var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(EncoderDesc)));
        Marshal.StructureToPtr(desc, ptr, false);
        var id = CreateInternal(ptr);
        Marshal.FreeHGlobal(ptr);
        return id;
        }

    public static void Reconfigure(int id, EncoderDesc desc)
    {
        var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(EncoderDesc)));
        Marshal.StructureToPtr(desc, ptr, false);
        ReconfigureInternal(id, ptr);
        Marshal.FreeHGlobal(ptr);
    }

    public static string GetError(int id)
    {
        var ptr = GetErrorInternal(id);
        return Marshal.PtrToStringAnsi(ptr);
    }
}

}
