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
	TConnectionInfo(RakNet::SystemAddress& Address, ConnectionGUID ConnectionGUID)
		: address(Address), connectionGUID(ConnectionGUID)
		{}
	RakNet::SystemAddress address;
	ConnectionGUID connectionGUID = InvalidGUID;
	std::string nameOverride;
};

struct TPendingRocketNetPacket
{
	TPendingRocketNetPacket(const char* Data, int PacketID, ConnectionGUID SenderGUID)
		: data(Data), packetID(PacketID), senderGUID(SenderGUID)
		{}
	const char* data = nullptr;
	int packetID = 0;
	ConnectionGUID senderGUID = 0;
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
	std::string StartHost(uint16_t Port, const char* Password);
	std::string StartClient(const char* Address, uint16_t Port, const char* Password);
	bool EndConnection();
	[[nodiscard]] bool IsConnected() const { return bConnectionSuccess; }
	[[nodiscard]] bool IsHost() const { return bIsHost && bConnectionSuccess; }
	std::vector<ConnectionGUID> GetConnectionGUIDs() const;

	// Receive packets
	bool ProcessPendingHostPackets();
	bool ProcessPendingClientPackets();
	void HandleNewRocketNetPacket(RakNet::Packet* Packet);
	std::vector<TPendingRocketNetPacket>& FetchPendingPackets();
	void ClearPendingPackets(){ pendingPackets.clear(); }

	// Send packets
	void SendDataToConnection(const ConnectionGUID& connectionGUID, const char* Data, bool bReliable, int PacketID);
	void SendDataToAllConnections(const char* Data, bool bReliable, int PacketID);
	void SendDataToHost(const char* Data, bool bReliable, int PacketID);

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
	void SendDataToConnection(const RakNet::SystemAddress& Address, const char* Data, bool bReliable, int PacketID);

	RakNet::RakPeerInterface *peer = nullptr;
	std::vector<TConnectionInfo> connections;
	std::vector<TPendingRocketNetPacket> pendingPackets;
	bool bConnectionSuccess = false;
	bool bIsHost = false;

};

#endif //RAKNET_LIBRARY_H
