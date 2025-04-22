#ifndef CFG_H
#define CFG_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "State.h"
#include "ProductionRule.h"
#include "StateSet.h"
#include "ParsingTable.h"
    
class CFG {
private:
    std::vector<State> nonTerminals;   // Stores all non-terminal states
    std::vector<State> terminals;      // Stores all terminal states
    std::vector<ProductionRule> productionRules; // Stores all production rules
    std::vector<StateSet> firstSets;    // Stores all state sets
    std::vector<StateSet> followSets;   // Stores all follow sets
    std::vector<State> leftOverStates;   // temp storage for computation...
    State startSymbol;                 // Stores the starting symbol of the grammar


public:
    // Constructor to initialize the CFG with a starting symbol
    CFG(const State& startSymbol) : startSymbol(startSymbol) {}

    // Adds a non-terminal state to the grammar if it doesn't already exist
    void addNonTerminal(const State& nonTerminal) { 
        if( isInNonTerminals(nonTerminal) == false) {
            nonTerminals.push_back(nonTerminal); 
        }
     }

    // Adds a terminal state to the grammar if it doesn't already exist
    void addTerminal(const State& terminal) { 
        if (isInTerminals(terminal) == false) {
            terminals.push_back(terminal); 
        }
     }

    // Adds a production rule to the grammar
    void addProductionRule(const ProductionRule& rule) { productionRules.push_back(rule); }

    // Displays the start symbol and all production rules in the grammar
    void display() const {
        std::cout << "Start Symbol: ";
        startSymbol.showSymbol();
        std::cout << std::endl << "Production Rules:" << std::endl;

        for (const auto& rule : productionRules) {
            rule.display();
        }
    }

    // Displays all non-terminal states in the grammar
    void showNonTerminals() const {
        std::cout << "Non-terminals: \n";
        for (const auto& nonTerminal : nonTerminals) {
            nonTerminal.showSymbol();
            std::cout << "\t(" ;
            nonTerminal.showType();
            std::cout << ") , ";
            std::cout << " \n";
        }
        std::cout << std::endl;
    }

    // Displays all terminal states in the grammar
    void showTerminals() const {
        std::cout << "Terminals: \n";
        for (const auto& terminal : terminals) {
            terminal.showSymbol();
            std::cout << "\t(" ;
            terminal.showType();
            std::cout << ") , ";
            std::cout << " \n";
        }
        std::cout << std::endl;
    }

    // Checks if a given state exists in the terminals list
    bool isInTerminals(const State& state) const {
        for (const auto& terminal : terminals) {
            if (terminal == state) return true;
        }
        return false;
    }

    // Checks if a given state exists in the non-terminals list
    bool isInNonTerminals(const State& state) const {
        for (const auto& nonTerminal : nonTerminals) {
            if (nonTerminal == state) return true;
        }
        return false;
    }

    // Function to remove a rule from the productionRules vector by comparing its LHS and RHS
    void removeRule(const ProductionRule& targetRule) {
        //std::cout << "removing rule : "; targetRule.display();
        if (productionRules.empty()) return;
        for (size_t i = 0; i < productionRules.size(); ++i) {
            if (productionRules[i].getLHS().getSymbol() == targetRule.getLHS().getSymbol() &&
                productionRules[i].getRHS() == targetRule.getRHS()) {
                productionRules.erase(productionRules.begin() + i);
                return; // Exit after deleting the rule
            }
        }
    }


