#include <iostream>
#include <string>
#include <thread>
#include <Gets.h>

#include "library.h"

int main(int argc, char** argv)
{
	std::cin.ignore();

	RocketNetInstance& NetInstance = RocketNetInstance::GetInstance();
	std::string ErrorMessage = NetInstance.StartClient(DefaultServerAddress, DefaultServerPort, "");
	if (ErrorMessage.size() != 0)
	{
		std::cout << ErrorMessage.c_str();
	}

	bool bShouldRun = true;
	if (argc > 1 && argv[1] == std::string("test"))
	{
		bShouldRun = false;
	}

	if (bShouldRun)
	{
		// Input thread
		char Message[512];
		bool bInputReceived = false;
		bool bListenForInput = true;
		std::thread inputThread([&]()
		{
			while (bListenForInput)
			{
				if (!bInputReceived)
				{
					Gets(Message, sizeof(Message));
					bInputReceived = true;
				}
			}
		});
		inputThread.detach();

		// Packet processing
		bool bRunning = true;
		while (bRunning)
		{
			Sleep(30);
			NetInstance.ProcessPendingClientPackets();

			if (NetInstance.IsConnected())
			{
				if (bInputReceived)
				{
					const std::string Input = Message;
					if (Input != "")
					{
						const std::string SetNameCommand("setname: ");
						const int NameStart = Input.find(SetNameCommand);
						if (NameStart != std::string::npos)
						{
							std::string Name = Input.substr(SetNameCommand.length());
							if (Name.length() != 0)
							{
								NetInstance.SendDataToHost(Name.c_str(), true, RocketNetChangeName);
								bInputReceived = false;
								continue;
							}
						}

						if (Input == "quit")
						{
							bRunning = false;
						}
						else
						{
							if (strcmp(Message, "") != 0)
							{
								NetInstance.SendDataToHost(Message, true, RocketNetSendMessage);
							}
						}
					}
					bInputReceived = false;
				}
			}
		}

		bListenForInput = false;
	}

	NetInstance.EndConnection();
}