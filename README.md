

---

# **Lexify: Smart Dictionary System** 🧠📖  
A modern, smart dictionary application that simplifies word management, enhances vocabulary, and provides intelligent features like autocomplete and grammar validation.

---

## 🚀 **Features**  
- **Word Autocomplete**: Suggests relevant words based on prefixes in real time.  
- **Grammar Validation**: Ensures that new words meet predefined grammatical rules.  
- **Persistent Storage**: Save and load words from files for consistent data management.  
- **Interactive UI**: User-friendly interface for searching, validating, and adding words.

---

## 📂 **Code Structure**  

### **Core Components**  
The project is modular, with the following main components:  

1. **Trie Class** (`trie.h` and `trie.cpp`)  
   - Implements the prefix tree for efficient word storage and retrieval.  
   - Key Functions:  
     - `insert(word)`: Adds a word to the trie.  
     - `search(word)`: Checks if a word exists in the trie.  
     - `getSuggestions(prefix)`: Returns a list of words starting with a given prefix.  

2. **Dictionary Class** (`dictionary.h` and `dictionary.cpp`)  
   - Acts as the bridge between the trie and the application logic.  
   - Manages operations like file saving/loading, validation, and grammar checks.  
   - Key Functions:  
     - `loadFromFile(filePath)`: Loads words into the trie from a file.  
     - `saveToFile(filePath)`: Saves current trie data to a file.  
     - `addWord(word)`: Adds a validated word to the dictionary.  
     - `validateWord(word)`: Ensures the word adheres to grammatical rules.  

3. **MainWindow Class** (`mainwindow.h` and `mainwindow.cpp`)  
   - Builds the graphical user interface using the Qt framework.  
   - Handles user interactions like word search, validation, and addition.  
   - Key Functions:  
     - `on_searchButton_clicked()`: Displays autocomplete suggestions for a prefix.  
     - `on_addButton_clicked()`: Adds a new word after validation.  
     - `on_saveButton_clicked()`: Triggers saving data to a file.  

---

## 📐 Data Structures and Algorithms

Lexify is powered by:  

Trie (Prefix Tree)
- Efficient for storing and retrieving words with prefix-based queries.  
  Time Complexity:  
  - Insert: `O(L)` (L = Length of the word).  
  - Search: `O(L)` (L = Length of the word).  
  Why Trie?  
  Unlike Hash Maps, which lack prefix-based search capabilities, Tries enable efficient autocompletion and storage of large vocabularies.  

## Set  
- Used for quick existence checks of words during file loading and validation.

---

## 📊 Workflow

1. **Word Initialization**: Load default words into the system.  
2. **User Search**: Enter a prefix to get instant suggestions.  
3. **Add Words**: Validate and add new words through the interactive UI.  
4. **Save/Load Data**: Ensure data persistence by integrating file operations.
  ### Diagram

![workflow diagram 1](https://github.com/Lily0335/lexify/blob/main/images/workflow-1.png)


![workflow diagram 2](https://github.com/Lily0335/lexify/blob/main/images/workflow-2.png)


---

## 🎨 **Technology Stack**  

- **Programming Language**: C++  
- **Framework**: Qt (for building the interactive user interface)  
- **Data Structures**: Trie, Set  

---

## 🖥️ **Getting Started**  

### **Prerequisites**  
- Qt Framework installed.  
- C++ compiler supporting modern C++ standards (C++11 or above).  

---

## 📂 **Folder Structure**  

```
lexify-smart-dictionary/  
├── src/  
│   ├── trie.h  
│   ├── trie.cpp  
│   ├── dictionary.h  
│   ├── dictionary.cpp  
│   ├── mainwindow.h  
│   ├── mainwindow.cpp  
│   ├── main.cpp  
│   └── resources/  
│       └── words.txt      # Default word list  
├── ui/  
│   └── mainwindow.ui      # Qt UI design file  
├── docs/  
│   └── README.md          # Project documentation  
└── LICENSE  
```  

---

## 🛠️ **Future Enhancements**  

- Multilingual support for broader accessibility.  
- Voice-based interaction for seamless user experience.  
- Advanced grammar analysis tools.  


---


## 📬 **Contact**  

For queries or feedback:  
- **Name**: Laraib Anwar  
- **Email**: [laraibanwar00002@gmail.com]  
- **LinkedIn**: [https://www.linkedin.com/posts/laraib-anwar-062553296_codingjourney-lexify-qt-activity-7264770051336007680-9kbc?utm_source=share&utm_medium=member_desktop]  