    // Step # 1
    //FUNCTION  for left Factoring
    void leftFactoring(){

    //FOR each NonTerminal in nonTerminals
    for (int x = 0 ; x < nonTerminals.size() ; x++){
    //for (const auto& nonTerminal : nonTerminals) {
        const auto& nonTerminal = nonTerminals[x];
        std::vector<ProductionRule> RULES_FOR_NONTERMINAL;
        //std::cout << "------> checking for... " << nonTerminal.getSymbol() << std::endl;

        //Collect all ProductionRules associated with NonTerminal
        for (const auto& rule : productionRules) {
            if (rule.getLHS() == nonTerminal) {
                RULES_FOR_NONTERMINAL.push_back(rule);
            }
        }
        //std::cout<< " got all rules for state : " << nonTerminal.getSymbol() << std::endl;

        // Compare each pair of production rules for common prefixes
        //FOR each rule_j in RULES_FOR_NONTERMINAL
        for (int j = 0; j < RULES_FOR_NONTERMINAL.size(); j++) {
            ProductionRule rule_j = RULES_FOR_NONTERMINAL[j];
            //FOR each rule_k after rule_j in RULES_FOR_NONTERMINAL
            for (int k = j + 1; k < RULES_FOR_NONTERMINAL.size(); k++) {
                ProductionRule rule_k = RULES_FOR_NONTERMINAL[k];
                std::vector<State> prefix, remainingJ, remainingK;//PREFIX = []
                int INDEX = 0;//INDEX = 0

                //std::cout << "--------------> here 1\n";

                // Compare symbols until mismatch
                //WHILE INDEX < min(size of rule_j, size of rule_k) AND 
                while(INDEX < std::min(rule_j.getRHS().size(), rule_k.getRHS().size()) &&
                
                rule_j.getRHS()[INDEX] == rule_k.getRHS()[INDEX]) {
                      //std::cout << "--------------> here2 " << INDEX << "\n" ;
                        if(rule_j.getRHS()[INDEX] == rule_k.getRHS()[INDEX]) //if prefixes match..
                      { //ADD rule_j[INDEX] to PREFIX
                          prefix.push_back(rule_j.getRHS()[INDEX]);
                          //std::cout << "--------------> here3 " << INDEX << "\n" ;
                      }
                      
                    
                    //std::cout << "--------------> here4 " << INDEX << "\n" ;
                    //prefix.push_back(rule_j.getRHS()[INDEX]);
                    //std::cout << "--------------> here5 " << INDEX << "\n" ;

                    INDEX++; //INCREMENT INDEX
                }

                //std::cout << "got all states after same prefix " << std::endl;

                

                //IF PREFIX is NOT empty
                if(!prefix.empty()) {
                    // Create a new NonTerminal for the factored part
                    State newNonTerminal = nonTerminal;
                    //NEW_NONTERMINAL = NonTerminal + "'"
                    newNonTerminal.setSymbol(newNonTerminal.getSymbol() + "'");
                    //update prefix
                    //prefix.pop_back();

                    //it repeats for a bunc of stuff..
                    //if(prefix.size()>1){
                      //  prefix.pop_back();
                    //}

                    prefix.push_back(newNonTerminal);
                    //MAKE NEW PRODUCTION RULE
                    ProductionRule newProductionRule(nonTerminal, prefix);
                    //ADD NEW PRODUCTION RULE to productionRules
                    productionRules.push_back(newProductionRule);
                    //REMOVE rule_j and rule_k from productionRules
                    //productionRules.erase(productionRules.begin() + j);
                    //productionRules.erase(productionRules.begin() + k);
                    //ADD NEW_NONTERMINAL to nonTerminals
                    addNonTerminal(newNonTerminal);

                    // remove rules that had common prefixes
                    //productionRules.erase(productionRules.begin() + j);
                    //productionRules.erase(productionRules.begin() + k);

                    //REMOVE rule_j and rule_k from RULES_FOR_NONTERMINAL
                    RULES_FOR_NONTERMINAL.erase(RULES_FOR_NONTERMINAL.begin() + j); //REMOVE rule_j from RULES_FOR_NONTERMINAL
                    RULES_FOR_NONTERMINAL.erase(RULES_FOR_NONTERMINAL.begin() + k); //REMOVE rule_k from RULES_FOR_NONTERMINAL



                    // Prepare remaining parts of rule_j and rule_k
                     // Prepare remaining parts for both rules
                     remainingJ.assign(rule_j.getRHS().begin() + INDEX, rule_j.getRHS().end()); //REMAINING_J = remaining symbols of rule_j after INDEX
                     remainingK.assign(rule_k.getRHS().begin() + INDEX, rule_k.getRHS().end()); //REMAINING_K = remaining symbols of rule_k after INDEX

                    //IF REMAINING_J is empty, ADD ε (epsilon) to REMAINING_J
                    if(remainingJ.empty()) {
                        State epsilon( "epsilon" , "terminal");
                        remainingJ.push_back(epsilon);
                    }
                    //IF REMAINING_K is empty, ADD ε (epsilon) to REMAINING_K
                    if(remainingK.empty()) {
                        //std::cout << "Adding epsilon to remainingK" << std::endl;
                        State epsilon( "epsilon" , "terminal");
                        remainingK.push_back(epsilon);
                    }

                    // Add the new production rules to grammar
                   // productionRules.push_back(ProductionRule(nonTerminal, prefix)); //ADD ProductionRule(NonTerminal -> PREFIX + NEW_NONTERMINAL) to productionRules
                    productionRules.push_back(ProductionRule(newNonTerminal, remainingJ)); //ADD ProductionRule(NEW_NONTERMINAL -> REMAINING_J) to productionRules
                    productionRules.push_back(ProductionRule(newNonTerminal, remainingK)); //ADD ProductionRule(NEW_NONTERMINAL -> REMAINING_K) to productionRules

                    // Remove the original conflicting rules from grammar
                    // Remove rule_j and rule_k from the original productionRules vector
                    removeRule(rule_j);
                    removeRule(rule_k);
                    //break; // Break out of the loop after removing the rules

                    // Restart comparison for updated list
                    j = 0;
                    k = 0;
                    //k = j; //k = j .. still tryna ficure out how this part works...
                }//ENDIF
            }//ENDFOR
         } //ENDFOR
    } //ENDFOR
}//END FUNCTION


// Step 2
// FUNCTION to eliminate left recursion
void eliminateLeftRecursion() {
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        State Ai = nonTerminals[i];

        // Step 2: Eliminate Direct Left Recursion
        std::vector<ProductionRule> recursiveRules;
        std::vector<ProductionRule> nonRecursiveRules;

        for (const auto& rule : productionRules) {
            if (rule.getLHS() == Ai) {
                if (rule.getRHS()[0] == Ai) {
                    recursiveRules.push_back(rule);
                } else {
                    nonRecursiveRules.push_back(rule);
                }
            }
        }

        if (!recursiveRules.empty()) {
            State AiDash(Ai.getSymbol() + "'", "Non-Terminal");
            addNonTerminal(AiDash);

            std::vector<ProductionRule> newRules;

            // Create rules for Ai'
            for (const auto& rule : recursiveRules) {
                std::vector<State> newRHS(rule.getRHS().begin() + 1, rule.getRHS().end());
                newRHS.push_back(AiDash);
                newRules.push_back(ProductionRule(AiDash, newRHS));
            }
            // Adding epsilon rule to Ai'
            newRules.push_back(ProductionRule(AiDash, {State("epsilon", "Terminal")}));

            // Create modified rules for Ai
            for (const auto& rule : nonRecursiveRules) {
                std::vector<State> newRHS = rule.getRHS();
                newRHS.push_back(AiDash);
                newRules.push_back(ProductionRule(Ai, newRHS));
            }

            // Remove the original recursive rules and add new ones
            productionRules = newRules;
        }

        // Step 1: Replace Indirect Left Recursion
        for (size_t j = 0; j < i; ++j) {
            State Aj = nonTerminals[j];
            std::vector<ProductionRule> newRules;

            for (const auto& rule : productionRules) {
                if (rule.getLHS() == Ai && rule.getRHS()[0] == Aj) {
                    std::vector<ProductionRule> AjRules;
                    
                    // Find all rules of Aj
                    for (const auto& r : productionRules) {
                        if (r.getLHS() == Aj) AjRules.push_back(r);
                    }

                    // Replace Aj in Ai's rules
                    for (const auto& AjRule : AjRules) {
                        std::vector<State> newRHS = AjRule.getRHS();
                        newRHS.insert(newRHS.end(), rule.getRHS().begin() + 1, rule.getRHS().end());
                        newRules.push_back(ProductionRule(Ai, newRHS));
                    }
                } else {
                    newRules.push_back(rule);
                }
            }

            // Update production rules with newRules
            productionRules = newRules;
        }

        
    }
}

