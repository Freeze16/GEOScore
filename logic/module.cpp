#include <pybind11/pybind11.h>

#include "parser.hpp"

PYBIND11_MODULE(logic, m, pybind11::mod_gil_not_used()) {
    m.doc() = "Logic to parse DOM pages";
    parser_init(m);
}
