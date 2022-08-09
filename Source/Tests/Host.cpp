#include <iostream>
#include <string>

#include "library.h"

int main(int argc, char** argv)
{
    hello();

    if (argc > 1 && argv[1] == std::string("failOnPurpose")) {
        std::cout << "error: lol";
    }
}