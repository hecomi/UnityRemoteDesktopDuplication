using UnityEngine;
using uOSC;
using uPacketDivision;

namespace UnityRemoteDesktopDuplication
{

public class DesktopSender : MonoBehaviour
{
    public uOscClient client;
    public DesktopEncoder encoder;
    public uint maxPacketSize = 1400;
    public uNvEncoder.EncoderDesc desc { get; set; }
    Divider divider_ = new Divider();

    public void OnEncoded(System.IntPtr data, int size)
    {
        var width = encoder.setting.width;
        var height = encoder.setting.height;
        client.Send("/uDD/Size", width, height);

        divider_.maxPacketSize = maxPacketSize;
        divider_.Divide(data, (uint)size);
        for (uint i = 0; i < divider_.GetChunkCount(); ++i)
        {
            client.Send("/uDD/Data", divider_.GetChunk(i));
        }
    }
}

}
