using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.Rendering;

namespace uNvPipe
{

[RequireComponent(typeof(Renderer))]
public class uNvPipeDecodedTexture : MonoBehaviour
{
    [SerializeField]
    uNvPipeDecoder decoder = null;

    Texture2D texture_;
    CommandBuffer cb_;

    void OnEnable()
    {
        Assert.IsNotNull(decoder, "Please set decoder.");
        decoder.onDecoded.AddListener(OnDecoded);

        cb_ = new CommandBuffer();
        cb_.name = "uNvPipeDecodedTexture" + decoder.id;
    }

    void OnDisable()
    {
        cb_.Dispose();
    }

    void CreateTexture()
    {
        texture_ = new Texture2D(decoder.width, decoder.height, TextureFormat.RGBA32, false, false);

        var renderer = GetComponent<Renderer>();
        renderer.material.mainTexture = texture_;
    }

    void Update()
    {
        var width = decoder.width;
        var height = decoder.height;
        if (!texture_ || texture_.width != width || texture_.height != height)
        {
            CreateTexture();
        }
    }

    void OnDecoded(System.IntPtr ptr, int size)
    {
        if (!texture_ || !isActiveAndEnabled) return;

        var callback = Lib.GetTextureUpdateCallback();
        if (callback == null) return;

        cb_.IssuePluginCustomTextureUpdateV2(callback, texture_, (uint)decoder.id);
        Graphics.ExecuteCommandBuffer(cb_); 
        cb_.Clear();
    }
}

}
