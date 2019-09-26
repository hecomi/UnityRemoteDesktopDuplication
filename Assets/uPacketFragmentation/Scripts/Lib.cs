using System.Runtime.InteropServices;

namespace uPacketFragmentation
{ 

public enum PacketEventType
{
    None = -1,
    FrameCompleted = 0,
    PacketLoss = 1,
};

public enum PacketLossType
{
    None = -1,
    NotReceived = 0,
    Timeout = 1,
};

public static class Lib
{
    public const string dllName = "uPacketFragmentation";

    public static class Fragmenter
    {
        [DllImport(dllName, EntryPoint = "UpfCreateFragmenter")]
        public static extern uint Create();

        [DllImport(dllName, EntryPoint = "UpfDestroyFragmenter")]
        public static extern void Destroy(uint id);

        [DllImport(dllName, EntryPoint = "UpfFragmenterIsValid")]
        public static extern bool IsValid(uint id);

        [DllImport(dllName, EntryPoint = "UpfFragmenterSetMaxPacketSize")]
        public static extern void SetMaxPacketSize(uint id, uint size);

        [DllImport(dllName, EntryPoint = "UpfFragmenterFragmentData")]
        public static extern void FragmentData(uint id, System.IntPtr data, uint size);

        [DllImport(dllName, EntryPoint = "UpfFragmenterGetFragmentCount")]
        public static extern uint GetFragmentCount(uint id);

        [DllImport(dllName, EntryPoint = "UpfFragmenterGetFragmentData")]
        public static extern System.IntPtr GetFragmentData(uint id, uint index);

        [DllImport(dllName, EntryPoint = "UpfFragmenterGetFragmentSize")]
        public static extern uint GetFragmentSize(uint id, uint index);
    }

    public static class Assembler
    {
        [DllImport(dllName, EntryPoint = "UpfCreateAssembler")]
        public static extern uint Create();

        [DllImport(dllName, EntryPoint = "UpfDestroyAssembler")]
        public static extern void Destroy(uint id);

        [DllImport(dllName, EntryPoint = "UpfAssemblerIsValid")]
        public static extern bool IsValid(uint id);

        [DllImport(dllName, EntryPoint = "UpfAssemblerAddData")]
        public static extern void AddData(uint id, System.IntPtr data, uint size);

        [DllImport(dllName, EntryPoint = "UpfAssemblerGetPacketEventType")]
        public static extern PacketEventType GetPacketEventType(uint id);

        [DllImport(dllName, EntryPoint = "UpfAssemblerGetPacketLossType")]
        public static extern PacketLossType GetPacketLossType(uint id);

        [DllImport(dllName, EntryPoint = "UpfAssemblerGetAssembledFrameIndex")]
        public static extern ulong GetAssembledFrameIndex(uint id);

        [DllImport(dllName, EntryPoint = "UpfAssemblerGetFrameData")]
        public static extern System.IntPtr GetFrameData(uint id, ulong index);

        [DllImport(dllName, EntryPoint = "UpfAssemblerGetFrameSize")]
        public static extern uint GetFrameSize(uint id, ulong index);

        [DllImport(dllName, EntryPoint = "UpfAssemblerRemoveFrame")]
        public static extern void RemoveFrame(uint id, ulong index);
    }
}

}
