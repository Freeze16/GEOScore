#include "parser.hpp"

#include <libxml/HTMLparser.h>
#include <stdexcept>
#include <string_view>

#define CONST_STR(s) (reinterpret_cast<const char *>(s))

namespace {
xmlNode *findByTag(xmlNode *root, const std::string_view name) noexcept {
    xmlNode *node = root->children;
    while (node && std::string_view(CONST_STR(node->name)) != "head")
        node = node->next ? node->next : node->children;
    return node;
}
} // namespace

Parser::Parser(const std::string &document) {
    htmlDocPtr doc =
        htmlReadMemory(document.data(), document.size(), nullptr, "UTF-8",
                       HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |
                           HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
    if (!doc) [[unlikely]]
        throw std::invalid_argument("Document is not valid");

    xmlNode *root = xmlDocGetRootElement(doc);

    xmlNode *head = findByTag(root, "head");
    xmlNode *body = findByTag(root, "body");

    if (!(head && body))
        throw std::invalid_argument(
            "Document doesn't contain head or body tag");

    // xmlNode *titleTag = findByTag(head, "title");
    // if (titleTag)
    //     title = std::string(CONST_STR(titleTag->name));

    for (xmlNode *node = head->next; node; node = node->next)
        if (std::string_view(CONST_STR(node->name)) == "meta") {
            std::string_view tag(
                CONST_STR(xmlGetProp(node, (const xmlChar *)"name")));
            std::string value(CONST_STR(node->name));

            if (tag == "author")
                author = value;
            else if (tag == "description")
                description = value;
        }
}
