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
    uNvEncoder.EncoderDesc setting = new uNvEncoder.EncoderDesc
    {
        width = 1920,
        height = 1080,
        frameRate = 60,
        format = uNvEncoder.Format.B8G8R8A8_UNORM,
        bitRate = 2000000,
        maxFrameSize = 40000,
    };

    public bool forceIdrFrame = false;

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

        for (int i = 0; true; ++i)
        {
            yield return new WaitForEndOfFrame();
            if (i % 2 == 0)
            {
                encoder.Update();
                encoder.Encode(texture.monitor.texture, forceIdrFrame);
            }
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
