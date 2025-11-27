#include <pybind11/pybind11.h>

PYBIND11_MODULE(logic, m) {
    m.doc() = "Logic to parse DOM pages";
}