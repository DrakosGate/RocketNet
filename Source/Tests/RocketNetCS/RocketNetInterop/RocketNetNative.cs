namespace RocketNet;

using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct RocketNetPacket
{
	public Int32 dataLength;
	// SizeParamIndex=0 means it will use the first variable (dataLength) as the size of this data char array
	[MarshalAs(UnmanagedType.LPStr, SizeParamIndex=0)]  
	public char[] data;
	public Int32 packetID;
	public Int64 senderGUID;
};

public static class RocketNetNative
{
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_GetInstance")]
	public static extern IntPtr abi_GetInstance();

	// Connection management functions
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_StartHost")]
	public static extern IntPtr abi_StartHost(IntPtr inSelf, ushort Port, IntPtr Password);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_StartClient")]
	public static extern IntPtr abi_StartClient(IntPtr inSelf, IntPtr address, ushort Port, IntPtr Password);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_EndConnection")]
	public static extern bool abi_EndConnection(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_IsConnected")]
	public static extern bool abi_IsConnected(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_IsHost")]
	public static extern bool abi_IsHost(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_GetConnectionGUIDs")]
	public static extern bool abi_GetConnectionGUIDs(IntPtr inSelf, ref Int32 numConnections, ref IntPtr firstConnection);

	// Receive packets
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_FetchPendingPackets")]
	public static extern bool abi_FetchPendingPackets(IntPtr inSelf, ref Int32 numPendingPackets, ref IntPtr firstPacket);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_ClearPendingPackets")]
	public static extern void abi_ClearPendingPackets(IntPtr inSelf);

	// Send packets
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToConnection")]
	public static extern void abi_SendDataToConnection(IntPtr inSelf, UInt64 connectionGUID, IntPtr data, bool bReliable, int packetID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToAllConnections")]
	public static extern void abi_SendDataToAllConnections(IntPtr inSelf, IntPtr data, bool bReliable, int packetID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToHost")]
	public static extern void abi_SendDataToHost(IntPtr inSelf, IntPtr data, bool bReliable, int packetID);

}