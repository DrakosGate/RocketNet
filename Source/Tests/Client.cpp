#include <iostream>
#include <string>
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
		bool bRunning = true;
		char Message[512];
		while (bRunning)
		{
			Sleep(30);
			NetInstance.ProcessPendingClientPackets();

			if (NetInstance.IsConnected())
			{
				Gets(Message, sizeof(Message));
				if (strcmp(Message, "quit") == 0)
				{
					bRunning = false;
				}
				else
				{
					if (strcmp(Message, "") != 0)
					{
						NetInstance.SendDataToHost(Message, true);
					}
				}
			}
		}
	}

	NetInstance.EndConnection();
}