#include "library.h"

#include <iostream>
#include "RakNet/RakPeerInterface.h"

void hello() {
    RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
    std::cout << "Hello, World!" << std::endl;
}
