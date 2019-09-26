using UnityEngine;
using System.Runtime.InteropServices;
using uOSC;
using uPacketFragmentation;

namespace UnityRemoteDesktopDuplication
{

public class DesktopSender : MonoBehaviour
{
    public uOscClient client;
    Fragmenter fragmenter_ = new Fragmenter();

    public int maxPacketSize = 1400;

    void Start()
    {
        fragmenter_.maxPacketSize = maxPacketSize;
    }

    public void OnEncoded(System.IntPtr data, int size)
    {
        fragmenter_.Fragment(data, (uint)size);
        var n = fragmenter_.GetFragmentCount();
        for (uint i = 0; i < n; ++i)
        {
            var fragmentData = fragmenter_.GetFragmentData(i);
            var fragmentSize = (int)fragmenter_.GetFragmentSize(i);
            byte[] buf = new byte[fragmentSize];
            Marshal.Copy(fragmentData, buf, 0, fragmentSize);
            client.Send("/texture/fragment", buf);
        }
    }
}

}
