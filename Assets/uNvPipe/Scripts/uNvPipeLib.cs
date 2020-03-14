using System;
using System.Runtime.InteropServices;

namespace uNvPipe
{

public enum Codec
{
    H264,
    HEVC,
}

public enum Format
{
    RGBA32,
    UINT4,
    UINT8,
    UINT16,
    UINT32,
}

public enum Compression
{
    LOSSY,
    LOSSLESS,
}

public static class Lib
{
    public const string dllName = "uNvPipe";

    // ---

    [DllImport(dllName, EntryPoint = "uNvPipeCreateEncoder")]
    public static extern int CreateEncoder();
    [DllImport(dllName, EntryPoint = "uNvPipeDeleteEncoder")]
    public static extern void DeleteEncoder(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeIsEncoderValid")]
    public static extern bool IsEncoderValid(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeInitializeEncoder")]
    public static extern bool InitializeEncoder(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeEncoderGetError")]
    private static extern IntPtr EncoderGetErrorInternal(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderWidth")]
    public static extern void SetEncoderWidth(int id, int width);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderHeight")]
    public static extern void SetEncoderHeight(int id, int height);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderFormat")]
    public static extern void SetEncoderFormat(int id, Format format);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderCodec")]
    public static extern void SetEncoderCodec(int id, Codec codec);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderCompression")]
    public static extern void SetEncoderCompression(int id, Compression compression);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderBitrate")]
    public static extern void SetEncoderBitrate(int id, float bitrate);
    [DllImport(dllName, EntryPoint = "uNvPipeSetEncoderFps")]
    public static extern void SetEncoderFps(int id, int fps);
    [DllImport(dllName, EntryPoint = "uNvPipeEncoderEncode")]
    public static extern bool EncoderEncode(int id, IntPtr data, bool forceIframe);
    [DllImport(dllName, EntryPoint = "uNvPipeGetEncoderEncodedSize")]
    public static extern int GetEncoderEncodedSize(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeGetEncoderEncodedData")]
    public static extern IntPtr GetEncoderEncodedData(int id);

    public static string EncoderGetError(int id)
    {
        var ptr = EncoderGetErrorInternal(id);
        return Marshal.PtrToStringAnsi(ptr);
    }

    // ---

    [DllImport(dllName, EntryPoint = "uNvPipeCreateDecoder")]
    public static extern int CreateDecoder();
    [DllImport(dllName, EntryPoint = "uNvPipeDeleteDecoder")]
    public static extern void DeleteDecoder(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeIsDecoderValid")]
    public static extern bool IsDecoderValid(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeInitializeDecoder")]
    public static extern bool InitializeDecoder(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeDecoderGetError")]
    public static extern IntPtr DecoderGetErrorInternal(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeSetDecoderWidth")]
    public static extern void SetDecoderWidth(int id, int width);
    [DllImport(dllName, EntryPoint = "uNvPipeSetDecoderHeight")]
    public static extern void SetDecoderHeight(int id, int height);
    [DllImport(dllName, EntryPoint = "uNvPipeSetDecoderFormat")]
    public static extern void SetDecoderFormat(int id, Format format);
    [DllImport(dllName, EntryPoint = "uNvPipeSetDecoderCodec")]
    public static extern void SetDecoderCodec(int id, Codec codec);
    [DllImport(dllName, EntryPoint = "uNvPipeDecoderDecode")]
    public static extern bool DecoderDecode(int id, IntPtr data, int size);
    [DllImport(dllName, EntryPoint = "uNvPipeGetDecoderDecodedSize")]
    public static extern int GetDecoderDecodedSize(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeGetDecoderDecodedData")]
    public static extern IntPtr GetDecoderDecodedData(int id);
    [DllImport(dllName, EntryPoint = "uNvPipeDecoderGetTextureUpdateCallback")]
    public static extern IntPtr GetTextureUpdateCallback();

    public static string DecoderGetError(int id)
    {
        var ptr = DecoderGetErrorInternal(id);
        return Marshal.PtrToStringAnsi(ptr);
    }
}

}
