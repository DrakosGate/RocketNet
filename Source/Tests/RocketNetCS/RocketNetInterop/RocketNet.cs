using System.Runtime.InteropServices;

namespace RocketNet;

public class RocketNetInstance
{
	private IntPtr _self;

	public RocketNetInstance()
	{
		_self = RocketNetNative.abi_GetInstance();
	}

	public bool StartHost(ushort Port, string Password)
	{
		return RocketNetNative.abi_StartHost(_self, Port, Marshal.StringToHGlobalAnsi(Password));
	}

	public bool StartClient(string address, ushort Port, string Password)
	{
		return RocketNetNative.abi_StartClient(_self, Marshal.StringToHGlobalAnsi(address), Port, Marshal.StringToHGlobalAnsi(Password));
	}

	public bool EndConnection()
	{
		return RocketNetNative.abi_EndConnection(_self);
	}
	public bool IsConnected()
	{
		return RocketNetNative.abi_IsConnected(_self);
	}
	public bool ProcessPendingHostPackets()
	{
		return RocketNetNative.abi_ProcessPendingHostPackets(_self);
	}
	public bool ProcessPendingClientPackets()
	{
		return RocketNetNative.abi_ProcessPendingClientPackets(_self);
	}
	/*
	static extern void HandleNewRocketNetPacket();
	*/
	public void SendDataToConnection(int ConnectionIndex, IntPtr Data, bool bReliable, int PacketID)
	{
		RocketNetNative.abi_SendDataToConnection(_self, ConnectionIndex, Data, bReliable, PacketID);
	}
	public void SendDataToAllConnections(IntPtr Data, bool bReliable, int PacketID)
	{
		RocketNetNative.abi_SendDataToAllConnections(_self, Data, bReliable, PacketID);
	}
	public void SendDataToHost(IntPtr Data, bool bReliable, int PacketID)
	{
		RocketNetNative.abi_SendDataToHost(_self, Data, bReliable, PacketID);
	}
	
	/*
	public void SendDataToAllConnections(byte* Data, bool bReliable, int PacketID)
	{
		SendDataToAllConnections(Cast("RocketNet"), Data, bReliable, PacketID);
	}
	public void SendDataToHost(IntPtr Data, bool bReliable, int PacketID)
	{
		abi_SendDataToHost(Cast("RocketNet"), Data, bReliable, PacketID);
	}
	 */
}