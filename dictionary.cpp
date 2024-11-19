
#include "dictionary.h"
#include <algorithm>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <regex>

void Dictionary::initializeDefaultWords() {
    const std::vector<std::string> defaultWords = {
        "apple", "banana", "cat", "dog", "elephant","happybirthday",
        "land","coat","cartoons","meals","holiday","honour","zebra","gun","pencil"
        "fish", "giraffe", "house", "ice", "jacket",
        "king", "lion", "monkey", "nest", "orange",
        "pencil", "queen", "rabbit", "snake", "tiger",
        "umbrella", "violet", "water", "xylophone", "yellow",
        "zebra", "book", "computer", "desk", "email",
        "folder", "garden", "hammer", "internet", "juice",
        "keyboard", "laptop", "mouse", "network", "phone",
        "printer", "router", "screen", "tablet", "website"
    };

    for (const auto& word : defaultWords) {
        addWord(word);
    }
}

void Dictionary::loadWordsFromFile() {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                addWord(line.toStdString());
            }
        }
        file.close();
    }
}

void Dictionary::saveWordsToFile() const {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& word : wordSet) {
            out << QString::fromStdString(word) << "\n";
        }
        file.close();
    }
}

bool Dictionary::isValidWord(const std::string& word) const {

    if (word.empty()) return false;

        // Check if all characters are letters
        for (char c : word) {
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                return false;
            }
    }

        // Additional checks for word length (optional)
        if (word.length() < 2 || word.length() > 45) {
            return false;
        }

        return true;
}


bool Dictionary::exists(const std::string& word) const {
    return wordSet.find(word) != wordSet.end();
}

bool Dictionary::removeWord(const std::string& word) {
    std::string lowercaseWord = word;
    std::transform(lowercaseWord.begin(), lowercaseWord.end(),
                   lowercaseWord.begin(), ::tolower);

    if (!exists(lowercaseWord)) {
        return false;
    }

    wordSet.erase(lowercaseWord);

    // Clear and rebuild trie
    trie.clear();
    for (const auto& w : wordSet) {
        trie.insert(w);
    }

    saveWordsToFile();
    return true;
}

int Dictionary::getLevenshteinDistance(const std::string& s1, const std::string& s2) const {
    const std::size_t len1 = s1.size();
    const std::size_t len2 = s2.size();
    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++)
        d[i][0] = i;
    for (int j = 0; j <= len2; j++)
        d[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            d[i][j] = std::min({
                d[i - 1][j] + 1,
                d[i][j - 1] + 1,
                d[i - 1][j - 1] + (s1[i - 1] != s2[j - 1])
            });
        }
    }
    return d[len1][len2];
}


bool Dictionary::checkGrammar(const std::string& word) const {
    if (word.empty()) return false;
    if (!isValidWord(word)) return false;

    for (size_t i = 2; i < word.length(); ++i) {
        if (word[i] == word[i-1] && word[i] == word[i-2]) {
            return false;
        }
    }

    if (word.length() < 2 || word.length() > 45) return false;
    return true;
}

bool Dictionary::addWord(const std::string& word) {
    std::string lowercaseWord = word;
    std::transform(lowercaseWord.begin(), lowercaseWord.end(),
                   lowercaseWord.begin(), ::tolower);

    if (!checkGrammar(lowercaseWord)) {
        return false;
    }

    wordSet.insert(lowercaseWord);
    trie.insert(lowercaseWord);
    saveWordsToFile();
    return true;
}
std::vector<std::string> Dictionary::getSpellingSuggestions(const std::string& word, int maxDistance) const {
    std::vector<std::pair<std::string, int>> suggestions;

    // Check all words in dictionary for potential matches
    for (const auto& dictWord : wordSet) {
        int distance = getLevenshteinDistance(word, dictWord);
        if (distance <= maxDistance) {
            suggestions.push_back({dictWord, distance});
        }
    }

    // Sort by distance (closest matches first)
    std::sort(suggestions.begin(), suggestions.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // Extract just the words
    std::vector<std::string> result;
    for (const auto& suggestion : suggestions) {
        result.push_back(suggestion.first);
    }

    return result;
}


std::vector<std::string> Dictionary::getSuggestions(const std::string& prefix,
                                                    int maxSuggestions) const {
    std::string lowercasePrefix = prefix;
    std::transform(lowercasePrefix.begin(), lowercasePrefix.end(),
                   lowercasePrefix.begin(), ::tolower);

    return trie.getSuggestions(lowercasePrefix, maxSuggestions);
}


