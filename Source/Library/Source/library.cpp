
#include <iostream>
#include "RakNet/RakPeerInterface.h"
#include "library.h"

void hello() {
    RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
    std::cout << "Hello, World!" << std::endl;

}
