#include "parser.hpp"

#include <cstring>
#include <libxml/HTMLparser.h>
#include <stdexcept>
#include <string_view>

#define CONST_STR(s) (reinterpret_cast<const char *>(s))

namespace {
inline void parse_node(Parser &parser, xmlNode *node) {
    if (node->type != XML_ELEMENT_NODE)
        return;

    std::string_view name(CONST_STR(node->name));
    if (name == "meta")
        parser.lines.push_back(name.data());
}

void DSR(Parser &parser, xmlNode *node) {
    for (xmlNode *cur_node = node; cur_node; cur_node = cur_node->next) {
        parse_node(parser, cur_node);
        DSR(parser, cur_node->children);
    }
}
} // namespace

Parser::Parser(const std::string &document) {
    const auto except = std::invalid_argument("Document is not valid");

    htmlDocPtr doc =
        htmlReadMemory(document.data(), document.size(), nullptr, "UTF-8",
                       HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |
                           HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (!doc) [[unlikely]]
        throw except;

    xmlNode *root = xmlDocGetRootElement(doc);
    if (!root) [[unlikely]]
        throw except;

    DSR(*this, root);

    xmlFreeDoc(doc);
    xmlCleanupParser();
}
