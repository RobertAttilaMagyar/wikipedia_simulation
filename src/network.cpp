#include "network.hpp"

using namespace wikipedia;

uint32_t Node::nextId = 0;

double Node::getDomain(size_t i)
{
    CHECK(state.at(i).has_value(), "Unknown state accessed");
    return state.at(i).value();
};

const double Node::getDomain(size_t i) const
{
    CHECK(state.at(i).has_value(), "Unknown state accessed");
    return state.at(i).value();
}

bool Node::knowsDomain(size_t i)
{
    return state.at(i).has_value();
}

bool Node::knowsDomain(size_t i) const
{
    return state.at(i).has_value();
}

Node::Node(size_t dimensions)
    : rng(std::random_device{}()), dimensions(dimensions)
{
    id = nextId++;
    state.resize(dimensions, std::nullopt);
    binomialKnownFields(prob);
}

void Node::binomialKnownFields(double p)
{
    std::uniform_real_distribution<double> dist(0, 1);
    auto &gen = rng::getEngine();
    for (size_t i = 0; i < dimensions; i++)
    {
        if (dist(gen) < p)
        {
            state[i] = std::make_optional(0.0);
        }
    }
}

const uint32_t Node::getId()
{
    return id;
}

const uint32_t Node::getId() const
{
    return id;
}

Article::Article(size_t dimensions)
    : Node(dimensions)
{
    knowledgeLimits.resize(state.size(), std::nullopt);

    std::uniform_real_distribution<double> dist(0, 1);
    auto &gen = rng::getEngine();
    for (size_t i = 0; i < state.size(); i++)
    {
        if (state[i].has_value())
            knowledgeLimits[i] = std::make_optional(dist(gen));
    }
}

Article *Article::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Article>(new Article(dimensions));
    spdlog::debug("Adding Article (id: {}) to network", ptr->getId());
    network->articles.push_back(ptr.get());
    return ptr.release();
}

double Article::getLimit(size_t i) const
{
    CHECK(knowledgeLimits.at(i).has_value(), "Knowledge-limit value cannot be accessed");
    return knowledgeLimits.at(i).value();
}

bool Article::update(const Editor *editor)
{
    spdlog::debug("Updating Article-{} with Editor-{}", this->getId(), editor->getId());
    std::vector<double> knowledgeDiffs = editor->getKnowledgeDiffs(this);

    double diffSum = std::accumulate(knowledgeDiffs.begin(), knowledgeDiffs.end(), 0.0);
    if (diffSum == 0.0)
    {
        spdlog::debug("Editor-{} cannot update Article-{}");
        return false;
    }

    std::discrete_distribution<> dist(knowledgeDiffs.begin(), knowledgeDiffs.end());
    std::uniform_real_distribution<double> udist(0.0, 1.0);
    auto &gen = rng::getEngine();
    size_t index = dist(gen);
    state.at(index) = (state.at(index).value() + std::min(knowledgeLimits.at(index).value(), udist(gen) * (knowledgeDiffs.at(index))));
    return true;
}

Editor::Editor(size_t dimensions)
    : Node(dimensions)
{

    std::uniform_real_distribution<double> dist(0, 1);
    auto &gen = rng::getEngine();

    for (auto &s : state)
    {
        if (s.has_value())
            s = std::make_optional(dist(gen));
    }
}

std::vector<double> Editor::getKnowledgeDiffs(const Article *article) const
{
    std::vector<double> knowledgeDiffs(state.size(), 0.0);
    for (size_t i = 0; i < state.size(); i++)
    {
        if (article->knowsDomain(i) && this->knowsDomain(i))
        {
            knowledgeDiffs.at(i) = std::max((std::min(this->getDomain(i), article->getLimit(i)) - article->getDomain(i)), 0.0);
        }
    }

    return knowledgeDiffs;
}

double Editor::contributionMeasure(const Article *article) const
{
    auto knowledgeDiffs = getKnowledgeDiffs(article);

    return std::accumulate(knowledgeDiffs.begin(), knowledgeDiffs.end(), 0.0);
}

Editor *Editor::create(Network *network, size_t dimensions)
{
    auto ptr = std::unique_ptr<Editor>(new Editor(dimensions));
    spdlog::debug("Adding Editor (id: {}) to network", ptr->getId());
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
