using UnityEngine;
using UnityEngine.Events;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace uNvPipe
{

public class uNvPipeEncoder : MonoBehaviour
{
    [SerializeField]
    public int width = 1920;

    [SerializeField]
    public int height = 1080;

    [SerializeField]
    public float bitrateMbps = 32f;

    [SerializeField]
    public int fps = 60;

    [SerializeField]
    public Codec codec = Codec.H264;

    [SerializeField]
    public Format format = Format.RGBA32;

    [SerializeField]
    public Compression compression = Compression.LOSSY;

    public int id { get; private set; } = -1;

    public bool isValid
    {
        get { return Lib.IsEncoderValid(id); }
    }

    public class EncodedCallback : UnityEvent<System.IntPtr, int> {}
    public EncodedCallback onEncoded { get; private set; } = new EncodedCallback();

    bool isEncoding_ = false;
    bool isEncoded_ = false;

    void OnEnable()
    {
        id = Lib.CreateEncoder();

        Lib.SetEncoderWidth(id, width);
        Lib.SetEncoderHeight(id, height);
        Lib.SetEncoderBitrate(id, bitrateMbps);
        Lib.SetEncoderFps(id, fps);
        Lib.SetEncoderCodec(id, codec);
        Lib.SetEncoderFormat(id, format);
        Lib.SetEncoderCompression(id, compression);

        if (!Lib.InitializeEncoder(id))
        {
            Debug.LogError(Lib.EncoderGetError(id));
            return;
        }
    }

    void OnDisable()
    {
        Lib.DeleteEncoder(id);
    }

    void _Encode(System.IntPtr data, bool forceIframe)
    {
        if (!isValid || isEncoding_) return;

        isEncoding_ = true;
        isEncoded_ = Lib.EncoderEncode(id, data, forceIframe);
        isEncoding_ = false;

        if (!isEncoded_)
        {
            Debug.LogError(Lib.EncoderGetError(id));
        }
    }

    public void Encode(System.IntPtr data, bool forceIframe = false)
    {
        _Encode(data, forceIframe);

        if (isEncoded_)
        {
            InvokeCallback();
        }
    }

    public void Encode(Texture2D texture, bool forceIframe = false)
    {
        var pixels  = texture.GetPixels32();
        var handle  = GCHandle.Alloc(pixels, GCHandleType.Pinned);
        var pointer = handle.AddrOfPinnedObject();

        Encode(pointer, forceIframe);

        handle.Free();
    }

    void InvokeCallback()
    {
        if (onEncoded == null) return;

        var ptr = Lib.GetEncoderEncodedData(id);
        var size = Lib.GetEncoderEncodedSize(id);
        onEncoded.Invoke(ptr, size);
    }
}

}
