#include "network.hpp"

using namespace wikipedia;

uint32_t Node::nextId = 0;

double Node::getDomain(size_t i)
{
    return knowledge.at(i);
};

Node::Node(size_t dimensions)
{
    id = nextId++;
    knowledge.resize(dimensions);
}

int Node::getId()
{
    return id;
}

Article::Article(size_t dimensions)
    : Node(dimensions)
{
}

Article *Article::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Article>(new Article(dimensions));
    network->articles.push_back(ptr.get());
    return ptr.release();
}

bool Article::update(const Editor *editor)
{
    return false;
}

Editor::Editor(size_t dimensions)
    : Node(dimensions)
{
}

Editor *Editor::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Editor>(new Editor(dimensions));
    network->editors.push_back(ptr.get());
    return ptr.release();
}

Network::Network(size_t dimensions)
{
    this->dimensions = dimensions;
}

Network::Network(size_t dimensions, uint32_t numEditors, uint32_t numArticles)
{
    this->dimensions = dimensions;
    while (numEditors--)
        this->addNode<Editor>();
    while (numArticles--)
        this->addNode<Article>();
}
