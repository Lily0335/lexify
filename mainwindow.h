
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QTimer>
#include "dictionary.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onTextChanged(const QString &text);
    void onSearchClicked();
    void onAddWordClicked();
    void onRemoveWordClicked();
    void onSuggestionClicked(QListWidgetItem *item);

private:
    void initializeUI();
    void setupConnections();
    void updateSuggestions(const QString &text);
    void updateStatus(const QString &message);
    void removeSelectedWord();

    Dictionary dictionary;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLineEdit *searchBox;
    QPushButton *searchButton;
    QListWidget *suggestionList;
    QLabel *statusLabel;
    QPushButton *addWordButton;
    QPushButton *removeWordButton;
};

#endif // MAINWINDOW_H
