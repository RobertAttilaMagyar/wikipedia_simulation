#include <iostream>
#include <stdio.h>
#include "network.hpp"
#include <bitset>

#include <spdlog/spdlog.h>

int main()
{

    spdlog::set_level(spdlog::level::debug);
    auto net = wikipedia::Network(20, 5, 8);
    wikipedia::NodePairs dummy = net.getPairs();

    for(const auto& [k, v]: dummy)
    {
        std::cout<<k->getId()<<' '<<v->getId()<<std::endl;
    }
    return 0;
}