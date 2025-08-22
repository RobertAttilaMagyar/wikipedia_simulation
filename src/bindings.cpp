#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "network.hpp"
#include <map>

namespace py = pybind11;

PYBIND11_MODULE(_core, m)
{   

    py::class_<wikipedia::Network>(m, "Network")
        .def(py::init<size_t>())
        .def(py::init<size_t, uint32_t, uint32_t>())
        .def("add_editor", &wikipedia::Network::addNode<wikipedia::Editor>, "Adds a new randomly initialized Editor to the network.")
        .def("add_article", &wikipedia::Network::addNode<wikipedia::Article>, "Adds a new randomly initialized Aricle to the network.")
        .def_property_readonly("editor_ids", [](const wikipedia::Network &self)
             {
                std::vector<uint32_t> v;
                for (const auto& e : self.getNodes<wikipedia::Editor>()) {
                    v.push_back(e->getId());
                }
                return v; }, "Returns a list of Editor IDs")
        .def_property_readonly("article_ids", [](const wikipedia::Network &self)
             {
                std::vector<uint32_t> v;
                for (const auto& a : self.getNodes<wikipedia::Article>()) {
                    v.push_back(a->getId());
                }
                return v; }, "Returns a list of Article IDs")
        .def("create_connections", &wikipedia::Network::getPairs, "Creates connections between Articles and Editors.")
        .def("clear_connections", &wikipedia::Network::clearConnections, "Clears connections in the network.")
        .def_property_readonly("adjacency", [](const wikipedia::Network &self)
             {
                    std::map<uint32_t, uint32_t> adjacency_map;
                    for(auto& [k, v] : self.adjacency())
                    {
                        adjacency_map.insert({k->getId(), v->getId()});
                    }
                    return adjacency_map; })

        .def("update_articles", &wikipedia::Network::updateArticles)

        ;
}