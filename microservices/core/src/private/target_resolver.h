#ifndef MICROSERVICES__CORE__SRC__PRIVATE__TARGET_RESOLVER_H
#define MICROSERVICES__CORE__SRC__PRIVATE__TARGET_RESOLVER_H

// find function by url

#include <boost/asio.hpp>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../include/corelib/alias.h"
#include "../include/corelib/http_assert.h"

struct CallbackInfo {
    std::unordered_map<std::string, std::string> url_params;
};

using EndpointFunction = std::function<std::shared_ptr<Response>(CallbackInfo&&)>;

class TerminalNode {
public:
    TerminalNode(const std::string& http_method, EndpointFunction function) : http_method(http_method), function(function) {}
    TerminalNode(std::string&& http_method, EndpointFunction function) : http_method(std::move(http_method)), function(function) {}

    const std::string http_method;
    const EndpointFunction function;
};

class Node {
public:
    using NodeSharedPtr = std::shared_ptr<Node>;

    Node(const std::string& name) : name(name) {}

    Node(std::string&& name) : name(std::move(name)) {}

    std::shared_ptr<Node> findNamedNode(const std::string& name) const {
        auto it = named_nodes.find(name);
        if (it == named_nodes.end()) {
            return {};
        }
        return it->second;
    }

    const std::vector<std::shared_ptr<Node>>& findPatternNodes() const { return pattern_nodes; }

    std::shared_ptr<TerminalNode> findTerminalNode(const std::string& name) const {
        auto it = terminal_nodes.find(name);
        if (it == terminal_nodes.end()) {
            return {};
        }
        return it->second;
    }

    std::shared_ptr<Node> addNode(const std::string& node_name, bool pattern) {
        auto node = std::make_shared<Node>(node_name);
        if (pattern) {
            return addPattrnNode(node_name);
        }
        return addNamedNode(node_name);
    }

    void addTerminalNode(const std::string& node_name, EndpointFunction callback) {
        auto it = terminal_nodes.emplace(node_name, std::make_shared<TerminalNode>(node_name, callback));
        LOGIC_ASSERT(it.second, "Node with name " + node_name + " already exist");
    }

    const std::string name;

protected:
    std::shared_ptr<Node> addNamedNode(const std::string& node_name) {
        auto it = named_nodes.emplace(node_name, std::make_shared<Node>(node_name));
        return it.first->second;
    }

    std::shared_ptr<Node> addPattrnNode(const std::string& node_name) {
        size_t idx = pattern_nodes.size();
        pattern_nodes.emplace_back(std::make_shared<Node>(node_name));
        return pattern_nodes[idx];
    }

    std::vector<std::shared_ptr<Node>> pattern_nodes;
    std::unordered_map<std::string, std::shared_ptr<Node>> named_nodes;  // /some/
    std::unordered_map<std::string, std::shared_ptr<TerminalNode>> terminal_nodes;
};

// TODO: add mock target resolver
class TargetResolver {
public:
    void checkUrlUnique(const size_t pos, const std::string& url, const std::string& http_method, std::shared_ptr<Node> node) {
        LOGIC_ASSERT(node, "Got nullptr node");
        size_t idx = url.find('/', pos);
        if (idx == std::string::npos) {
            idx = (pos >= url.size()) ? std::string::npos : url.size();
        }
        

        if (idx == std::string::npos) {
            // terminal node
            auto terminal_node = node->findTerminalNode(http_method);
            LOGIC_ASSERT(!terminal_node, "Can not add url: " + url + " you have already same url");
            return;
        }

        const size_t substr_len = idx - pos;
        std::string substr = url.substr(pos, substr_len);
        PARSE_ASSERT(substr.size() > 0, "Suburl don't have to be empty 'some//http' -> 'some/some/http'");
        const bool pattern_suburl = (substr[0] == '{');

        if (!pattern_suburl) {
            // named node
            std::shared_ptr<Node> named_node = node->findNamedNode(substr);
            if (named_node) {
                checkUrlUnique(idx + 1 /* skip '/' */, url, http_method, named_node);
            }
            return;
        }

        const std::vector<std::shared_ptr<Node>>& pattern_nodes = node->findPatternNodes();
        for (int i = 0; i < pattern_nodes.size(); ++i) {
            checkUrlUnique(idx + 1, url, http_method, pattern_nodes[i]);
        }
    }

