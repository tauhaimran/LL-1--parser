#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include "CFG.h"

class ParsingTable {
private:
    std::vector<std::vector<std::string>> table;
    std::vector<std::string> nonTerminals;
    std::vector<std::string> terminals;

    // Helper function to find the index of a symbol (NonTerminal or Terminal)
    int findIndex(const std::vector<std::string>& symbols, const std::string& symbol) {
        for (int i = 0; i < symbols.size(); ++i) {
            if (symbols[i] == symbol) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor to initialize the table
    ParsingTable(const std::vector<std::string>& nonTerminals, const std::vector<std::string>& terminals)
        : nonTerminals(nonTerminals), terminals(terminals) {
        table.resize(nonTerminals.size(), std::vector<std::string>(terminals.size(), ""));
    }

    // Add entry to parsing table
    void addEntry(const std::string& nonTerminal, const std::string& terminal, const std::string& production) {
        int row = findIndex(nonTerminals, nonTerminal);
        int col = findIndex(terminals, terminal);

        if (row == -1 || col == -1) return;

        if (table[row][col].empty()) {
            table[row][col] = production; // Add production rule to the table
        } else {
            table[row][col] += " | " + production; // Handle conflicts by showing alternatives
        }
    }

    // Display the parsing table
    void display() const {
        std::cout << "\nLL(1) Parsing Table:\n";
        std::cout << "Non-Terminal\\Terminal\t";
        
        for (const std::string& terminal : terminals) {
            std::cout << terminal << "\t";
        }
        std::cout << "\n";

        for (int i = 0; i < nonTerminals.size(); ++i) {
            std::cout << nonTerminals[i] << "\t\t";
            for (int j = 0; j < terminals.size(); ++j) {
                std::cout << (table[i][j].empty() ? "-" : table[i][j]) << "\t";
            }
            std::cout << "\n";
        }
    }
};

#endif
