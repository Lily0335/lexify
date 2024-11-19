
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <set>
#include <vector>
#include <Qstring>
#include "trie.h"

class Dictionary {
public:
    Dictionary(const QString& path = "dictionary.txt") : filePath(path) {
        initializeDefaultWords();
        loadWordsFromFile();
    }

    bool addWord(const std::string& word);
    bool removeWord(const std::string& word);
    bool exists(const std::string& word) const;
    bool checkGrammar(const std::string& word) const;
    std::vector<std::string> getSuggestions(const std::string& prefix, int maxSuggestions = 10) const;
    std::vector<std::string> getSpellingSuggestions(const std::string& word, int maxDistance = 2) const;

private:
    std::set<std::string> wordSet;
    Trie trie;
    QString filePath;

    void initializeDefaultWords();
    void loadWordsFromFile();
    void saveWordsToFile() const;
    bool isValidWord(const std::string& word) const;
    int getLevenshteinDistance(const std::string& s1, const std::string& s2) const;
};

#endif // DICTIONARY_H
