#include <iostream>
#include <stdio.h>
#include "network.hpp"
#include <bitset>

#include <spdlog/spdlog.h>

int main()
{

    spdlog::set_level(spdlog::level::debug);
    auto net = wikipedia::Network(20, 5, 8);

    return 0;
}