#include "library.h"

#include <iostream>
#include <algorithm>
#include <format>

#include "RakPeerInterface.h"
#include "BitStream.h"

const char* RocketNetInstance::StartHost(uint16_t port, const char* password)
{
	if (peer != nullptr)
	{
		return "Cannot Start Host as this instance is already active";
	}

	std::cout << "Hi, I am the Host" << std::endl;

	char str[512];
	peer = RakNet::RakPeerInterface::GetInstance();
	if (peer == nullptr)
	{
		return "RocketNet Host Error: No RakNet Peer interface found";
	}

	RakNet::SocketDescriptor socketDescriptor(port, "");
	auto Result = peer->Startup(MaxClients, &socketDescriptor, 1);
	if (Result != RakNet::StartupResult::RAKNET_STARTED)
	{
		return "RocketNet Host Error: Could not start the RakNet Peer";
	}

	peer->SetMaximumIncomingConnections(MaxClients);
	bConnectionSuccess = true;
	bIsHost = true;
	//Peer->SetIncomingPassword(password.c_str(), password.size());

	return "";
}

const char* RocketNetInstance::StartClient(const char* address, uint16_t port, const char* password)
{
	if (peer != nullptr)
	{
		return "Cannot Start Host as this instance is already active";
	}

	std::cout << "Hi, I am a Client" << std::endl;

	peer = RakNet::RakPeerInterface::GetInstance();
	if (peer == nullptr)
	{
		return "RocketNet Client Error: No RakNet Peer interface found";
	}

	RakNet::SocketDescriptor socketDescriptor;
	auto Result = peer->Startup(1, &socketDescriptor, 1);
	if (Result != RakNet::StartupResult::RAKNET_STARTED)
	{
		return "RocketNet Client Error: Could not start the RakNet Peer";
	}

	auto ConnectionResult = peer->Connect(address, port, password, strlen(password));
	if (ConnectionResult != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
	{
		return std::format("RocketNet Client Error: Could not connect to {}", address).c_str();
	}

	return "";
}

bool RocketNetInstance::EndConnection()
{
	if (peer != nullptr)
	{
		bIsHost = false;
		bConnectionSuccess = false;
		connections.clear();
		pendingPackets.clear();
		RakNet::RakPeerInterface::DestroyInstance(peer);
		return true;
	}
	return false;
}

std::vector<ConnectionGUID>& RocketNetInstance::GetConnectionGUIDs()
{
	connectionGUIDs.clear();
	for (const auto& connection : connections)
	{
		connectionGUIDs.push_back(connection.connectionGUID);
	}
	return connectionGUIDs;
}

std::string RocketNetInstance::GetNameForConnection(const ConnectionGUID& connectionGUID) const
{
	if (const TConnectionInfo* connection = GetConnectionInfoForGUID(connectionGUID))
	{
		return (connection->nameOverride.length() > 0 ? connection->nameOverride : std::format("Player {}", connection->address.systemIndex));
	}
	return "";
}

bool RocketNetInstance::CollectPendingPackets()
{
	// Listen for packets while running
	RakNet::Packet* Packet = nullptr;
	for (Packet = peer->Receive(); Packet != nullptr; peer->DeallocatePacket(Packet), Packet = peer->Receive())
	{
		switch (Packet->data[0])
		{
			// --------------------------------------------------------------------------------------------
			// ----- Host packets -----
			// --------------------------------------------------------------------------------------------
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				connections.emplace_back(Packet->systemAddress, Packet->guid.g);
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				connections.emplace_back(Packet->systemAddress, Packet->guid.g);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			// --------------------------------------------------------------------------------------------
			// ----- Client packets -----
			// --------------------------------------------------------------------------------------------
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				printf("Type |quit| to exit or |setname: name| to change your name.\n");
				connections.emplace_back(Packet->systemAddress, Packet->guid.g);
				bConnectionSuccess = true;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			// --------------------------------------------------------------------------------------------
			// RocketNet packets - these are added to the pending packet list and passed back to the caller
			// --------------------------------------------------------------------------------------------
			default:
				HandleNewRocketNetPacket(Packet);
				break;
		}
	}

	return pendingPackets.size() > 0;
}

TPendingRocketNetPacket* RocketNetInstance::HandleNextPendingPacket()
{
	if (lastPacketHandled < pendingPackets.size())
	{
		TPendingRocketNetPacket* nextUnhandledPacket = &pendingPackets[lastPacketHandled];
		++lastPacketHandled;
		return nextUnhandledPacket;
	}

	return nullptr;
}

void RocketNetInstance::ClearPendingPackets()
{
	lastPacketHandled = 0;
	pendingPackets.clear();
}

void RocketNetInstance::HandleNewRocketNetPacket(RakNet::Packet* packet)
{
	// Read and extract the data from the packet without the packet ID - this will be passed separately
	RakNet::RakString inString;
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.Read(inString);

	// Add the extracted data with packet ID and sender GUID to the list of pending packets to be handled later
	pendingPackets.emplace_back(TPendingRocketNetPacket(inString.C_String(), inString.GetLength(), packet->data[0], packet->guid.g));
}

const TConnectionInfo* RocketNetInstance::GetConnectionInfoForGUID(const ConnectionGUID& connectionGuid) const
{
	// Find the address for the connection matching this GUID
	const auto foundConnection = std::find_if(std::begin(connections), std::end(connections), [&connectionGuid](const TConnectionInfo& connection)
	{
		return connection.connectionGUID == connectionGuid;
	});
	if (foundConnection != std::end(connections))
	{
		return &(*foundConnection);
	}

	return nullptr;
}

void RocketNetInstance::SendDataToConnection(const ConnectionGUID& connectionGUID, const char* data, bool bReliable, int packetID)
{
	const TConnectionInfo* connectionInfo = GetConnectionInfoForGUID(connectionGUID);
	if (connectionInfo != nullptr)
	{
		SendDataToConnection(connectionInfo->address, data, bReliable, packetID);
	}
}

void RocketNetInstance::SendDataToConnection(const RakNet::SystemAddress& address, const char* data, bool bReliable, int packetID)
{
	RakNet::BitStream outStream;
	outStream.Write((RakNet::MessageID)packetID);
	outStream.Write(data);
	int Result = peer->Send(&outStream, HIGH_PRIORITY, bReliable ? RELIABLE_ORDERED : UNRELIABLE_SEQUENCED, 0, address, false);
	assert(Result != 0);
}

void RocketNetInstance::SendDataToAllConnections(const char* data, bool bReliable, int packetID)
{
	assert(bIsHost);

	for (const TConnectionInfo& Connection : connections)
	{
		SendDataToConnection(Connection.connectionGUID, data, bReliable, packetID);
	}
}

void RocketNetInstance::SendDataToHost(const char* data, bool bReliable, int packetID)
{
	assert(!bIsHost && connections.size() == 1);

	SendDataToConnection(connections[0].connectionGUID, data, bReliable, packetID);
}

void RocketNetInstance::HostSetConnectionName(const ConnectionGUID& connectionGUID, const std::string& name)
{
	assert (bIsHost);

	auto foundConnection = std::find_if(std::begin(connections), std::end(connections), [&connectionGUID](const TConnectionInfo& Connection)
	{
		return Connection.connectionGUID == connectionGUID;
	});
	if (foundConnection != std::end(connections))
	{
		foundConnection->nameOverride = name;
		std::cout << std::format("Player {} name changed to {}", foundConnection->address.systemIndex, foundConnection->nameOverride) << std::endl;
	}
}

