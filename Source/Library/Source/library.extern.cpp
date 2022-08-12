#include "library.h"

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
	CSHARP ROCKETNET_API const char* CSHARP_RocketNet_StartHost(RocketNetInstance* object, const unsigned short port, const char* password)
	{
		auto methodResult = object->StartHost(port, password);
		return methodResult;
	}
	CSHARP ROCKETNET_API const char* CSHARP_RocketNet_StartClient(RocketNetInstance* object, const char* address, const unsigned short port, const char* password)
	{
		auto methodResult = object->StartClient(address, port, password);
		return methodResult;
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
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_IsHost(RocketNetInstance* object)
	{
		auto methodResult = object->IsHost();
		return methodResult;
	}
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_GetConnectionGUIDs(RocketNetInstance* object, int32_t& numConnections, ConnectionGUID*& firstGUID)
	{
		auto connectionGUIDs = object->GetConnectionGUIDs();
		numConnections = connectionGUIDs.size();
		if (numConnections > 0)
		{
			firstGUID = &connectionGUIDs[0];
			return true;
		}

		return false;
	}

	// Receive packets
	CSHARP ROCKETNET_API bool CSHARP_RocketNet_CollectPendingPackets(RocketNetInstance* object)
	{
		auto methodResult = object->CollectPendingPackets();
		return methodResult;
	}
	CSHARP ROCKETNET_API TPendingRocketNetPacket* CSHARP_RocketNet_HandleNextPendingPacket(RocketNetInstance* object)
	{
		return object->HandleNextPendingPacket();
	}
	CSHARP ROCKETNET_API void CSHARP_RocketNet_ClearPendingPackets(RocketNetInstance* object)
	{
		object->ClearPendingPackets();
	}

	// Send packets
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToConnection(RocketNetInstance* object, ConnectionGUID connectionGUID, const char* data, bool bReliable, int packetID)
	{
		object->SendDataToConnection(connectionGUID, data, bReliable, packetID);
	}
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToAllConnections(RocketNetInstance* object, const char* data, bool bReliable, int packetID)
	{
		object->SendDataToAllConnections(data, bReliable, packetID);
	}
	CSHARP ROCKETNET_API void CSHARP_RocketNet_SendDataToHost(RocketNetInstance* object, const char* data, bool bReliable, int packetID)
	{
		object->SendDataToHost(data, bReliable, packetID);
	}
}