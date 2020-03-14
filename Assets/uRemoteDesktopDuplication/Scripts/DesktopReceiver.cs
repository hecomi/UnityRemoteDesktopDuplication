using UnityEngine;
using System.Runtime.InteropServices;
using uOSC;
using uPacketFragmentation;

namespace UnityRemoteDesktopDuplication
{

public class DesktopReceiver : MonoBehaviour
{
    public uOscServer server;
    public uNvPipe.uNvPipeDecoder decoder;
    public uNvEncoder.Examples.OutputDataToFile output;
    Assembler assembler_ = new Assembler();
    bool isInitialized_ = false;

    void OnEnable()
    {
        if (server)
        {
            server.onDataReceived.AddListener(OnDataReceived);
        }
    }

    void OnDisable()
    {
        if (server)
        {
            server.onDataReceived.RemoveListener(OnDataReceived);
        }
    }

    public void OnDataReceived(Message message)
    {
        if (message.address == "/uDD/ScreenSize" && !decoder.isActiveAndEnabled)
        {
            var width = (int)message.values[0];
            var height = (int)message.values[1];
            SetScreenSize(width, height);
        }
        else if (message.address == "/uDD/Fragment")
        {
            var data = (byte[])message.values[0];
            AddFragmentData(data);
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

    void AddFragmentData(byte[] data)
    {
        var handle = GCHandle.Alloc(data, GCHandleType.Pinned);
        var ptr = handle.AddrOfPinnedObject();
        var size = (uint)data.Length;
        assembler_.AddData(ptr, size);
        handle.Free();
    }

    void CheckPacketEvent()
    {
        if (!isInitialized_) return;

        switch (assembler_.GetPacketEventType())
        {
            case PacketEventType.FrameCompleted:
            {
                var index = assembler_.GetAssembledFrameIndex();
                var data = assembler_.GetFrameData(index);
                var size = assembler_.GetFrameSize(index);
                decoder.Decode(data, (int)size);
                if (output) output.OnData(data, (int)size);
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
