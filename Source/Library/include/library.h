#ifndef RAKNET_LIBRARY_H
#define RAKNET_LIBRARY_H

#include <vector>
#include <string>
#include "MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"

// Todo: Hard-coded default stuff to remove
const std::string DefaultServerAddress = "127.0.0.1";
const unsigned short DefaultServerPort = 60000;
const unsigned short MaxClients = 4;

// Todo: This should be controlled by the plugin?
enum ERocketNetReservedMessageIDs
{
	RocketNetReservedIDStart = ID_USER_PACKET_ENUM
};

class RocketNetInstance
{
public:
	// Singleton instance
	static RocketNetInstance& GetInstance()
	{
		static RocketNetInstance Instance;
		return Instance;
	}

	// Open and close connection
	std::string StartHost(const unsigned short Port, const std::string& Password);
	std::string StartClient(const std::string& Address, const unsigned short Port, const std::string& Password);
	bool EndConnection();
	bool IsConnected() const { return ConnectionSuccess; }

	// Receive packets
	bool ProcessPendingHostPackets();
	bool ProcessPendingClientPackets();
	void OnRocketNetMessageReceived(RakNet::Packet* Packet);

	// Send packets
	void SendDataToConnection(RakNet::SystemAddress& Address, const char* Data, bool bReliable, int PacketID = RocketNetReservedIDStart);
	void SendDataToHost(const char* Data, bool bReliable, int PacketID = RocketNetReservedIDStart);

protected:
	// Singleton protection
	RocketNetInstance(){}
	RocketNetInstance(const RocketNetInstance&) = delete;
	RocketNetInstance& operator=(const RocketNetInstance &) = delete;
	RocketNetInstance(RocketNetInstance &&) = delete;
	RocketNetInstance & operator=(RocketNetInstance &&) = delete;

protected:
	RakNet::RakPeerInterface *Peer = nullptr;
	RakNet::SystemAddress HostConnectionAddress;
	std::vector<RakNet::SystemAddress> ConnectionAddresses;
	bool ConnectionSuccess = false;

};

#endif //RAKNET_LIBRARY_H
