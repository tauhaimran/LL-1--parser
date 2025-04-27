#ifndef CFG_READER_H
#define CFG_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "CFG.h"
#include "ProductionRule.h"
#include "State.h"

class CFGReader {
public:
    static bool readFromFile(const std::string& filename, CFG& cfg) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        bool firstRule = true; // To identify the start symbol from the first rule
        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            if (line.empty()) continue;

            size_t arrowPos = line.find("->");
            if (arrowPos == std::string::npos) continue;

            std::string lhsSymbol = line.substr(0, arrowPos);
            std::string rhsSymbols = line.substr(arrowPos + 2);

            // LHS is always a non-terminal; set type based on whether it's the start symbol
            State lhs(lhsSymbol, firstRule ? "start-state" : "non-terminal");
            cfg.addNonTerminal(lhs);
            if (firstRule) firstRule = false; // Only the first LHS is the start symbol

            std::stringstream ss(rhsSymbols);
            std::string production;
            while (std::getline(ss, production, '|')) {
                std::stringstream rhsStream(production);
                std::string symbol;
                std::vector<State> rhsStates;

                while (rhsStream >> symbol) {
                    if (symbol == "ε" || symbol == "epsilon") {
                        State epsilon("epsilon", "terminal");
                        rhsStates.push_back(epsilon);
                        cfg.addTerminal(epsilon);
                    } else if (isNonTerminal(symbol)) {
                        State nonTerminal(symbol, "non-terminal");
                        rhsStates.push_back(nonTerminal);
                        cfg.addNonTerminal(nonTerminal);
                    } else {
                        State terminal(symbol, "terminal");
                        rhsStates.push_back(terminal);
                        cfg.addTerminal(terminal);
                    }
                }
                cfg.addProductionRule(ProductionRule(lhs, rhsStates));
            }
        }
        file.close();
        return true;
    }

private:
    static bool isNonTerminal(const std::string& symbol) {
        // Non-terminals: Start with uppercase letter, optionally followed by letters, digits, ', or `
        if (symbol.empty() || !std::isupper(symbol[0])) return false;
        for (size_t i = 1; i < symbol.length(); ++i) {
            char c = symbol[i];
            if (!std::isalnum(c) && c != '\'' && c != '`') return false;
        }
        return true;
    }
};

#endif // CFG_READER_H