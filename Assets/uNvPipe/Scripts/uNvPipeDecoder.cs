using UnityEngine;
using UnityEngine.Events;
using System.Threading.Tasks;

namespace uNvPipe
{

public class uNvPipeDecoder : MonoBehaviour
{
    [SerializeField]
    public int width = 1920;

    [SerializeField]
    public int height = 1080;

    [SerializeField]
    public Codec codec = Codec.H264;

    [SerializeField]
    public Format format = Format.RGBA32;

    [SerializeField]
    bool multithreaded = true;

    public int id { get; private set; } = -1;

    public bool isValid
    {
        get {  return Lib.IsDecoderValid(id); }
    }

    public System.IntPtr decodedData
    {
        get { return Lib.GetDecoderDecodedData(id); }
    }

    public int decodedSize
    {
        get { return Lib.GetDecoderDecodedSize(id); }
    }

    public class DecodedCallback : UnityEvent<System.IntPtr, int> {}
    public DecodedCallback onDecoded { get; private set; } = new DecodedCallback();

    bool isDecoding_ = false;
    bool isDecoded_ = false;

    void OnEnable()
    {
        id = Lib.CreateDecoder();

        Lib.SetDecoderWidth(id, width);
        Lib.SetDecoderHeight(id, height);
        Lib.SetDecoderCodec(id, codec);
        Lib.SetDecoderFormat(id, format);

        if (!Lib.InitializeDecoder(id))
        {
            Debug.LogError(Lib.DecoderGetError(id));
            return;
        }
    }

    void OnDisable()
    {
        if (isValid)
        {
            Lib.DeleteDecoder(id);
        }
    }

    void _Decode(System.IntPtr data, int size)
    {
        if (!isValid || isDecoding_) return;

        isDecoding_ = true;
        isDecoded_ = Lib.DecoderDecode(id, data, size);
        isDecoding_ = false;

        if (!isDecoded_)
        {
            Debug.LogError(Lib.DecoderGetError(id));
        }
    }

    public async void Decode(System.IntPtr data, int size)
    {
        if (multithreaded)
        {
            await Task.Run(() => _Decode(data, size));
        }
        else
        {
            _Decode(data, size);
        }

        if (isDecoded_)
        {
            InvokeCallback();
        }
    }

    void InvokeCallback()
    {
        if (onDecoded == null) return;

        var ptr = Lib.GetDecoderDecodedData(id);
        var size = Lib.GetDecoderDecodedSize(id);
        onDecoded.Invoke(ptr, size);
    }
}

}
