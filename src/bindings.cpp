#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "network.hpp"

namespace py = pybind11;

PYBIND11_MODULE(wikipedia_module, m)
{
    py::class_<wikipedia::Network>(m, "Network")
        .def(py::init<size_t>())
        .def(py::init<size_t, uint32_t, uint32_t>())

        // Explicitly specify template instances if needed:
        .def("add_editor", &wikipedia::Network::addNode<wikipedia::Editor>)
        .def("add_article", &wikipedia::Network::addNode<wikipedia::Article>)
        .def("get_editor_ids", [](const wikipedia::Network &self)
             {
                std::vector<uint32_t> v;
                for (const auto& e : self.getNodes<wikipedia::Editor>()) {
                    v.push_back(e->getId());
                }
                return v; } )

        .def("get_article_ids", [](const wikipedia::Network &self)
             {
                std::vector<uint32_t> v;
                for (const auto& e : self.getNodes<wikipedia::Article>()) {
                    v.push_back(e->getId());
                }
                return v; } )

        ;
}