    // TODO uptimize work with strings
    void addUrlToTrie(std::string&& url, std::string&& http_method, EndpointFunction callback) {
        LOGIC_ASSERT(root, "Root node is nullptr");
        std::shared_ptr<Node> node = root;
        int balance = 0;
        std::string sub_url = "";

        // skip first "/"
        // process first symbol
        PARSE_ASSERT(!url.empty() && url[0] == '/', "request url: " + url + " have to start with '/'");
        PARSE_ASSERT(!url.empty() && url[url.size() - 1] != '/', "request url: " + url + " don't have to end with '/'");

        std::unordered_set<std::string> pattern_node_names;
        auto no_pattern_dublication = [&pattern_node_names](bool pattern, const std::string& sub_url) -> bool {
            if (pattern) {
                auto res = pattern_node_names.find(sub_url);
                return res == pattern_node_names.end();
            }
            return true;
        };

        for (int i = 1; i < url.size() - 1; ++i) {
            char ch = url[i];
            if (ch == '{') {
                PARSE_ASSERT(url[i - 1] == '/', "Token '{' used only before '/', like this: '/some/{id}/url'");
                PARSE_ASSERT(balance == 0, "Prevent nested braces like 'some/{{url}}' -> 'some/{url}'");
                balance += 1;
            } else if (ch == '}') {
                PARSE_ASSERT(url[i + 1] == '/', "Token '}' used only after '/', like this: '/some/{id}/url'");
                PARSE_ASSERT(balance == 1, "Token '}' expected token '{' before like '/some/id}' -> '/some/{id}'");
                balance -= 1;
            } else if (ch == '/') {
                const bool string_pattern = url[i - 1] == '}';

                PARSE_ASSERT(balance == 0,
                        "Token '}' and '{' have to use like '/some/{id}/animal' not '/some/{id/}/animal'");
                PARSE_ASSERT(sub_url.size() > 0, "Suburl too short");
                PARSE_ASSERT(no_pattern_dublication(string_pattern, sub_url),
                        "Don't dublicate patten names: '/{id}/{id}' -> '/{id}/{other_id}'");

                node = node->addNode(sub_url, string_pattern);
                pattern_node_names.insert(sub_url);
                sub_url = "";
            } else {
                sub_url += ch;
            }
        }
        const char last_ch = url[url.size() - 1];
        balance = (last_ch == '}') ? balance - 1 : balance;
        const bool string_pattern = last_ch == '}';
        sub_url += last_ch;

        PARSE_ASSERT(balance == 0, "Use token '}' at the end like '/some/{id' -> '/some/{id}'");
        PARSE_ASSERT(sub_url.size() > 0, "Suburl too short");
        PARSE_ASSERT(no_pattern_dublication(string_pattern, sub_url),
                "Don't dublicate patten names: '/{id}/{id}' -> '/{id}/{other_id}'");

        node = node->addNode(sub_url, string_pattern);
        node->addTerminalNode(http_method, callback);
    }

public:
    TargetResolver() {}

    void addEndpoint(std::string&& url, std::string&& http_method, EndpointFunction callback) {
        checkUrlUnique(1, url, http_method, root);
        addUrlToTrie(std::move(url), std::move(http_method), callback);
    }

    std::function<std::shared_ptr<Response>()> getCallback(const std::string& url, const std::string& http_method) const {
        CallbackInfo info;
        std::shared_ptr<TerminalNode> terminal_node = getUrlParamsRecursive(1, url, http_method, root, info);
        BAD_REQUEST_ASSERT(terminal_node, "Expectod not nullptr terminal node");
        return [terminal_node, callback_info = std::move(info)]() mutable -> std::shared_ptr<Response> {
            return terminal_node->function(std::move(callback_info));
        };
    }

private:
    // TODO: change Node* to iterator -> universal iterator?
    std::shared_ptr<TerminalNode> getUrlParamsRecursive(
            const size_t pos, const std::string& url, const std::string& http_method, std::shared_ptr<Node> node, CallbackInfo& info) const {
        LOGIC_ASSERT(node, "Got nullptr node");
        size_t idx = url.find('/', pos);
        if (idx == std::string::npos) {
            idx = (pos >= url.size()) ? std::string::npos : url.size();
        }

        if (idx == std::string::npos) {
            return node->findTerminalNode(http_method);
        }

        const std::string suburl = url.substr(pos, idx - pos);
        std::shared_ptr<Node> named_node = node->findNamedNode(suburl);
        if (named_node) {
            std::shared_ptr<TerminalNode> result = getUrlParamsRecursive(idx + 1, url, http_method, named_node, info);
            if (result) {
                return result;
            }
        }

        const std::vector<std::shared_ptr<Node>>& pattern_node_result = node->findPatternNodes();
        for (int j = 0; j < pattern_node_result.size(); ++j) {
            std::shared_ptr<TerminalNode> result = getUrlParamsRecursive(idx + 1, url, http_method, pattern_node_result[j], info);
            if (!result) {
                continue;
            }
            info.url_params[node->name] = std::move(suburl);
            return result;
        }
        return {};
    }

    std::shared_ptr<Node> root = std::make_shared<Node>("root");
};

#endif  // MICROSERVICES__CORE__SRC__PRIVATE__TARGET_RESOLVER_H
