using UnityEngine;
using System.Runtime.InteropServices;
using uOSC;
using uPacketFragmentation;

namespace UnityRemoteDesktopDuplication
{

public class DesktopSender : MonoBehaviour
{
    public uOscClient client;
    public DesktopEncoder encoder;
    Fragmenter fragmenter_ = new Fragmenter();

    public uNvEncoder.EncoderDesc desc { get; set; }

    public int maxPacketSize = 1400;

    void Start()
    {
        fragmenter_.maxPacketSize = maxPacketSize;
    }

    public void OnEncoded(System.IntPtr data, int size)
    {
        var width = encoder.setting.width;
        var height = encoder.setting.height;
        client.Send("/uDD/ScreenSize", width, height);

        fragmenter_.Fragment(data, (uint)size);
        var n = fragmenter_.GetFragmentCount();
        for (uint i = 0; i < n; ++i)
        {
            var fragmentData = fragmenter_.GetFragmentData(i);
            if (fragmentData == System.IntPtr.Zero) return;
            var fragmentSize = (int)fragmenter_.GetFragmentSize(i);
            byte[] buf = new byte[fragmentSize];
            Marshal.Copy(fragmentData, buf, 0, fragmentSize);
            client.Send("/uDD/Fragment", buf);
        }
    }
}

}
