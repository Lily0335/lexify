// trie.h
#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Trie {
private:
    struct Node {
        std::unordered_map<char, std::unique_ptr<Node>> children;
        bool isEndOfWord;
        Node() : isEndOfWord(false) {}
    };

    std::unique_ptr<Node> root;

    void collectWords(Node* node, std::string& prefix,
                      std::vector<std::string>& results,
                      int maxResults) const;

public:
    Trie() : root(std::make_unique<Node>()) {}

    void insert(const std::string& word);
    bool contains(const std::string& word) const;
    bool startsWith(const std::string& prefix) const;
    std::vector<std::string> getSuggestions(const std::string& prefix,
                                            int maxSuggestions = 10) const;
    void clear();
};
#endif
