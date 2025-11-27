#include "parser.hpp"

#include <libxml/HTMLparser.h>
#include <stdexcept>

Parser::Parser(const std::string &document) : description("abobus") {
    htmlDocPtr doc =
        htmlReadMemory(document.data(), document.size(), nullptr, "UTF-8",
                       HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |
                           HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (!doc) [[unlikely]]
        throw std::invalid_argument("Document is not valid");

    [[maybe_unused]] xmlNode *root = xmlDocGetRootElement(doc);
}
