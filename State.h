#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <string>

//GLOBAL HELPER FUNCTION
bool CHECK_IF_NON_TERMINAL( std::string symbol) {
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

// classfor terminala nd non-terminal states
class State{
private:
    std::string symbol;
    std::string SymbolType; //terminal or non-terminal
    bool isTerminal; // Terminal is always a terminal
    bool isNonTerminal; // Terminal is never a non-terminal
    bool isEpsilon; // Terminal is never an epsilon
    bool isStartState; // Terminal is never a start state


public:



    State(std::string val , std::string type) : symbol(removeSpaces(val)), SymbolType(type) {
        SymbolType = type;
        if (type == "terminal") {
            isTerminal = true;
            isNonTerminal = false;
            isEpsilon = false;
            isStartState = false;
        }
        else if (type == "non-terminal") {
            isTerminal = false;
            isNonTerminal = true;
            isEpsilon = false;
            isStartState = false;
        }
        else if (type == "epsilon") {
            isTerminal = false;
            isNonTerminal = false;
            isEpsilon = true;
            isStartState = false;
        }
        else if (type == "start-state") {
            isTerminal = false;
            isNonTerminal = false;
            isEpsilon = false;
            isStartState = true;
        }

    }

        // Add this constructor
        State(const std::string& val) : symbol(removeSpaces(val)) {

                bool xx = CHECK_IF_NON_TERMINAL(val);
                
            if(xx){
                
                if(symbol == "S"){
                    SymbolType = "start-state";
                    isTerminal = false;
                    isNonTerminal = false;
                    isEpsilon = false;
                    isStartState = true;
                }
                else{
                SymbolType = "non-terminal";
                isTerminal = false;
                isNonTerminal = true;
                isEpsilon = false;
                isStartState = false;
            }
            }
            else if(symbol == "epsilon"){
                SymbolType = "epsilon";
                isTerminal = false;
                isNonTerminal = false;
                isEpsilon = true;
                isStartState = false;      
            }
            else{
                SymbolType = "terminal";
                isTerminal = true;
                isNonTerminal = false;
                isEpsilon = false;
                isStartState = false;

        }
    }


    std::string getSymbol() const { return symbol; }
    std::string getSymbol() { return symbol; }
    std::string getType() const { return SymbolType; }
    void showSymbol() const { std::cout << symbol; }
    void showType() const { std::cout << SymbolType; }
    //bool isTerminal() { return isTerminal; }

    void setSymbol(std::string val) { symbol = removeSpaces(val); }
    void setType(std::string type) { SymbolType = type; }
    
    bool operator==(const State& other) const {
        // Compare relevant members of the State class
        // Replace 'member' with the actual member(s) to compare
        return this->symbol == other.symbol;
    }

    std::string removeSpaces(const std::string& str) {
        std::string result;
        for (char ch : str) {
            if (ch != ' ') {
                result += ch;
            }
        }
        return result;
    }
    
};

#endif // TERMINAL_H
