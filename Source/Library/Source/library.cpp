#include "library.h"

#include <iostream>
#include <algorithm>
#include <format>

#include "RakPeerInterface.h"
#include "BitStream.h"

std::string RocketNetInstance::StartHost(const unsigned short Port, const std::string& Password)
{
	std::cout << "Hi, I am the Host" << std::endl;

	char str[512];
	Peer = RakNet::RakPeerInterface::GetInstance();
	if (Peer == nullptr)
	{
		return "RocketNet Host Error: No RakNet Peer interface found";
	}

	RakNet::SocketDescriptor socketDescriptor(Port, "");
	auto Result = Peer->Startup(MaxClients, &socketDescriptor, 1);
	if (Result != RakNet::StartupResult::RAKNET_STARTED)
	{
		return "RocketNet Host Error: Could not start the RakNet Peer";
	}

	Peer->SetMaximumIncomingConnections(MaxClients);
	ConnectionSuccess = true;
	//Peer->SetIncomingPassword(Password.c_str(), Password.size());

	return "";
}

std::string RocketNetInstance::StartClient(const std::string& Address, const unsigned short Port, const std::string& Password)
{
	std::cout << "Hi, I am a Client" << std::endl;
	Peer = RakNet::RakPeerInterface::GetInstance();
	if (Peer == nullptr)
	{
		return "RocketNet Client Error: No RakNet Peer interface found";
	}

	RakNet::SocketDescriptor socketDescriptor;
	auto Result = Peer->Startup(1, &socketDescriptor, 1);
	if (Result != RakNet::StartupResult::RAKNET_STARTED)
	{
		return "RocketNet Client Error: Could not start the RakNet Peer";
	}

	auto ConnectionResult = Peer->Connect(Address.c_str(), Port, Password.c_str(), Password.size());
	if (ConnectionResult != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
	{
		return "RocketNet Client Error: Could not connect to " + Address;
	}

	return "";
}

bool RocketNetInstance::EndConnection()
{
	if (Peer != nullptr)
	{
		RakNet::RakPeerInterface::DestroyInstance(Peer);
		return true;
	}
	return false;
}

bool RocketNetInstance::ProcessPendingHostPackets()
{
	// Listen for packets while running
	RakNet::Packet* Packet = nullptr;
	for (Packet = Peer->Receive(); Packet != nullptr; Peer->DeallocatePacket(Packet), Packet = Peer->Receive())
	{
		switch (Packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				ConnectionAddresses.push_back(Packet->systemAddress);
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				ConnectionAddresses.push_back(Packet->systemAddress);
				ConnectionSuccess = true;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			default:
				// Handle RocketNet messages
				OnRocketNetMessageReceived(Packet);
				break;
		}
	}

	return false;
}

bool RocketNetInstance::ProcessPendingClientPackets()
{
	// Listen for packets while running
	RakNet::Packet* Packet = nullptr;
	for (Packet = Peer->Receive(); Packet != nullptr; Peer->DeallocatePacket(Packet), Packet = Peer->Receive())
	{
		switch (Packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				HostConnectionAddress = Packet->systemAddress;
				assert(HostConnectionAddress.size() != 0);
				ConnectionSuccess = true;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;
			default:
				// Handle RocketNet messages
				OnRocketNetMessageReceived(Packet);
				break;
		}
	}

	return false;
}

void RocketNetInstance::OnRocketNetMessageReceived(RakNet::Packet* Packet)
{
	switch (Packet->data[0])
	{
		case RocketNetReservedIDStart:
		{
			RakNet::RakString InString;
			RakNet::BitStream InStream(Packet->data, Packet->length, false);
			InStream.IgnoreBytes(sizeof(RakNet::MessageID));
			InStream.Read(InString);

			// Message from the host
			if (Packet->systemAddress == HostConnectionAddress)
			{
				std::cout << InString.C_String() << std::endl;
			}
			// Message from a client connection
			else
			{
				// Pass the message on to other clients except for the one that sent the message
				const auto SenderID = std::find(std::begin(ConnectionAddresses), std::end(ConnectionAddresses), Packet->systemAddress);
				if (SenderID != std::end(ConnectionAddresses))
				{
					const int SenderIndex = SenderID->systemIndex;
					const std::string Response = std::format("Player {} says {}", SenderIndex, InString.C_String());
					std::cout << Response << std::endl;
					for (auto& Address: ConnectionAddresses)
					{
						if (Address != Packet->systemAddress)
						{
							SendDataToConnection(Address, Response.c_str(), true);
						}
					}
				}
			}
		}
		break;
	}
}

void RocketNetInstance::SendDataToConnection(RakNet::SystemAddress& Address, const char* Data, bool bReliable, int PacketID)
{
	assert(Address.size() != 0);

	RakNet::BitStream outStream;
	outStream.Write((RakNet::MessageID)PacketID);
	outStream.Write(Data);
	int Result = Peer->Send(&outStream, HIGH_PRIORITY, bReliable ? RELIABLE_ORDERED : UNRELIABLE_SEQUENCED, 0, Address, false);
	assert(Result != 0);
}

void RocketNetInstance::SendDataToHost(const char* Data, bool bReliable, int PacketID)
{
	assert(HostConnectionAddress.size() != 0);

	SendDataToConnection(HostConnectionAddress, Data, bReliable, PacketID);
}
