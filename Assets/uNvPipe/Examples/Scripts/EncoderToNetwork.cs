using UnityEngine;
using UnityEngine.Assertions;
using System.Runtime.InteropServices;

namespace uNvPipe.Examples
{

public class EncoderToNetwork : MonoBehaviour
{
    [SerializeField]
    uOSC.uOscClient oscClient = null;

    [SerializeField]
    uNvPipeEncoder encoder = null;

    void OnEnable()
    {
        Assert.IsNotNull(oscClient, "Please set osc client to EncoderToNetwork.");
        Assert.IsNotNull(encoder, "Please set encoder to EncoderToNetwork.");

        if (encoder)
        {
            encoder.onEncoded.AddListener(OnEncoded);
        }
    }

    void OnDisable()
    {
        if (encoder)
        {
            encoder.onEncoded.RemoveListener(OnEncoded);
        }
    }

    void OnEncoded(System.IntPtr ptr, int size)
    {
        if (oscClient)
        {
            var buf = new byte[size];
            Marshal.Copy(ptr, buf, 0, size);
            oscClient.Send("/uNvPipe/Data", buf);
        }
    }
}

}
