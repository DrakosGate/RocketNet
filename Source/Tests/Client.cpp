#include <iostream>
#include <string>

#include "library.h"

int main(int argc, char** argv)
{
    std::string ErrorMessage = StartClient();
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
        RunClient();
    }

    CloseClient();
}