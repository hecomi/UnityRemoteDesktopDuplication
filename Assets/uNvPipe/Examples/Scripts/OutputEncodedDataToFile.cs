using UnityEngine;
using UnityEngine.Assertions;
using System.IO;
using System.Runtime.InteropServices;

namespace uNvPipe.Examples
{

public class OutputEncodedDataToFile : MonoBehaviour
{
    [SerializeField]
    uNvPipeEncoder encoder = null;

    [SerializeField]
    string filePath = "test.h264";

    FileStream fileStream_;
    BinaryWriter binaryWriter_;

    void Start()
    {
        Assert.IsNotNull(encoder, "Please set encoder to EncoderToDecoder.");

        if (!encoder) return;

        fileStream_ = new FileStream(filePath, FileMode.Create, FileAccess.Write);
        binaryWriter_ = new BinaryWriter(fileStream_);

        encoder.onEncoded.AddListener(OnEncoded);
    }

    void OnApplicationQuit()
    {
        if (fileStream_ != null) 
        {
            fileStream_.Close();
        }

        if (binaryWriter_ != null) 
        {
            binaryWriter_.Close();
        }
    }

    void OnEncoded(System.IntPtr ptr, int size)
    {
        var bytes = new byte[size];
        Marshal.Copy(ptr, bytes, 0, size);
        binaryWriter_.Write(bytes);
    }
}

}
