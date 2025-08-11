#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <memory>
#include <cstddef>
#include <span>
#include <rng.hpp>
#include <stdexcept>
#include <map>

#include <algorithm>

#define CHECK(cond, msg)                                    \
    do                                                      \
    {                                                       \
        if (!(cond))                                        \
        {                                                   \
            throw std::runtime_error("CHECK failed: " msg); \
        }                                                   \
    } while (0)

namespace wikipedia
{
    class Article;
    class Editor;
    class Network;

    class Node
    {
        static uint32_t nextId;

    public:
        virtual ~Node() = default;

    public:
        double getDomain(size_t i);
        const double getDomain(size_t i) const;
        int getId();

        uint64_t getKnownFields();
        const uint64_t getKnownFields() const;

        const std::map<size_t, double> &getKnowledge();
        size_t dimensions;

    private:
        std::mt19937 rng;
        int id;
        uint64_t binomialKnownFields(double p);

    protected:
        std::map<size_t, double> knowledge;
        uint64_t knownFields;

        double prob = 0.2;
        Node(size_t dimensions);
    };

    class Article : public Node
    {
    public:
        static Article *create(Network *network, size_t dimensions);

        bool update(const Editor *editor);
        ~Article() = default;

    private:
        std::map<size_t, double> knowledgeLimits;
        explicit Article(size_t dimensions);
    };

    class Editor : public Node
    {
    public:
        static Editor *create(Network *network, size_t dimensions);
        ~Editor() = default;

    private:
        explicit Editor(size_t dimensions);
    };

    class Network
    {
        friend class Editor;
        friend class Article;

    public:
        Network(size_t dimensions);
        Network(size_t dimensions, uint32_t numEditors, uint32_t numArticles);

    public:
        template <class T>
        bool addNode();

        template <class T>
        const std::vector<T *> &getNodes() const;

    private:
        size_t dimensions;
        std::vector<Article *> articles;
        std::vector<Editor *> editors;
    };

    template <>
    inline const std::vector<Article *> &Network::getNodes<Article>() const
    {
        return this->articles;
    }

    template <>
    inline const std::vector<Editor *> &Network::getNodes<Editor>() const
    {
        return this->editors;
    }

    template <>
    inline bool Network::addNode<Editor>()
    {
        Editor *editor = Editor::create(this, dimensions);
        return editor != nullptr;
    }

    template <>
    inline bool Network::addNode<Article>()
    {
        Article *editor = Article::create(this, dimensions);
        return editor != nullptr;
    }

}
