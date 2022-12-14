using System.Runtime.InteropServices;
using System.Text;

namespace RocketNet;

[StructLayout(LayoutKind.Sequential)]
public struct RocketNetPacket
{
	public Int32 dataLength;
	public IntPtr data;
	public Int32 packetID;
	public long senderGUID;
};

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

	public Int64[] GetConnectionGUIDs()
	{
		Int32 numConnections = 0;
		IntPtr firstConnectionPtr = IntPtr.Zero;
		RocketNetNative.abi_GetConnectionGUIDs(_self, ref numConnections, ref firstConnectionPtr);

		Int64[] outConnectionGUIDs = new Int64[numConnections];
		if (numConnections > 0 && firstConnectionPtr != IntPtr.Zero)
		{
			for (int i = 0; i < numConnections; ++i)
			{
				outConnectionGUIDs[i] = Marshal.ReadInt64(firstConnectionPtr);
			}
		}

		return outConnectionGUIDs;
	}

	// --------------------------------------------------------------------------------------------
	// Receive packets
	// --------------------------------------------------------------------------------------------

	// This function checks for any unhandled pending packets and returns true if unhandled packets are found
	public bool CollectPendingPackets()
	{
		var methodResult = RocketNetNative.abi_CollectPendingPackets(_self);
		return methodResult;
	}
	// This function processes all packets and returns an array of unhandled packets which should be handled manually by this connection
	public bool HandleNextPendingPacket(ref RocketNetPacket unhandledPacket)
	{
		IntPtr unhandledPacketPtr = RocketNetNative.abi_HandleNextPendingPacket(_self);
		if (unhandledPacketPtr != IntPtr.Zero)
		{
			unhandledPacket = Marshal.PtrToStructure<RocketNetPacket>(unhandledPacketPtr);
			return true;
		}

		return false;
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
	public void SendDataToConnection(long ConnectionGUID, IntPtr data, bool bReliable, int packetID)
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