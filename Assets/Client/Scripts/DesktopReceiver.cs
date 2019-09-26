using UnityEngine;
using UnityEngine.Events;
using System.Runtime.InteropServices;
using uOSC;
using uPacketFragmentation;

namespace UnityRemoteDesktopDuplication
{

public class DesktopReceiver : MonoBehaviour
{
    public uOscServer server;
    Assembler assembler_ = new Assembler();

    [System.Serializable]
    public class FrameEvent : UnityEvent<System.IntPtr, int> {};
    public FrameEvent onFrame = new FrameEvent();

    public void OnDataReceived(Message message)
    {
        var address = message.address;
        if (address == "/texture/fragment")
        {
            var data = (byte[])message.values[0];
            var handle = GCHandle.Alloc(data, GCHandleType.Pinned);
            var ptr = handle.AddrOfPinnedObject();
            var size = (uint)data.Length;
            assembler_.AddData(ptr, size);
            handle.Free();
        }

        switch (assembler_.GetPacketEventType())
        {
            case PacketEventType.FrameCompleted:
            {
                var index = assembler_.GetAssembledFrameIndex();
                var data = assembler_.GetFrameData(index);
                var size = assembler_.GetFrameSize(index);
                if (onFrame != null) onFrame.Invoke(data, (int)size);
                assembler_.RemoveFrame(index);
                break;
            }
            case PacketEventType.PacketLoss:
            {
                // TODO: IDR フレーム要求
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