// step # 3 
// FUNCTION to compute the FIRST set for a given symbol 
void computeALLFirstSets() {

    for (const auto& nonTerminal : nonTerminals) {
        //std::cout << "Computing follow set for: " << nonTerminal.getSymbol() << std::endl;
        firstSets.push_back(StateSet(nonTerminal));
    }

    for (const auto& nonTerminal : nonTerminals) {
        computeFirstSet(nonTerminal);
    }

    while(!leftOverStates.empty()){
        State temp = leftOverStates.back();
        leftOverStates.pop_back();
        computeFirstSet(temp);
    }
}

void displayFirstSets() const {
    for (const auto& stateSet : firstSets) {
        stateSet.showFirstSet();
    }
}

void computeFirstSet(const State& nonTerminal) {
    for (const auto& rule : productionRules) {
        if (rule.getLHS() == nonTerminal) {
            const auto& rhs = rule.getRHS();
            if (!rhs.empty()) {
                const State& firstSymbol = rhs[0];
                if (firstSymbol.getType() == "terminal" || firstSymbol.getType() == "epsilon") {
                    // Add terminal or epsilon directly to FIRST set
                    for (auto& stateSet : firstSets) {
                        if(stateSet.getSet().empty() && stateSet.getSymbol() == nonTerminal){
                            //std::cout << "Adding " << firstSymbol.getSymbol() << " to first set of " << nonTerminal.getSymbol() << std::endl;
                            stateSet.addToFirstSet(State("epsilon", "terminal"));
                            //break;
                        }
                        else if (stateSet.getSymbol() == nonTerminal) {
                            //std::cout << "Adding " << firstSymbol.getSymbol() << " to first set of " << nonTerminal.getSymbol() << std::endl;
                            stateSet.addToFirstSet(firstSymbol);
                            //break;
                        }
                    }
                } else {
                    // Recursively compute FIRST for non-terminals
                    //leftOverStates.push_back(nonTerminal);
                    //leftOverStates.push_back(firstSymbol);
                    computeFirstSet(firstSymbol); //first getting missing
                    for(auto& stateSet : firstSets){
                        if(stateSet.getSymbol() == nonTerminal){
                        
                            for(auto& stateSet2 : firstSets){
                                if(stateSet2.getSymbol() == firstSymbol){
                                    for(auto& s : stateSet2.getSet()){
                                        stateSet.copySet(stateSet2.getSet());
                                        std::cout << "Adding " << firstSymbol.getSymbol() << "'s set to to first set of " << nonTerminal.getSymbol() << std::endl;
                                    }
                                }
                            }
                        }
                    }
                    //computeFirstSet(nonTerminal); //then recomputing the one that had the missing
                    //std::cout << "Adding FIRST(" << firstSymbol.getSymbol() << ") to first set of " << nonTerminal.getSymbol() << std::endl;
                }
            }
        }
    }
    return;
}

