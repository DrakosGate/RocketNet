#include <iostream>
#include <string>
#include <assert.h>
#include <format>

#include "library.h"

void HandlePendingPacket(TPendingRocketNetPacket* unhandledPacket)
{
	RocketNetInstance& netInstance = RocketNetInstance::GetInstance();
	assert(netInstance.IsConnected());
	auto connectionGUIDs = netInstance.GetConnectionGUIDs();

	switch (unhandledPacket->packetID)
	{
		// Packets we shouldn't be handling
		case RocketNetReservedIDStart:
			assert(false);
			break;
		// Packets we should be handling
		case RocketNetChangeName:
			{
				netInstance.HostSetConnectionName(unhandledPacket->senderGUID, std::string(unhandledPacket->data));
			}
			break;
		case RocketNetSendMessage:
			{
				// Add player name to message before sending to other players
				const std::string SenderName = netInstance.GetNameForConnection(unhandledPacket->senderGUID);
				const std::string Response = std::format("> {} says {}", SenderName, unhandledPacket->data);
				std::cout << Response << std::endl;

				// Pass the message on to other clients except for the one that sent the message
				for (auto& connectionGUID : connectionGUIDs)
				{
					if (connectionGUID != unhandledPacket->senderGUID)
					{
						netInstance.SendDataToConnection(connectionGUID, Response.c_str(), true, RocketNetSendMessage);
					}
				}
			}
			break;
	}
}

int main(int argc, char **argv)
{
	RocketNetInstance& netInstance = RocketNetInstance::GetInstance();
	const char* errorMessage = netInstance.StartHost(DefaultServerPort, "TestPass");
	if (strlen(errorMessage) > 0)
	{
		std::cout << errorMessage << std::endl;
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
			if (netInstance.CollectPendingPackets())
			{
				while (auto unhandledPacket = netInstance.HandleNextPendingPacket())
				{
					HandlePendingPacket(unhandledPacket);
				}
				netInstance.ClearPendingPackets();
			}
		}
	}

	netInstance.EndConnection();
}