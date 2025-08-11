#include "network.hpp"

#include <iostream>

using namespace wikipedia;

uint32_t Node::nextId = 0;

double Node::getDomain(size_t i)
{
    return knowledge.at(i);
};

const double Node::getDomain(size_t i) const
{
    return knowledge.at(i);
}

Node::Node(size_t dimensions)
    : rng(std::random_device{}()), dimensions(dimensions)
{
    CHECK(dimensions <= 64, "Maximum number of dimensions should be <=64");
    id = nextId++;
    binomialKnownFields(prob);
}

const std::map<size_t, double> &Node::getKnowledge()
{
    return knowledge;
}

uint64_t Node::getKnownFields()
{
    return knownFields;
}

const uint64_t Node::getKnownFields() const
{
    return knownFields;
}

uint64_t Node::binomialKnownFields(double p)
{
    size_t dims = dimensions;
    std::uniform_real_distribution<double> dist(0, 1);

    auto &gen = rng::getEngine();
    while (dims--)
    {
        knownFields |= static_cast<uint64_t>(dist(gen) < p) << dims;
    }

    return knownFields;
}

int Node::getId()
{
    return id;
}

Article::Article(size_t dimensions)
    : Node(dimensions)
{
    std::uniform_real_distribution<double> dist(0,1);
    auto& gen = rng::getEngine();
    uint64_t kf = this->getKnownFields();
    size_t counter = 0;

    while (kf)
    {
        if (!(kf & 0x1))
        {
            knowledge[counter] = 0.0;
            knowledgeLimits[counter] = dist(gen);
        }
        counter++;
        kf >>= 1;
    }
}

Article *Article::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Article>(new Article(dimensions));
    network->articles.push_back(ptr.get());
    return ptr.release();
}

bool Article::update(const Editor *editor)
{
    // Still not that right, needs more work.

    const uint64_t kf1 = this->getKnownFields();
    const uint64_t kf2 = editor->getKnownFields();
    uint64_t commonFields = kf1 & kf2;
    if (!commonFields)
        return false;

    size_t counter = 0;

    std::uniform_real_distribution<double> dist(0, 1);
    auto &gen = rng::getEngine();

    while (commonFields)
    {
        if (commonFields & 0x1)
        {
            double articleValue = this->getDomain(counter);
            double editorValue = editor->getDomain(counter);

            double diff = editorValue - articleValue;
            knowledge[counter] += dist(gen) * std::max(diff, 0.0);
            knowledge[counter] = std::min(knowledgeLimits[counter], knowledge[counter]);
        }
        counter++;
        commonFields >>= 1;
    }

    return true;
}

Editor::Editor(size_t dimensions)
    : Node(dimensions)
{

    std::uniform_real_distribution<double> dist(0, 1);
    auto &gen = rng::getEngine();

    uint64_t kf = knownFields;
    size_t counter = 0;
    while (kf)
    {
        if (kf & 0x1)
            knowledge[counter] = dist(gen);
        counter++;
        kf >>= 1;
    }
}

Editor *Editor::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Editor>(new Editor(dimensions));
    network->editors.push_back(ptr.get());
    return ptr.release();
}

Network::Network(size_t dimensions)
    : dimensions(dimensions)
{
}

Network::Network(size_t dimensions, uint32_t numEditors, uint32_t numArticles)
{
    this->dimensions = dimensions;
    while (numEditors--)
        this->addNode<Editor>();
    while (numArticles--)
        this->addNode<Article>();
}
