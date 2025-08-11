#include <iostream>
#include <stdio.h>
#include "network.hpp"

int main(){
    auto myNet = wikipedia::Network(15, 3, 5);
    for(auto n: myNet.getNodes<wikipedia::Editor>())std::cout<<n->getId()<<std::endl;
    for(auto m: myNet.getNodes<wikipedia::Article>())std::cout<<m->getId()<<std::endl;
}