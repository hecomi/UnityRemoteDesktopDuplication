using UnityEngine;
using uOSC;
using uPacketDivision;
using System.Runtime.InteropServices;

namespace UnityRemoteDesktopDuplication
{

public class DesktopReceiver : MonoBehaviour
{
    public uNvPipe.uNvPipeDecoder decoder;
    public uint timeout = 100;
    Assembler assembler_ = new Assembler();
    bool isInitialized_ = false;

    public void OnDataReceived(Message message)
    {
        assembler_.timeout = timeout;

        if (message.address == "/uDD/Size")
        {
            var width = (int)message.values[0];
            var height = (int)message.values[1];
            SetScreenSize(width, height);
        }
        else if (message.address == "/uDD/Data")
        {
            assembler_.Add((byte[])message.values[0]);
            CheckPacketEvent();
        }
    }

    void SetScreenSize(int width, int height)
    {
        if (isInitialized_) return;

        Debug.LogFormat("Start decoder: width => {0}, height => {0}", width, height);
        decoder.width = width;
        decoder.height = height;
        decoder.enabled = true;

        isInitialized_ = true;
    }

    void CheckPacketEvent()
    {
        if (!isInitialized_) return;

        switch (assembler_.GetEventType())
        {
            case uPacketDivision.EventType.FrameCompleted:
            {
                var data = assembler_.GetAssembledData<byte>();
                int size = data.Length;
                var handle = GCHandle.Alloc(data, GCHandleType.Pinned);
                var ptr = handle.AddrOfPinnedObject();
                decoder.Decode(ptr, (int)size);
                handle.Free();
                break;
            }
            case uPacketDivision.EventType.PacketLoss:
            {
                Debug.LogError("packet loss");
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

}
