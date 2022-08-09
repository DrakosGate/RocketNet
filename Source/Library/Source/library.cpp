
#include <iostream>
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "library.h"

const unsigned short MaxClients = 4;
const unsigned short ServerPort = 60000;
const std::string ServerAddress = "127.0.0.1";
const std::string Password = "";
RakNet::RakPeerInterface* Peer = nullptr;

std::string StartHost()
{
    std::cout << "Starting Host" << std::endl;

    char str[512];
    Peer = RakNet::RakPeerInterface::GetInstance();
    if (Peer == nullptr)
    {
        return "RocketNet Host Error: No RakNet Peer interface found";
    }

    RakNet::SocketDescriptor socketDescriptor(ServerPort, "");
    auto Result = Peer->Startup(MaxClients, &socketDescriptor, 1);
    if (Result != RakNet::StartupResult::RAKNET_STARTED)
    {
        return "RocketNet Host Error: Could not start the RakNet Peer";
    }

    Peer->SetMaximumIncomingConnections(MaxClients);

    return "";
}

std::string StartClient()
{
    std::cout << "Starting Client" << std::endl;
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

    auto ConnectionResult = Peer->Connect(ServerAddress.c_str(), ServerPort, Password.c_str(), Password.size());
    if (ConnectionResult != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
    {
        return "RocketNet Client Error: Could not connect to " + ServerAddress;
    }

    return "";
}

bool RunHost()
{
    // Listen for packets while running
    bool bRunning = true;
    RakNet::Packet* Packet = nullptr;
    while (bRunning)
    {
        for (Packet = Peer->Receive(); Packet != nullptr; Peer->DeallocatePacket(Packet), Packet=Peer->Receive())
        {
            switch(Packet->data[0])
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
                    break;
                case ID_NEW_INCOMING_CONNECTION:
                    printf("A connection is incoming.\n");
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
                    printf("Message with identifier %i has arrived.\n", Packet->data[0]);
                    break;
            }
        }
    }

    return false;
}

bool RunClient()
{
    // Listen for packets while running
    bool bRunning = true;
    RakNet::Packet* Packet = nullptr;
    while (bRunning)
    {
        for (Packet = Peer->Receive(); Packet != nullptr; Peer->DeallocatePacket(Packet), Packet=Peer->Receive())
        {
            switch(Packet->data[0])
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
                    printf("Message with identifier %i has arrived.\n", Packet->data[0]);
                    break;
            }
        }
    }

    return false;
}

bool CloseHost()
{
    if (Peer != nullptr)
    {
        RakNet::RakPeerInterface::DestroyInstance(Peer);
        return true;
    }
    return false;
}

bool CloseClient()
{
    if (Peer != nullptr)
    {
        RakNet::RakPeerInterface::DestroyInstance(Peer);
        return true;
    }
    return false;
}
