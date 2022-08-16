#include <iostream>
#include <string>
#include <thread>
#include <Gets.h>
#include <assert.h>

#include "library.h"

void HandlePendingPacket(TPendingRocketNetPacket* unhandledPacket)
{
	switch (unhandledPacket->packetID)
	{
		// Packets clients shouldn't be receiving
		default:
		case RocketNetChangeName:
			assert(false);
			break;
		// Packets clients need to handle
		case RocketNetSendMessage:
			{
				std::cout << unhandledPacket->data << std::endl;
			}
			break;
	}
}

int main(int argc, char** argv)
{
	std::cin.ignore();

	RocketNetInstance& netInstance = RocketNetInstance::GetInstance();
	std::string errorMessage = netInstance.StartClient(DefaultServerAddress, DefaultServerPort, "");
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
		// Input thread
		char message[512];
		bool bInputReceived = false;
		bool bListenForInput = true;
		std::thread inputThread([&]()
		{
			while (bListenForInput)
			{
				if (!bInputReceived)
				{
					Gets(message, sizeof(message));
					bInputReceived = true;
				}
			}
		});
		inputThread.detach();

		// Packet processing
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

			if (netInstance.IsConnected())
			{
				if (bInputReceived)
				{
					const std::string input = message;
					if (input != "")
					{
						const std::string SetNameCommand("setname: ");
						const int nameStart = input.find(SetNameCommand);
						if (nameStart != std::string::npos)
						{
							std::string Name = input.substr(SetNameCommand.length());
							if (Name.length() != 0)
							{
								netInstance.SendDataToHost(Name.c_str(), true, RocketNetChangeName);
								bInputReceived = false;
								continue;
							}
						}

						if (input == "quit")
						{
							bRunning = false;
						}
						else
						{
							if (strcmp(message, "") != 0)
							{
								netInstance.SendDataToHost(message, true, RocketNetSendMessage);
							}
						}
					}
					bInputReceived = false;
				}
			}
		}

		bListenForInput = false;
	}

	netInstance.EndConnection();
}