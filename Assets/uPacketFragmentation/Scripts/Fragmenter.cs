namespace uPacketFragmentation
{

public class Fragmenter : System.IDisposable
{
    public uint id { get; private set; }

    public Fragmenter()
    {
        id = Lib.Fragmenter.Create();
    }

    ~Fragmenter()
    {
        Dispose();
    }

    public void Dispose()
    {
        Lib.Fragmenter.Destroy(id);
    }

    public bool isValid
    {
        get { return Lib.Fragmenter.IsValid(id); }
    }

    public int maxPacketSize
    {
        set
        {
            Lib.Fragmenter.SetMaxPacketSize(id, (uint)value);
        }
    }

    public void Fragment(System.IntPtr data, uint size)
    {
        Lib.Fragmenter.FragmentData(id, data, size);
    }

    public uint GetFragmentCount()
    {
        return Lib.Fragmenter.GetFragmentCount(id);
    }

    public System.IntPtr GetFragmentData(uint index)
    {
        return Lib.Fragmenter.GetFragmentData(id, index);
    }

    public uint GetFragmentSize(uint index)
    {
        return Lib.Fragmenter.GetFragmentSize(id, index);
    }
}

}