// Step # 4
// FUNCTION to compute the FOLLOW set for a given symbol
void computeAllFollowSets() {
    
    for (const auto& nonTerminal : nonTerminals) {
        //std::cout << "Computing follow set for: " << nonTerminal.getSymbol() << std::endl;
        followSets.push_back(StateSet(nonTerminal));
    }

    for (const auto& nonTerminal : nonTerminals) {
        //std::cout << "Computing follow set for: " << nonTerminal.getSymbol() << std::endl;
        computeFollowSet(nonTerminal);
    }

    
    for ( auto& startSet : followSets) {
        if(startSet.getSymbol() == startSymbol){
            for (auto& stateSet : followSets) {
                if (stateSet.getSet().empty()) {
                    startSet.copySet(stateSet.getSet());
                    //std::cout << "Adding $ to follow set of " << startSymbol.getSymbol() << std::endl;
                }
            }
        }
    }
    
}

void displayFollowSets() const {
    for (const auto& stateSet : followSets) {
        stateSet.showFollowSet();
    }
}

void computeFollowSet(const State& nonTerminal) {
        //std::cout << ">>>>Computing follow set for: " << nonTerminal.getSymbol() << std::endl;
        if (nonTerminal == startSymbol) {
            //std::cout << "Adding $ to follow set of " << nonTerminal.getSymbol() << std::endl;
                //followSets.push_back( StateSet(nonTerminal));
                for(auto& stateSet : followSets){
                    if(stateSet.getSymbol() == startSymbol){
                        std::cout << "Adding $ to follow set of " << nonTerminal.getSymbol() << std::endl;
                        stateSet.addToFollowSet(State("$", "terminal"));
                    }
                }
        }

        for (const auto& rule : productionRules) {
            const auto& rhs = rule.getRHS();
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (rhs[i] == nonTerminal && i + 1 < rhs.size()) {
                    const State& nextSymbol = rhs[i + 1];
                    if (nextSymbol.getType() == "terminal") {
                        for (auto& stateSet : followSets) {
                            if (stateSet.getSet().front() == nonTerminal.getSymbol()) {
                                std::cout << "Adding " << nextSymbol.getSymbol() << " to follow set of " << nonTerminal.getSymbol() << std::endl;
                                stateSet.addToFollowSet(nextSymbol);
                            }
                        }
                    } else {
                        computeFirstSet(nextSymbol);
                        for(const auto& stateSet : firstSets){
                            if(stateSet.getSymbol() == nextSymbol){
                                for(auto& follow_stateSet : followSets){
                                    if(follow_stateSet.getSymbol() == nonTerminal){
                                        std::vector<std::string> temp = stateSet.getSet();
                                        std::vector<std::string> temp_new ;
                                        for(const auto& s : temp){
                                                temp_new.push_back(s);
                                        }
                                        for(const auto& s : temp_new){
                                            //if(s != "epsilon"){
                                                follow_stateSet.addToFollowSet(State(s, "terminal"));
                                            //}
                                        
                                    }
                                }
                            }
                        }
                        //computeFollowSet(nonTerminal);
                        //std::cout << "Adding FIRST(" << nextSymbol.getSymbol() << ") to follow set of " << nonTerminal.getSymbol() << std::endl;
                    }
                }
            }
        }
    }
}

