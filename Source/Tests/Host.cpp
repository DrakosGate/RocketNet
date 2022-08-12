#include <iostream>
#include <string>
#include <assert.h>
#include <format>

#include "library.h"

void HandlePendingPackets(std::vector<TPendingRocketNetPacket>& PendingPackets)
{
	RocketNetInstance& netInstance = RocketNetInstance::GetInstance();
	assert(netInstance.IsConnected());
	auto connectionGUIDs = netInstance.GetConnectionGUIDs();

	for (auto& packet : PendingPackets)
	{
		switch (packet.packetID)
		{
			// Packets we shouldn't be handling
			case RocketNetReservedIDStart:
				assert(false);
				break;
			// Packets we should be handling
			case RocketNetChangeName:
				{
					netInstance.HostSetConnectionName(packet.senderGUID, std::string(packet.data));
				}
				break;
			case RocketNetSendMessage:
				{
					// Add player name to message before sending to other players
					const std::string SenderName = netInstance.GetNameForConnection(packet.senderGUID);
					const std::string Response = std::format("> {} says {}", SenderName, packet.data);
					std::cout << Response << std::endl;

					// Pass the message on to other clients except for the one that sent the message
					for (auto& connectionGUID : connectionGUIDs)
					{
						if (connectionGUID != packet.senderGUID)
						{
							netInstance.SendDataToConnection(connectionGUID, Response.c_str(), true, RocketNetSendMessage);
						}
					}
				}
				break;
		}
	}
}

int main(int argc, char **argv)
{
	RocketNetInstance& netInstance = RocketNetInstance::GetInstance();
	std::string errorMessage = netInstance.StartHost(DefaultServerPort, "TestPass");
	if (errorMessage.size() != 0)
	{
		std::cout << errorMessage.c_str();
	}

	bool bShouldRun = true;
	if (argc > 1 && argv[1] == std::string("test"))
	{
		bShouldRun = false;
	}

	if (bShouldRun)
	{
		bool bRunning = true;
		while (bRunning)
		{
			Sleep(30);
			HandlePendingPackets(netInstance.FetchPendingPackets());
			netInstance.ClearPendingPackets();
		}
	}

	netInstance.EndConnection();
}