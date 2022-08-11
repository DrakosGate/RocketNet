#include "library.h"

#include <assert.h>

extern "C"
{
	/**
	 * @summary implements the passthrough function for getId to make it callable from C#
	 * @from generation_tool\interop\elements\members\callable\member_function.py:379
	 */
	CSHARP ROCKETNET_API RocketNetInstance* CSHARP_RocketNet_GetInstance(RocketNetInstance* object)
	{
		auto methodResult = &object->GetInstance();
		return methodResult;
	}

	// Open and close connection
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_StartHost(RocketNetInstance* object, const unsigned short Port, const char* Password)
	{
		auto methodResult = object->StartHost(Port, Password);
		return methodResult.length() > 0;
	}
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_StartClient(RocketNetInstance* object, const char* Address, const unsigned short Port, const char* Password)
	{
		auto methodResult = object->StartClient(Address, Port, Password);
		return methodResult.length() > 0;
	}
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_EndConnection(RocketNetInstance* object)
	{
		auto methodResult = object->EndConnection();
		return methodResult;
	}
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_IsConnected(RocketNetInstance* object)
	{
		auto methodResult = object->IsConnected();
		return methodResult;
	}

	// Receive packets
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_ProcessPendingHostPackets(RocketNetInstance* object)
	{
		auto methodResult = object->ProcessPendingHostPackets();
		return methodResult;
	}
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_ProcessPendingClientPackets(RocketNetInstance* object)
	{
		auto methodResult = object->ProcessPendingClientPackets();
		return methodResult;
	}
	/*
	CSHARP ROCKETNET_API void CSHARP_RocketNet_OnRocketNetMessageReceived(RocketNetInstance* object, RakNet::Packet* Packet)
	{
		object->OnRocketNetMessageReceived(Packet);
	}
	*/

	// Send packets
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToConnection(RocketNetInstance* object, int ConnectionIndex, const char* Data, bool bReliable, int PacketID)
	{
		object->SendDataToConnection(ConnectionIndex, Data, bReliable, PacketID);
	}
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToAllConnections(RocketNetInstance* object, const char* Data, bool bReliable, int PacketID)
	{
		object->SendDataToAllConnections(Data, bReliable, PacketID);
	}
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToHost(RocketNetInstance* object, const char* Data, bool bReliable, int PacketID)
	{
		object->SendDataToHost(Data, bReliable, PacketID);
	}
}