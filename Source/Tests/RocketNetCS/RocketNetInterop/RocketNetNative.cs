namespace RocketNet;

using System.Runtime.InteropServices;

public static unsafe class RocketNetNative
{
	
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_GetInstance")]
	public static extern IntPtr abi_GetInstance();
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_StartHost")]
	public static extern bool abi_StartHost(IntPtr inSelf, ushort Port, IntPtr Password);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_StartClient")]
	public static extern bool abi_StartClient(IntPtr inSelf, IntPtr address, ushort Port, IntPtr Password);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_EndConnection")]
	public static extern bool abi_EndConnection(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_IsConnected")]
	public static extern bool abi_IsConnected(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_ProcessPendingHostPackets")]
	public static extern bool abi_ProcessPendingHostPackets(IntPtr inSelf);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_ProcessPendingClientPackets")]
	public static extern bool abi_ProcessPendingClientPackets(IntPtr inSelf);
	/*
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_OnRocketNetMessageReceived")]
	static extern void abi_OnRocketNetMessageReceived(IntPtr inSelf);
	*/
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToConnection")]
	public static extern void abi_SendDataToConnection(IntPtr inSelf, int ConnectionIndex, IntPtr Data, bool bReliable, int PacketID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToAllConnections")]
	public static extern void abi_SendDataToAllConnections(IntPtr inSelf, IntPtr Data, bool bReliable, int PacketID);
	[DllImport(@"RocketNet", EntryPoint = "CSHARP_RocketNet_SendDataToHost")]
	public static extern void abi_SendDataToHost(IntPtr inSelf, IntPtr Data, bool bReliable, int PacketID);

}