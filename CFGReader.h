#ifndef CFG_READER_H
#define CFG_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <iostream>
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
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            if (line.empty()) continue;

            size_t arrowPos = line.find("->");
            if (arrowPos == std::string::npos) continue;

            std::string lhsSymbol = line.substr(0, arrowPos);
            std::string rhsSymbols = line.substr(arrowPos + 2);
            //std::cout << "lhs: " << lhsSymbol << " rhs: " << rhsSymbols << std::endl;

            State lhs(lhsSymbol, "non-terminal");

            if( lhsSymbol == "S") { //if it is the start state
                State lhs = State(lhsSymbol, "non-terminal");    
            }
            else if ( lhsSymbol == "epsilon") { // if it is the epsilon
                State lhs = State(lhsSymbol, "epsilon");
            }
            else {
                
            }
            
            if( isNonTerminal(lhsSymbol) && cfg.isInTerminals(lhs) == false) {// Add symbol as non-terminal
                cfg.addNonTerminal(lhs); }
            

            std::stringstream ss(rhsSymbols);
            std::string production;
            while (std::getline(ss, production, '|')) {
                std::stringstream rhsStream(production);
                std::string symbol;
                std::vector<State> rhsStates;

                while (rhsStream >> symbol) {
                    //std::cout << "symbol: " << symbol << " : " <<  isNonTerminal(symbol) << std::endl;
                    if (isNonTerminal(symbol)) { // If it matches NonTerminal pattern
                        State nonTerminal(symbol, "non-terminal");
                        rhsStates.push_back(nonTerminal);
                        cfg.addNonTerminal(nonTerminal);
                    }
                    else { // Anything else is a Terminal
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
    //std::cout << "testing >>> symbol: " << symbol << std::endl;
    
    if (symbol == "S" && symbol.size() == 1) return true; // Special case for Start Symbol
    if (symbol.size() == 1 && std::isupper(symbol[0])) return true; // Single Capital Letter A-Z

    // Check pattern: A-Z followed by 0-9, ', or ` in any combination
    if (std::isupper(symbol[0])) {
        //std::cout << "\nitr-ing>>>  " << symbol[0] ;
        
        for (int i = 1; i < symbol.size(); ++i) {
            //std::cout << "\nitr-ing>>>  " << symbol[i] ;
            if (!(std::isdigit(symbol[i]) || symbol[i] != '\'' || symbol[i] != '`')) {
                return false; // If it's not a digit, ' or `
            }
        }
        
        //std::cout << " <<  \n" ;
        return true; // Valid Non-Terminal pattern
    }
    
    return false;
}


};

#endif // CFG_READER_H
