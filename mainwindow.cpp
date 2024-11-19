

#include "mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initializeUI();
    setupConnections();
}

void MainWindow::initializeUI() {
    setWindowTitle("Dictionary Search");
    setMinimumSize(600, 400);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    // Project title
    QLabel *projectTitle = new QLabel("Lexify: Smart Dictionary System", this);
    projectTitle->setAlignment(Qt::AlignCenter);
    projectTitle->setFont(QFont("Arial", 20, QFont::Bold)); // Customize font size and style
    projectTitle->setStyleSheet("color: #333; margin-bottom: 20px;"); // Styling for the text
    mainLayout->addWidget(projectTitle);
    // Search box and button layout
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Enter a word to search...");
    searchBox->setFont(QFont("Arial", 12));
    searchButton = new QPushButton("Search", this);
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);
    mainLayout->addLayout(searchLayout);

    // Suggestions list
    suggestionList = new QListWidget(this);
    suggestionList->setFont(QFont("Arial", 11));
    suggestionList->setAlternatingRowColors(true);
    mainLayout->addWidget(suggestionList);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setFont(QFont("Arial", 10));
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Add word button
    addWordButton = new QPushButton("Add New Word", this);
    buttonLayout->addWidget(addWordButton);

    // Remove word button
    removeWordButton = new QPushButton("Remove Word", this);
    removeWordButton->setEnabled(false);
    removeWordButton->setObjectName("removeWordButton");
    buttonLayout->addWidget(removeWordButton);

    mainLayout->addLayout(buttonLayout);

    // Set stylesheet
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f0f0;
        }
        QLineEdit {
            border: 2px solid #ccc;
            border-radius: 5px;
            padding: 5px;
            background-color: white;
        }
        QLineEdit:focus {
            border-color: #0078d4;
        }
        QListWidget {
            border: 1px solid #ccc;
            border-radius: 5px;
            background-color: white;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #eee;
        }
        QListWidget::item:selected {
            background-color: #0078d4;
            color: white;
        }
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 16px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #006cbd;
        }
        QPushButton:pressed {
            background-color: #005ca3;
        }
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
        QPushButton#removeWordButton {
            background-color: #d43d3d;
        }
        QPushButton#removeWordButton:hover {
            background-color: #bd3636;
        }
QPushButton#removeWordButton:pressed {
            background-color: #a62f2f;
        }
        QLabel {
            color: #666666;
        }
    )");
}

void MainWindow::setupConnections() {
    connect(searchBox, &QLineEdit::textChanged,
            this, &MainWindow::onTextChanged);
    connect(searchButton, &QPushButton::clicked,
            this, &MainWindow::onSearchClicked);
    connect(addWordButton, &QPushButton::clicked,
            this, &MainWindow::onAddWordClicked);
    connect(removeWordButton, &QPushButton::clicked,
            this, &MainWindow::onRemoveWordClicked);
    connect(suggestionList, &QListWidget::itemClicked,
            this, &MainWindow::onSuggestionClicked);
    connect(suggestionList, &QListWidget::itemSelectionChanged, [this]() {
        removeWordButton->setEnabled(!suggestionList->selectedItems().isEmpty());
    });
}

void MainWindow::onTextChanged(const QString &text) {
    if (text.isEmpty()) {
        suggestionList->clear();
        updateStatus("");
        return;
    }

    updateSuggestions(text);
}


void MainWindow::onSearchClicked() {
    QString word = searchBox->text().trimmed();
    if (word.isEmpty()) {
        updateStatus("Please enter a word to search");
        return;
    }

    std::string stdWord = word.toStdString();
    bool exists = dictionary.exists(stdWord);

    if (exists) {
        updateStatus("Word exists in dictionary");
    } else {
        // Get spelling suggestions for incorrect words
        std::vector<std::string> spellingSuggestions = dictionary.getSpellingSuggestions(stdWord);

        suggestionList->clear();
        if (!spellingSuggestions.empty()) {
            updateStatus("Word not found. Did you mean:");
            for (const auto& suggestion : spellingSuggestions) {
                suggestionList->addItem(QString::fromStdString(suggestion));
            }
        } else {
            updateStatus("Word not found and no similar words found");
        }
    }
}

void MainWindow::onAddWordClicked() {
    bool ok;
    QString word = QInputDialog::getText(this, "Add New Word",
                                         "Enter the word to add:",
                                         QLineEdit::Normal, "", &ok);
    if (ok && !word.isEmpty()) {
        // Convert to lowercase to match dictionary's internal logic
        std::string lowercaseWord = word.toLower().toStdString();

        if (dictionary.exists(lowercaseWord)) {
            QMessageBox::information(this, "Word Exists",
                                     "This word already exists in the dictionary.");
            return;
        }

        if (dictionary.addWord(lowercaseWord)) {
            // Multiple ways to provide feedback
            updateStatus("Word added successfully!");

            // Also use a message box for more visibility
            QMessageBox::information(this, "Word Added",
                                     QString("'%1' was successfully added to the dictionary!").arg(word));

            // Update suggestions and clear search box
            searchBox->clear();
            updateSuggestions("");
        } else {
            QMessageBox::warning(this, "Invalid Word",
                                 "The word could not be added. Please ensure it:\n"
                                 "- Contains only letters\n"
                                 "- Is between 2 and 45 characters\n"
                                 "- Does not contain triple repeated letters");
        }
    }
}

void MainWindow::onRemoveWordClicked() {
    removeSelectedWord();
}

void MainWindow::removeSelectedWord() {
    QList<QListWidgetItem*> selectedItems = suggestionList->selectedItems();
    if (selectedItems.isEmpty()) return;

    QString word = selectedItems[0]->text();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Removal",
        QString("Are you sure you want to remove '%1' from the dictionary?").arg(word),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (dictionary.removeWord(word.toStdString())) {
            updateStatus("Word removed successfully!");
            updateSuggestions(searchBox->text());
        } else {
            updateStatus("Failed to remove word.");
        }
    }
}

void MainWindow::onSuggestionClicked(QListWidgetItem *item) {
    searchBox->setText(item->text());
    updateStatus("Selected word: " + item->text());
}

void MainWindow::updateSuggestions(const QString &text) {
    suggestionList->clear();

    if (text.isEmpty()) return;

    auto suggestions = dictionary.getSuggestions(text.toStdString());
    for (const auto& suggestion : suggestions) {
        suggestionList->addItem(QString::fromStdString(suggestion));
    }

    if (suggestions.empty()) {
        updateStatus("No suggestions found");
    } else {
        updateStatus("Showing suggestions");
    }
}

void MainWindow::updateStatus(const QString &message) {
    statusLabel->setText(message);
}



