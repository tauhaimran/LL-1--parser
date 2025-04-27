#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include "CFG.h"

class ParsingTable {
private:
    std::vector<std::vector<std::string>> table;
    std::vector<State> nonTerminals;
    std::vector<State> terminals;

    // Helper function to find the index of a non-terminal by symbol
    int findNonTerminalIndex(const std::string& symbol) const {
        for (int i = 0; i < nonTerminals.size(); ++i) {
            if (nonTerminals[i].getSymbol() == symbol) {
                return i;
            }
        }
        return -1;
    }

    // Helper function to find the index of a terminal by symbol, or $ for the last column
    int findTerminalIndex(const std::string& symbol) const {
        // Special case for end marker
        if (symbol == "$") {
            return terminals.size(); // $ is the last column
        }
        
        for (int i = 0; i < terminals.size(); ++i) {
            if (terminals[i].getSymbol() == symbol) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor to initialize the table
    ParsingTable(const std::vector<std::vector<std::string>>& tableData, const std::vector<State>& nonTerminals, const std::vector<State>& terminals)
        : table(tableData), nonTerminals(nonTerminals), terminals(terminals) {
        // tableData is assumed to be correctly sized from CFG::computeLL1ParsingTable
    }

    // Add entry to parsing table
    void addEntry(const std::string& nonTerminal, const std::string& terminal, const std::string& production) {
        int row = findNonTerminalIndex(nonTerminal);
        int col = findTerminalIndex(terminal);

        if (row == -1 || col == -1) return;

        if (table[row][col].empty()) {
            table[row][col] = production; // Add production rule to the table
        } else {
            table[row][col] += " | " + production; // Handle conflicts by showing alternatives
        }
    }

    // Get the production rule from the table
    std::string getEntry(const std::string& nonTerminal, const std::string& terminal) const {
        int row = findNonTerminalIndex(nonTerminal);
        int col = findTerminalIndex(terminal);

        if (row == -1 || col == -1) return "";
        return table[row][col];
    }

    // Get the list of terminal symbols
    std::vector<std::string> getTerminals() const {
        std::vector<std::string> symbols;
        for (const auto& state : terminals) {
            symbols.push_back(state.getSymbol());
        }
        return symbols;
    }

    // Display the parsing table
    void display() const {
        std::cout << "\nLL(1) Parsing Table:\n";
        std::cout << "Non-Terminal\\Terminal\t";

        for (const State& terminal : terminals) {
            std::cout << terminal.getSymbol() << "\t";
        }
        std::cout << "$\t"; // Include the end-of-input marker
        std::cout << "\n";

        for (int i = 0; i < nonTerminals.size(); ++i) {
            std::cout << nonTerminals[i].getSymbol() << "\t\t";
            for (int j = 0; j < terminals.size() + 1; ++j) { // +1 for the $ column
                std::cout << (table[i][j].empty() ? "-" : table[i][j]) << "\t";
            }
            std::cout << "\n";
        }
    }
};

#endif