using UnityEngine;
using System.Collections;

namespace UnityRemoteDesktopDuplication
{

public class DesktopEncoder : MonoBehaviour
{
    [SerializeField]
    uDesktopDuplication.Texture texture = null;

    [SerializeField]
    uNvEncoder.Encoder encoder = null;

    [SerializeField]
    public uNvEncoder.EncoderDesc setting = new uNvEncoder.EncoderDesc
    {
        width = 1920,
        height = 1080,
        frameRate = 60,
        format = uNvEncoder.Format.B8G8R8A8_UNORM,
        bitRate = 2000000,
        maxFrameSize = 40000,
    };

    public int idrFrameIntervalFrame = 60;
    int idrFrameCounter_ = 0;

    void OnEnable()
    {
        StartCoroutine(EncodeLoop());
    }

    void OnDisable()
    {
        StopAllCoroutines();
        encoder.Destroy();
    }

    IEnumerator EncodeLoop()
    {
        for (;;)
        {
            if (texture.monitor != null) break;
            yield return new WaitForEndOfFrame();
        }

        setting.width = texture.monitor.width;
        setting.height = texture.monitor.height;
        encoder.Create(setting);

        for (;;)
        {
            if (setting.frameRate < 60)
            {
                yield return new WaitForSeconds(1f / setting.frameRate);
            }

            bool idr = idrFrameCounter_++ % idrFrameIntervalFrame == 0;
            encoder.Encode(texture.monitor.texture, idr);
            encoder.Update();
        }
    }

    [ContextMenu("Reconfigure")]
    public void Reconfigure()
    {
        if (encoder == null) return;
        encoder.Reconfigure(setting);
    }
}

}
