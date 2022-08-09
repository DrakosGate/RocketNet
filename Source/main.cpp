#include "library.h"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    hello();

    if (argc > 1 && argv[1] == std::string("failOnPurpose")) {
        std::cout << "error: lol";
    }
}