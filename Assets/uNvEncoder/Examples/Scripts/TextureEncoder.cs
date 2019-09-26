using UnityEngine;
using UnityEngine.Assertions;
using System.Collections;

namespace uNvEncoder.Examples
{

public class TextureEncoder : MonoBehaviour
{
    public Encoder encoder = new Encoder();
    public Texture texture = null;
    public EncoderDesc setting;
    public bool forceIdrFrame = true;

    void OnEnable()
    {
        Assert.IsNotNull(texture);
        setting.width = texture.width;
        setting.height = texture.height;
        encoder.Create(setting);
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
            yield return new WaitForEndOfFrame();
            Encode();
        }
    }

    void Encode()
    {
        if (!texture) return;

        encoder.Update();
        encoder.Encode(texture, forceIdrFrame);
    }
}

}
