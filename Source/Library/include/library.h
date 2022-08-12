#ifndef RAKNET_LIBRARY_H
#define RAKNET_LIBRARY_H

#include <vector>
#include <string>
#include "MessageIdentifiers.h"
#include "RocketNetApi.h"
#include "RakNet/RakNetTypes.h"

// Todo: Hard-coded default stuff to remove
#define DefaultServerAddress "127.0.0.1"
#define DefaultServerPort 60000
#define MaxClients 4
typedef uint64_t ConnectionGUID;
#define InvalidGUID ((ConnectionGUID)-1)

// Todo: This should be controlled by the plugin?
enum ERocketNetReservedMessageIDs
{
	RocketNetReservedIDStart = ID_USER_PACKET_ENUM,
	RocketNetSendMessage,
	RocketNetChangeName
};

struct TConnectionInfo
{
	TConnectionInfo(){}
	TConnectionInfo(RakNet::SystemAddress& address, ConnectionGUID ConnectionGUID)
		: address(address), connectionGUID(ConnectionGUID)
		{}
	RakNet::SystemAddress address;
	ConnectionGUID connectionGUID = InvalidGUID;
	std::string nameOverride;
};

struct TPendingRocketNetPacket
{
	TPendingRocketNetPacket(const char* data, int32_t dataLength, int32_t packetID, ConnectionGUID senderGUID)
		: data(data), dataLength(dataLength), packetID(packetID), senderGUID(senderGUID)
		{}

	// Don't change the order of variables in this struct - it's important that dataLength is element 0
	int32_t dataLength = 0;
	const char* data = nullptr;
	int32_t packetID = 0;
	ConnectionGUID senderGUID = InvalidGUID;
};

class ROCKETNET_API RocketNetInstance
{
public:
	// Singleton instance
	static RocketNetInstance& GetInstance()
	{
		static RocketNetInstance sInstance;
		return sInstance;
	}

	// Open and close connection
	const char* StartHost(uint16_t port, const char* password);
	const char* StartClient(const char* address, uint16_t port, const char* password);
	bool EndConnection();
	[[nodiscard]] bool IsConnected() const { return bConnectionSuccess; }
	[[nodiscard]] bool IsHost() const { return bIsHost && bConnectionSuccess; }
	std::vector<ConnectionGUID> GetConnectionGUIDs() const;

	// Receive packets
	std::vector<TPendingRocketNetPacket>& FetchPendingPackets();
	void ClearPendingPackets(){ pendingPackets.clear(); }

	// Send packets
	void SendDataToConnection(const ConnectionGUID& connectionGUID, const char* data, bool bReliable, int packetID);
	void SendDataToAllConnections(const char* data, bool bReliable, int packetID);
	void SendDataToHost(const char* data, bool bReliable, int packetID);

	// Used for testing
	std::string GetNameForConnection(const ConnectionGUID& connectionGUID) const;
	void HostSetConnectionName(const ConnectionGUID& connectionGUID, const std::string& name);

public:
	// Singleton protection
	RocketNetInstance(const RocketNetInstance&) = delete;
	RocketNetInstance& operator=(const RocketNetInstance &) = delete;
	RocketNetInstance(RocketNetInstance &&) = delete;
	RocketNetInstance & operator=(RocketNetInstance &&) = delete;

protected:
	RocketNetInstance(){}
	[[nodiscard]] const TConnectionInfo* GetConnectionInfoForGUID(const ConnectionGUID& connectionGuid) const;
	void SendDataToConnection(const RakNet::SystemAddress& address, const char* data, bool bReliable, int packetID);
	void HandleNewRocketNetPacket(RakNet::Packet* packet);

	RakNet::RakPeerInterface *peer = nullptr;
	std::vector<TConnectionInfo> connections;
	std::vector<TPendingRocketNetPacket> pendingPackets;
	bool bConnectionSuccess = false;
	bool bIsHost = false;

};

#endif //RAKNET_LIBRARY_H
