namespace uPacketFragmentation
{

public class Assembler : System.IDisposable
{
    public uint id { get; private set; }

    public Assembler()
    {
        id = Lib.Assembler.Create();
    }

    ~Assembler()
    {
        Dispose();
    }

    public void Dispose()
    {
        Lib.Assembler.Destroy(id);
    }

    public bool isValid
    {
        get { return Lib.Assembler.IsValid(id); }
    }

    public void AddData(System.IntPtr data, uint size)
    {
        Lib.Assembler.AddData(id, data, size);
    }

    public PacketEventType GetPacketEventType()
    {
        return Lib.Assembler.GetPacketEventType(id);
    }

    public PacketLossType GetPacketLossType()
    {
        return Lib.Assembler.GetPacketLossType(id);
    }

    public ulong GetAssembledFrameIndex()
    {
        return Lib.Assembler.GetAssembledFrameIndex(id);
    }

    public System.IntPtr GetFrameData(ulong index)
    {
        return Lib.Assembler.GetFrameData(id, index);
    }

    public uint GetFrameSize(ulong index)
    {
        return Lib.Assembler.GetFrameSize(id, index);
    }

    public void RemoveFrame(ulong index)
    {
        Lib.Assembler.RemoveFrame(id, index);
    }
}

}
