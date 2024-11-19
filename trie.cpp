#include "trie.h"

void Trie::insert(const std::string& word) {
    Node* current = root.get();
    for (char c : word) {
        if (!current->children[c]) {
            current->children[c] = std::make_unique<Node>();
        }
        current = current->children[c].get();
    }
    current->isEndOfWord = true;
}

bool Trie::contains(const std::string& word) const {
    Node* current = root.get();
    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }
    return current->isEndOfWord;
}

bool Trie::startsWith(const std::string& prefix) const {
    Node* current = root.get();
    for (char c : prefix) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }
    return true;
}

std::vector<std::string> Trie::getSuggestions(const std::string& prefix,
                                              int maxSuggestions) const {
    std::vector<std::string> suggestions;
    Node* current = root.get();

    for (char c : prefix) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return suggestions;
        }
        current = it->second.get();
    }

    std::string prefixStr = prefix;
    collectWords(current, prefixStr, suggestions, maxSuggestions);
    return suggestions;
}

void Trie::collectWords(Node* node, std::string& prefix,
                        std::vector<std::string>& results,
                        int maxResults) const {
    if (results.size() >= maxResults) return;

    if (node->isEndOfWord) {
        results.push_back(prefix);
    }

    for (const auto& [c, childNode] : node->children) {
        if (results.size() >= maxResults) break;
        prefix.push_back(c);
        collectWords(childNode.get(), prefix, results, maxResults);
        prefix.pop_back();
    }
}

void Trie::clear() {
    root = std::make_unique<Node>();
}
