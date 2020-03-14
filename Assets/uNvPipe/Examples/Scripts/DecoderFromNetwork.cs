using UnityEngine;
using UnityEngine.Assertions;
using System.Runtime.InteropServices;

namespace uNvPipe.Examples
{

public class DecoderFromNetwork : MonoBehaviour
{
    [SerializeField]
    uOSC.uOscServer oscServer = null;

    [SerializeField]
    uNvPipeDecoder decoder = null;

    void OnEnable()
    {
        Assert.IsNotNull(oscServer, "Please set osc client to DecoderToNetwork.");
        Assert.IsNotNull(decoder, "Please set decoder to DecoderToNetwork.");

        if (oscServer)
        {
            oscServer.onDataReceived.AddListener(OnDataReceived);
        }
    }

    void OnDisable()
    {
        if (oscServer)
        {
            oscServer.onDataReceived.RemoveListener(OnDataReceived);
        }
    }

    void OnDataReceived(uOSC.Message message)
    {
        if (message.address != "/uNvPipe/Data") return;

        var buf = (byte[])message.values[0];
        var pinnedBuf = GCHandle.Alloc(buf, GCHandleType.Pinned);
        var ptr = pinnedBuf.AddrOfPinnedObject();
        decoder.Decode(ptr, buf.Length);
        pinnedBuf.Free();
    }
}

}
