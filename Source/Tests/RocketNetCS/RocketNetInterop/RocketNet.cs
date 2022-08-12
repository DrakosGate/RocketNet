using System.Runtime.InteropServices;

namespace RocketNet;

public class RocketNetInstance
{
	private IntPtr _self;

	public RocketNetInstance()
	{
		_self = RocketNetNative.abi_GetInstance();
	}

	// --------------------------------------------------------------------------------------------
	// Connection management functions
	// --------------------------------------------------------------------------------------------

	// This function Hosts a new connection using the Port and optional Password provided
	public string StartHost(ushort Port, string Password)
	{
		var errorMessage = RocketNetNative.abi_StartHost(_self, Port, Marshal.StringToHGlobalAnsi(Password));
		return errorMessage.ToString();
	}
	// This function joins an existing connection using the Address, Port and optional Password provided
	public string StartClient(string address, ushort Port, string Password)
	{
		var errorMessage = RocketNetNative.abi_StartClient(_self, Marshal.StringToHGlobalAnsi(address), Port, Marshal.StringToHGlobalAnsi(Password));
		return errorMessage.ToString();
	}
	// This function ends an existing connection - can be used by Host and Client
	public bool EndConnection()
	{
		return RocketNetNative.abi_EndConnection(_self);
	}
	// This function returns whether or not we are currently connected
	public bool IsConnected()
	{
		return RocketNetNative.abi_IsConnected(_self);
	}
	// This function returns whether or not we are Hosting this connection or joined an existing connection
	public bool IsHost()
	{
		return RocketNetNative.abi_IsHost(_self);
	}

	public bool GetConnectionGUIDs()
	{
		Int32 numConnections = 0;
		IntPtr firstConnectionPtr = IntPtr.Zero;
		var methodResult = RocketNetNative.abi_GetConnectionGUIDs(_self, ref numConnections, ref firstConnectionPtr);
		return methodResult;
	}

	// --------------------------------------------------------------------------------------------
	// Receive packets
	// --------------------------------------------------------------------------------------------

	// This function processes all packets and returns an array of unhandled packets which should be handled manually by this connection
	public bool FetchPendingPackets()
	{
		Int32 numPendingPackets = 0;
		IntPtr firstPacketPtr = IntPtr.Zero;
		var methodResult = RocketNetNative.abi_FetchPendingPackets(_self, ref numPendingPackets, ref firstPacketPtr);
		return methodResult;
	}
	// This function clears the list of pending packets
	public void ClearPendingPackets()
	{
		RocketNetNative.abi_ClearPendingPackets(_self);
	}

	// --------------------------------------------------------------------------------------------
	// Send packets
	// --------------------------------------------------------------------------------------------

	// This function is used to send a packet to a connection using it's 64bit Connection GUID identifier
	public void SendDataToConnection(UInt64 ConnectionGUID, IntPtr data, bool bReliable, int packetID)
	{
		RocketNetNative.abi_SendDataToConnection(_self, ConnectionGUID, data, bReliable, packetID);
	}
	// This function is used to send a packet to all connections
	public void SendDataToAllConnections(IntPtr data, bool bReliable, int packetID)
	{
		RocketNetNative.abi_SendDataToAllConnections(_self, data, bReliable, packetID);
	}
	// This function is used to send a packet to the Host of our connection
	public void SendDataToHost(IntPtr data, bool bReliable, int packetID)
	{
		RocketNetNative.abi_SendDataToHost(_self, data, bReliable, packetID);
	}
}