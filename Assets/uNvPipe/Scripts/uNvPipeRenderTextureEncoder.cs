using UnityEngine;
using UnityEngine.Assertions;

namespace uNvPipe.Examples
{

public class uNvPipeRenderTextureEncoder : MonoBehaviour
{
    enum ForceIframeTiming
    {
        EveryFrame,
        Periodically,
        Manual,
    }

    [SerializeField]
    uNvPipeEncoder encoder = null;

    [SerializeField]
    ForceIframeTiming forceIframeTiming = ForceIframeTiming.Periodically;

    [SerializeField, Tooltip("sec")]
    float forceIframePeriod = 1f;

    [SerializeField]
    RenderTexture texture = null;

    public bool forceIframe { get; set; }

    Texture2D texture2d_;
    float encodeTimer_ = 0f;
    float forceIframeTimer_ = 0f;

    void Start()
    {
        Assert.IsNotNull(encoder, "Please set encoder.");
        Assert.IsNotNull(texture, "Please set texture.");

        if (texture)
        {
            texture2d_ = new Texture2D(
                texture.width,
                texture.height,
                TextureFormat.RGBA32,
                false,
                false);
        }
    }

    void Update()
    {
        if (encoder.fps == Application.targetFrameRate)
        {
            Encode();
        }
        else
        {
            var T = 1f / encoder.fps;
            encodeTimer_ += Time.deltaTime;

            if (encodeTimer_ >= T)
            {
                encodeTimer_ -= T;
                Encode();
            }
        }
    }

    void UpdateIframe()
    {
        switch (forceIframeTiming)
        {
            case ForceIframeTiming.Manual:
                break;
            case ForceIframeTiming.EveryFrame:
                forceIframe = true;
                break;
            case ForceIframeTiming.Periodically:
                if (forceIframeTimer_ <= 0f)
                {
                    forceIframeTimer_ += forceIframePeriod;
                    forceIframe = true;
                }
                else
                {
                    forceIframe = false;
                }
                forceIframeTimer_ -= Time.deltaTime;
                break;
        }
    }

    void Encode()
    {
        if (!texture || !encoder) return;

        if (encoder.width != texture.width ||
            encoder.height != texture.height)
        {
            Debug.LogError("encoder size is different from the given render texture.");
            return;
        }

        UpdateIframe();

		var activeRenderTexture = RenderTexture.active;
		RenderTexture.active = texture;

        var area = new Rect(0f, 0f, texture2d_.width, texture2d_.height);
		texture2d_.ReadPixels(area, 0, 0);
		texture2d_.Apply();

		RenderTexture.active = activeRenderTexture;

        encoder.Encode(texture2d_, forceIframe);
    }
}

}
