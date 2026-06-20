#include "network.hpp"
#include <iostream>

int main()
{
    wikipedia::Network net(10);

    net.addNode<wikipedia::Editor>();
    net.addNode<wikipedia::Article>();

    std::cout << "Editors: " << net.getNodes<wikipedia::Editor>().size() << "\n";
    std::cout << "Articles: " << net.getNodes<wikipedia::Article>().size() << "\n";

    wikipedia::Article* art = net.getNodeByIdx<wikipedia::Article>(0);

    for(auto i = 0; i < art->dimensions; i++)
    {
        std::cout<<art->getLimit(i)<<std::endl;
    }

    return 0;
}