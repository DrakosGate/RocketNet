#include <iostream>
#include <string>

#include "library.h"

int main(int argc, char **argv)
{
	RocketNetInstance& NetInstance = RocketNetInstance::GetInstance();
	std::string ErrorMessage = NetInstance.StartHost(DefaultServerPort, "TestPass");
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
		while (bRunning)
		{
			Sleep(30);
			NetInstance.ProcessPendingHostPackets();
		}
	}

	NetInstance.EndConnection();
}