// FUNCTION to compute the LL(1) Parsing Table
void computeLL1ParsingTable() const {
    // Create a 2D table where rows are Non-Terminals and columns are Terminals
    std::vector<std::vector<std::string>> parsingTable(nonTerminals.size(), std::vector<std::string>(terminals.size() + 1, " "));

    // Loop through all production rules
    for (const auto& rule : productionRules) {
        const State& LHS = rule.getLHS();
        const std::vector<State>& RHS = rule.getRHS();
        
        // Get the FIRST set of the RHS of the production
        std::vector<State> firstSetRHS = computeFirstOfRHS(RHS);
        
        // For each terminal in the FIRST set of RHS
        for (const auto& terminal : firstSetRHS) {
            if (terminal.getSymbol() != "epsilon") {
                int rowIndex = getNonTerminalIndex(LHS);
                int colIndex = getTerminalIndex(terminal);

                if (colIndex != -1) {
                    parsingTable[rowIndex][colIndex] = rule.getProductionAsString();
                }
            }
        }

        // If epsilon is in the FIRST set, add the rule to FOLLOW sets entries
        for(auto& firstSetRHSstates : firstSetRHS){
            if (isEpsilon(firstSetRHSstates)) {
                for (const auto& followTerminal : getFollowSet(LHS)) {
                    int rowIndex = getNonTerminalIndex(LHS);
                    int colIndex = getTerminalIndex(followTerminal);

                    if (colIndex != -1) {
                        parsingTable[rowIndex][colIndex] = rule.getProductionAsString();
                    }
                    
                    // Add end marker '$' for starting symbol
                    if (followTerminal.getSymbol() == "$") {
                        parsingTable[rowIndex][terminals.size()] = rule.getProductionAsString();
                    }
                }
            }
        }
    }

    displayLL1ParsingTable(parsingTable);
}

