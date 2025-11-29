#include "parser.hpp"

#include <cstring>
#include <libxml/HTMLparser.h>
#include <stdexcept>
#include <string_view>

#define CONST_STR(s) (reinterpret_cast<const char *>(s))

namespace {
inline std::optional<std::string> get_prop_attr(xmlNode *node, const char *prop_name) {
    xmlChar *attr = xmlGetProp(node, reinterpret_cast<const xmlChar *>(prop_name));
    if (attr) {
        std::string value(CONST_STR(attr));
        xmlFree(attr);
        return value;
    }
    return std::nullopt;
}

inline std::optional<std::string> get_node_content(xmlNode *node) {
    xmlChar *content = xmlNodeGetContent(node);
    if (content) {
        std::string value(CONST_STR(content));
        xmlFree(content);
        if (value.empty()) {
            return std::nullopt;
        }
        return value;
    }
    return std::nullopt;
}

inline void parse_node(Parser &parser, xmlNode *node) {
    if (node->type != XML_ELEMENT_NODE)
        return;

    std::string_view name(CONST_STR(node->name));
    
    if (name == "title") {
        if (!parser.title) {
            if (auto content = get_node_content(node)) {
                parser.title = std::move(*content);
            }
        }
    }
    else if (name == "meta") {
        if (auto name_attr = get_prop_attr(node, "name")) {
            if (auto content_attr = get_prop_attr(node, "content")) {
                if (*name_attr == "author" && !parser.author) {
                    parser.author = std::move(*content_attr);
                } else if (*name_attr == "description" && !parser.description) {
                    parser.description = std::move(*content_attr);
                } else if (*name_attr == "keywords") {
                    std::stringstream ss(std::move(*content_attr));
                    std::string keyword;
                    while (std::getline(ss, keyword, ',')) {
                        size_t first = keyword.find_first_not_of(' ');
                        size_t last = keyword.find_last_not_of(' ');
                        if (std::string::npos != first) {
                            parser.keywords.push_back(keyword.substr(first, (last - first + 1)));
                        }
                    }
                }
            }
        }
    }
    else if (name.length() == 2 && name[0] == 'h' && name[1] >= '1' && name[1] <= '6') {
        if (auto content = get_node_content(node)) {
            int weight_int = name[1] - '0'; 
            HeaderWeight weight = static_cast<HeaderWeight>(weight_int); 
            parser.headers.emplace_back(weight, std::move(*content));
        }
    }
    else if (name == "p" || name == "div" || name == "li") {
        if (auto content = get_node_content(node)) {
            parser.lines.push_back(std::move(*content));
        }
    }
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
