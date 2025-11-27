#ifndef LOGIC_PARSER_HPP
#define LOGIC_PARSER_HPP

#include <pybind11/native_enum.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional>
#include <string>
#include <utility>
#include <vector>

enum class HeaderWeight { H1 = 1, H2, H3, H4, H5, H6 };

class Parser {
  private:
    std::optional<std::string> title;
    std::optional<std::string> author;
    std::optional<std::string> description;
    std::vector<std::string> keywords;
    std::vector<std::pair<HeaderWeight, std::string>> headers;
    std::vector<std::string> lines;

  public:
    Parser(const std::string &document);

    const decltype(title) &getTitle() const noexcept { return title; }
    const decltype(author) &getAuthor() const noexcept { return author; }
    const decltype(description) &getDescription() const noexcept {
        return description;
    }
    const decltype(keywords) &getKeywords() const noexcept { return keywords; }
    const decltype(headers) &getHeaders() const noexcept { return headers; }
    const decltype(lines) &getLines() const noexcept { return lines; }
};

static inline void parser_init(pybind11::module_ &m) {
    namespace py = pybind11;

    py::native_enum<HeaderWeight>(m, "TitleWeight", "enum.Enum")
        .value("H1", HeaderWeight::H1)
        .value("H2", HeaderWeight::H2)
        .value("H3", HeaderWeight::H3)
        .value("H4", HeaderWeight::H4)
        .value("H5", HeaderWeight::H5)
        .value("H6", HeaderWeight::H6)
        .export_values()
        .finalize();

    py::class_<Parser>(m, "HtmlParser")
        .def(py::init<const std::string &>())
        .def_property_readonly("title", &Parser::getTitle)
        .def_property_readonly("author", &Parser::getAuthor)
        .def_property_readonly("description", &Parser::getDescription)
        .def_property_readonly("keywords", &Parser::getKeywords)
        .def_property_readonly("headers", &Parser::getHeaders)
        .def_property_readonly("lines", &Parser::getLines);
}

#endif // LOGIC_PARSER_HPP