// Helper function to compute FIRST set of a RHS vector
std::vector<State> computeFirstOfRHS(const std::vector<State>& rhs) const {
    std::vector<State> result;

    if (rhs.empty()) return {State("epsilon", "terminal")};

    for (const auto& symbol : rhs) {
        if (symbol.getType() == "terminal") {
            result.push_back(symbol);
            break;
        } 
        else if (symbol.getType() == "Non-Terminal") {
            for (const auto& stateSet : firstSets) {
                if (stateSet.getSymbol() == symbol) {
                    result.insert(result.end(), stateSet.getSet().begin(), stateSet.getSet().end());

                    bool hasEpsilon = false;
                    for (const auto& state : stateSet.getSet()) {
                        if (state == "epsilon") {
                            hasEpsilon = true;
                            break;
                        }
                    }
                    
                    if (!hasEpsilon) {
                        return result;
                    }
                }
            }
        }
    }

    return result;
}

// Helper function to get index of a Non-Terminal in nonTerminals vector
int getNonTerminalIndex(const State& nonTerminal) const {
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        if (nonTerminals[i] == nonTerminal) return i;
    }
    return -1;
}

// Helper function to get index of a Terminal in terminals vector
int getTerminalIndex(const State& terminal) const {
    for (size_t i = 0; i < terminals.size(); ++i) {
        if (terminals[i] == terminal) return i;
    }
    return -1;
}

// Helper function to get FOLLOW set for a given Non-Terminal
std::vector<State> getFollowSet(const State& nonTerminal) const {
    for (const auto& stateSet : followSets) {
        if (stateSet.getSymbol() == nonTerminal) {
            std::vector<State> result;
            for (const auto& symbol : stateSet.getSet()) {
                result.push_back(State(symbol, "terminal")); // Adjust type as needed
            }
            return result;
        }
    }
    return {};
}

// FUNCTION to display the LL(1) Parsing Table
void displayLL1ParsingTable(const std::vector<std::vector<std::string>>& parsingTable) const {
    std::cout << "\nLL(1) Parsing Table:\n\n";

    // Determine column width for better alignment
    const int columnWidth = 15;

    // Print header row
    std::cout << std::setw(columnWidth) << " ";
    for (const auto& terminal : terminals) {
        std::cout << std::setw(columnWidth) << terminal.getSymbol();
    }
    std::cout << std::setw(columnWidth) << "$" << "\n";

    // Print a separator line
    std::cout << std::string(columnWidth * (terminals.size() + 3), '-') << "\n";

    // Print rows for each non-terminal
    for (size_t i = 0; i < parsingTable.size(); ++i) {
        std::cout << std::setw(columnWidth) << nonTerminals[i].getSymbol() << "|";
        for (const auto& cell : parsingTable[i]) {
            std::cout << std::setw(columnWidth) << cell << "|";
        }
        std::cout << "\n";
    }
}
    // Helper function to check if a State is epsilon
    bool isEpsilon(const State& state) const {
        return state.getSymbol() == "epsilon";
    }

    // Helper function to check if a State is not epsilon
    bool isNotEpsilon(const State& state) const {
        return state.getSymbol() != "epsilon";
    }

};

#endif // CFG_H
