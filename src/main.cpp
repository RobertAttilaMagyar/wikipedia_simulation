#include <iostream>
#include <stdio.h>
#include "network.hpp"
#include <bitset>

int main()
{
    auto myNet = wikipedia::Network(10, 3, 5);
    for (auto n : myNet.getNodes<wikipedia::Editor>())
    {
        std::cout << n->getId() << ' ' << std::bitset<64>(n->getKnownFields()) << std::endl;

        for (const auto& [key, value] : n->getKnowledge()) {
        std::cout << (int)key << " -> " << value << '\n';
    }
    }
    for (auto m : myNet.getNodes<wikipedia::Article>())
        std::cout << m->getId() << ' ' << std::bitset<64>(m->getKnownFields()) << std::endl;
}