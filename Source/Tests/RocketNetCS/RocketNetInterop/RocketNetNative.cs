namespace RocketNet;

using System.Runtime.InteropServices;

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
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_CollectPendingPackets")]
	public static extern bool abi_CollectPendingPackets(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_HandleNextPendingPacket")]
	public static extern IntPtr abi_HandleNextPendingPacket(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_ClearPendingPackets")]
	public static extern void abi_ClearPendingPackets(IntPtr inSelf);

	// Send packets
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToConnection")]
	public static extern void abi_SendDataToConnection(IntPtr inSelf, long connectionGUID, IntPtr data, bool bReliable, int packetID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToAllConnections")]
	public static extern void abi_SendDataToAllConnections(IntPtr inSelf, IntPtr data, bool bReliable, int packetID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToHost")]
	public static extern void abi_SendDataToHost(IntPtr inSelf, IntPtr data, bool bReliable, int packetID);

}