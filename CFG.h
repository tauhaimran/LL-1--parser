#ifndef CFG_H
#define CFG_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "State.h"
#include "ProductionRule.h"
#include "StateSet.h"
#include "ParsingTable.h"

class CFG {
private:
    std::vector<State> nonTerminals;
    std::vector<State> terminals;
    std::vector<ProductionRule> productionRules;
    std::vector<StateSet> firstSets;
    std::vector<StateSet> followSets;
    std::vector<State> leftOverStates;
    State startSymbol;

public:
    CFG(const State& startSymbol = State("S", "start-state")) : startSymbol(startSymbol) {}

    std::vector<State> getNonTerminals() const { return nonTerminals; }
    std::vector<State> getTerminals() const { return terminals; }

    void addNonTerminal(const State& nonTerminal) {
        if (!isInNonTerminals(nonTerminal)) {
            nonTerminals.push_back(nonTerminal);
        }
    }

    void addTerminal(const State& terminal) {
        if (!isInTerminals(terminal)) {
            terminals.push_back(terminal);
        }
    }

    void addProductionRule(const ProductionRule& rule) {
        productionRules.push_back(rule);
    }

    void display() const {
        std::cout << "Start Symbol: ";
        startSymbol.showSymbol();
        std::cout << std::endl << "Production Rules:" << std::endl;
        for (const auto& rule : productionRules) {
            rule.display();
        }
    }

    void showNonTerminals() const {
        std::cout << "Non-terminals: \n";
        for (const auto& nonTerminal : nonTerminals) {
            nonTerminal.showSymbol();
            std::cout << "\t(";
            nonTerminal.showType();
            std::cout << ") , \n";
        }
        std::cout << std::endl;
    }

    void showTerminals() const {
        std::cout << "Terminals: \n";
        for (const auto& terminal : terminals) {
            terminal.showSymbol();
            std::cout << "\t(";
            terminal.showType();
            std::cout << ") , \n";
        }
        std::cout << std::endl;
    }

    bool isInTerminals(const State& state) const {
        for (const auto& terminal : terminals) {
            if (terminal.getSymbol() == state.getSymbol()) return true;
        }
        return false;
    }

    bool isInNonTerminals(const State& state) const {
        for (const auto& nonTerminal : nonTerminals) {
            if (nonTerminal.getSymbol() == state.getSymbol()) return true;
        }
        return false;
    }

    void removeRule(const ProductionRule& targetRule) {
        for (size_t i = 0; i < productionRules.size(); ++i) {
            if (productionRules[i].getLHS() == targetRule.getLHS() &&
                productionRules[i].getRHS() == targetRule.getRHS()) {
                productionRules.erase(productionRules.begin() + i);
                return;
            }
        }
    }

    void leftFactoring() {
        for (size_t x = 0; x < nonTerminals.size(); ++x) {
            const auto& nonTerminal = nonTerminals[x];
            std::vector<ProductionRule> RULES_FOR_NONTERMINAL;
            for (const auto& rule : productionRules) {
                if (rule.getLHS() == nonTerminal) {
                    RULES_FOR_NONTERMINAL.push_back(rule);
                }
            }

            for (size_t j = 0; j < RULES_FOR_NONTERMINAL.size(); ++j) {
                for (size_t k = j + 1; k < RULES_FOR_NONTERMINAL.size(); ++k) {
                    ProductionRule rule_j = RULES_FOR_NONTERMINAL[j];
                    ProductionRule rule_k = RULES_FOR_NONTERMINAL[k];
                    std::vector<State> prefix, remainingJ, remainingK;
                    size_t INDEX = 0;

                    while (INDEX < std::min(rule_j.getRHS().size(), rule_k.getRHS().size()) &&
                           rule_j.getRHS()[INDEX] == rule_k.getRHS()[INDEX]) {
                        prefix.push_back(rule_j.getRHS()[INDEX]);
                        INDEX++;
                    }

                    if (!prefix.empty()) {
                        State newNonTerminal(nonTerminal.getSymbol() + "'", "non-terminal");
                        addNonTerminal(newNonTerminal);
                        prefix.push_back(newNonTerminal);
                        ProductionRule newProductionRule(nonTerminal, prefix);
                        productionRules.push_back(newProductionRule);

                        remainingJ.assign(rule_j.getRHS().begin() + INDEX, rule_j.getRHS().end());
                        remainingK.assign(rule_k.getRHS().begin() + INDEX, rule_k.getRHS().end());

                        if (remainingJ.empty()) {
                            remainingJ.push_back(State("epsilon", "terminal"));
                        }
                        if (remainingK.empty()) {
                            remainingK.push_back(State("epsilon", "terminal"));
                        }

                        productionRules.push_back(ProductionRule(newNonTerminal, remainingJ));
                        productionRules.push_back(ProductionRule(newNonTerminal, remainingK));

                        removeRule(rule_j);
                        removeRule(rule_k);

                        leftFactoring();
                        return;
                    }
                }
            }
        }
    }

    void eliminateLeftRecursion() {
        for (size_t i = 0; i < nonTerminals.size(); ++i) {
            State Ai = nonTerminals[i];

            // Step 1: Eliminate Indirect Left Recursion
            for (size_t j = 0; j < i; ++j) {
                State Aj = nonTerminals[j];
                std::vector<ProductionRule> newRules;
                for (const auto& rule : productionRules) {
                    if (rule.getLHS() == Ai && !rule.getRHS().empty() && rule.getRHS()[0] == Aj) {
                        for (const auto& AjRule : productionRules) {
                            if (AjRule.getLHS() == Aj) {
                                std::vector<State> newRHS = AjRule.getRHS();
                                newRHS.insert(newRHS.end(), rule.getRHS().begin() + 1, rule.getRHS().end());
                                newRules.push_back(ProductionRule(Ai, newRHS));
                            }
                        }
                    } else {
                        newRules.push_back(rule);
                    }
                }
                productionRules = newRules;
            }

            // Step 2: Eliminate Direct Left Recursion
            std::vector<ProductionRule> recursiveRules, nonRecursiveRules;
            for (const auto& rule : productionRules) {
                if (rule.getLHS() == Ai) {
                    if (!rule.getRHS().empty() && rule.getRHS()[0] == Ai) {
                        recursiveRules.push_back(rule);
                    } else {
                        nonRecursiveRules.push_back(rule);
                    }
                }
            }

            // Preserve rules for other non-terminals
            std::vector<ProductionRule> newRules;
            for (const auto& rule : productionRules) {
                if (rule.getLHS() != Ai) {
                    newRules.push_back(rule);
                }
            }

            if (!recursiveRules.empty()) {
                State AiDash(Ai.getSymbol() + "'", "non-terminal");
                addNonTerminal(AiDash);

                // Transform non-recursive rules: Ai -> β | β Ai'
                for (const auto& rule : nonRecursiveRules) {
                    std::vector<State> newRHS = rule.getRHS();
                    if (newRHS.empty() || (newRHS.size() == 1 && newRHS[0].getSymbol() == "epsilon")) {
                        newRules.push_back(ProductionRule(Ai, rule.getRHS()));
                        newRules.push_back(ProductionRule(Ai, {AiDash}));
                    } else {
                        newRules.push_back(ProductionRule(Ai, rule.getRHS()));
                        newRHS.push_back(AiDash);
                        newRules.push_back(ProductionRule(Ai, newRHS));
                    }
                }

                // Transform recursive rules: Ai' -> α Ai' | epsilon
                for (const auto& rule : recursiveRules) {
                    std::vector<State> newRHS(rule.getRHS().begin() + 1, rule.getRHS().end());
                    if (!newRHS.empty()) {
                        newRHS.push_back(AiDash);
                        newRules.push_back(ProductionRule(AiDash, newRHS));
                    }
                }
                newRules.push_back(ProductionRule(AiDash, {State("epsilon", "terminal")}));
            } else {
                newRules.insert(newRules.end(), nonRecursiveRules.begin(), nonRecursiveRules.end());
            }

            productionRules = newRules;
        }
    }

    void computeALLFirstSets() {
        firstSets.clear();
        for (const auto& nonTerminal : nonTerminals) {
            firstSets.emplace_back(nonTerminal);
        }

        bool changed;
        do {
            changed = false;
            for (const auto& rule : productionRules) {
                State A = rule.getLHS();
                auto& firstA = getFirstSet(A);
                const auto& rhs = rule.getRHS();
                if (rhs.empty() || (rhs.size() == 1 && rhs[0].getSymbol() == "epsilon")) {
                    if (firstA.addToFirstSet(State("epsilon", "terminal"))) {
                        changed = true;
                    }
                } else {
                    bool allHaveEpsilon = true;
                    for (size_t i = 0; i < rhs.size(); ++i) {
                        const auto& symbol = rhs[i];
                        if (isInTerminals(symbol)) {
                            if (firstA.addToFirstSet(symbol)) {
                                changed = true;
                            }
                            allHaveEpsilon = false;
                            break;
                        } else if (isInNonTerminals(symbol)) {
                            auto& firstB = getFirstSet(symbol);
                            for (const auto& s : firstB.getSet()) {
                                if (s != "epsilon") {
                                    if (firstA.addToFirstSet(State(s, "terminal"))) {
                                        changed = true;
                                        std::cout << "Adding " << symbol.getSymbol() << "'s set to first set of " << A.getSymbol() << std::endl;
                                    }
                                }
                            }
                            if (!firstB.hasEpsilon()) {
                                allHaveEpsilon = false;
                                break;
                            }
                        }
                    }
                    if (allHaveEpsilon && firstA.addToFirstSet(State("epsilon", "terminal"))) {
                        changed = true;
                    }
                }
            }
        } while (changed);
    }

    void computeAllFollowSets() {
        followSets.clear();
        for (const auto& nonTerminal : nonTerminals) {
            followSets.emplace_back(nonTerminal);
        }

        getFollowSet(startSymbol).addToFollowSet(State("$", "terminal"));
        bool changed;
        do {
            changed = false;
            for (const auto& rule : productionRules) {
                State A = rule.getLHS();
                const auto& rhs = rule.getRHS();
                for (size_t i = 0; i < rhs.size(); ++i) {
                    if (isInNonTerminals(rhs[i])) {
                        State B = rhs[i];
                        auto& followB = getFollowSet(B);
                        if (i + 1 < rhs.size()) {
                            const auto& next = rhs[i + 1];
                            if (isInTerminals(next)) {
                                if (followB.addToFollowSet(next)) {
                                    changed = true;
                                    std::cout << "Adding " << next.getSymbol() << " to follow set of " << B.getSymbol() << std::endl;
                                }
                            } else if (isInNonTerminals(next)) {
                                auto& firstNext = getFirstSet(next);
                                for (const auto& s : firstNext.getSet()) {
                                    if (s != "epsilon" && followB.addToFollowSet(State(s, "terminal"))) {
                                        changed = true;
                                        std::cout << "Adding " << s << " to follow set of " << B.getSymbol() << std::endl;
                                    }
                                }
                            }
                        }
                        bool allSubsequentHaveEpsilon = true;
                        for (size_t j = i + 1; j < rhs.size(); ++j) {
                            if (isInTerminals(rhs[j])) {
                                allSubsequentHaveEpsilon = false;
                                break;
                            }
                            if (!getFirstSet(rhs[j]).hasEpsilon()) {
                                allSubsequentHaveEpsilon = false;
                                break;
                            }
                        }
                        if (i + 1 == rhs.size() || allSubsequentHaveEpsilon) {
                            auto& followA = getFollowSet(A);
                            for (const auto& s : followA.getSet()) {
                                if (followB.addToFollowSet(State(s, "terminal"))) {
                                    changed = true;
                                    std::cout << "Adding " << s << " to follow set of " << B.getSymbol() << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        } while (changed);
    }

    void displayFirstSets() const {
        for (const auto& stateSet : firstSets) {
            stateSet.showFirstSet();
        }
    }

    void displayFollowSets() const {
        for (const auto& stateSet : followSets) {
            stateSet.showFollowSet();
        }
    }

    std::vector<std::vector<std::string>> computeLL1ParsingTable() const {
        // Get terminal symbols excluding epsilon
        std::vector<State> tSymbols;
        for (const auto& t : terminals) {
            if (t.getSymbol() != "epsilon") {
                tSymbols.push_back(t);
            }
        }
        // Add end marker as a special terminal
        tSymbols.push_back(State("$", "terminal"));
    
        // Initialize parsing table with empty cells
        std::vector<std::vector<std::string>> parsingTable(
            nonTerminals.size(),
            std::vector<std::string>(tSymbols.size(), "")
        );
    
        for (const auto& rule : productionRules) {
            const State& LHS = rule.getLHS();
            const std::vector<State>& RHS = rule.getRHS();
            std::vector<State> firstSetRHS = computeFirstOfRHS(RHS);
    
            for (const auto& terminal : firstSetRHS) {
                if (terminal.getSymbol() != "epsilon") {
                    int rowIndex = getNonTerminalIndex(LHS);
                    int colIndex = getTerminalIndexForTable(terminal, tSymbols);
                    
                    if (rowIndex >= 0 && colIndex >= 0) {
                        parsingTable[rowIndex][colIndex] = rule.getProductionAsString();
                    }
                }
            }
    
            if (isEpsilon(firstSetRHS)) {
                const auto& followSet = getFollowSet(LHS);
                for (const auto& symbol : followSet.getSet()) {
                    State followTerminal(symbol, "terminal");
                    int rowIndex = getNonTerminalIndex(LHS);
                    int colIndex = getTerminalIndexForTable(followTerminal, tSymbols);
                    
                    if (rowIndex >= 0 && colIndex >= 0) {
                        parsingTable[rowIndex][colIndex] = rule.getProductionAsString();
                    }
                }
            }
        }
    
        return parsingTable;
    }
    

private:
     // Update getTerminalIndexForTable to better handle the terminal search
     int getTerminalIndexForTable(const State& terminal, const std::vector<State>& tableTerminals) const {
        for (size_t i = 0; i < tableTerminals.size(); ++i) {
            if (tableTerminals[i].getSymbol() == terminal.getSymbol()) {
                return static_cast<int>(i);
            }
        }
        return -1; // Not found
    }
    StateSet& getFirstSet(const State& nt) {
        for (auto& fs : firstSets) {
            if (fs.getSymbol() == nt) return fs;
        }
        firstSets.emplace_back(nt);
        return firstSets.back();
    }

    const StateSet& getFirstSet(const State& nt) const {
        for (const auto& fs : firstSets) {
            if (fs.getSymbol() == nt) return fs;
        }
        static StateSet empty(State("", "non-terminal"));
        return empty;
    }

    StateSet& getFollowSet(const State& nt) {
        for (auto& fs : followSets) {
            if (fs.getSymbol() == nt) return fs;
        }
        followSets.emplace_back(nt);
        return followSets.back();
    }

    const StateSet& getFollowSet(const State& nt) const {
        for (const auto& fs : followSets) {
            if (fs.getSymbol() == nt) return fs;
        }
        static StateSet empty(State("", "non-terminal"));
        return empty;
    }

    std::vector<State> computeFirstOfRHS(const std::vector<State>& rhs) const {
        std::vector<State> result;
        if (rhs.empty() || (rhs.size() == 1 && rhs[0].getSymbol() == "epsilon")) {
            result.emplace_back("epsilon", "terminal");
            return result;
        }

        bool allHaveEpsilon = true;
        for (size_t i = 0; i < rhs.size(); ++i) {
            const auto& symbol = rhs[i];
            if (isInTerminals(symbol)) {
                result.push_back(symbol);
                allHaveEpsilon = false;
                break;
            } else if (isInNonTerminals(symbol)) {
                auto& firstSym = getFirstSet(symbol);
                for (const auto& s : firstSym.getSet()) {
                    if (s != "epsilon") {
                        result.emplace_back(s, "terminal");
                    }
                }
                if (!firstSym.hasEpsilon()) {
                    allHaveEpsilon = false;
                    break;
                }
            }
        }
        if (allHaveEpsilon) {
            result.emplace_back("epsilon", "terminal");
        }
        return result;
    }

    int getNonTerminalIndex(const State& nonTerminal) const {
        for (size_t i = 0; i < nonTerminals.size(); ++i) {
            if (nonTerminals[i].getSymbol() == nonTerminal.getSymbol()) return i;
        }
        return -1;
    }

    int getTerminalIndex(const State& terminal) const {
        for (size_t i = 0; i < terminals.size(); ++i) {
            if (terminals[i].getSymbol() == terminal.getSymbol()) return i;
        }
        return -1;
    }

    void displayLL1ParsingTable(const std::vector<std::vector<std::string>>& parsingTable) const {
        std::vector<State> tSymbols;
        for (const auto& t : terminals) {
            if (t.getSymbol() != "epsilon") {
                tSymbols.push_back(t);
            }
        }
        tSymbols.push_back(State("$", "terminal"));

        std::cout << "\nLL(1) Parsing Table:\n\n";
        const int columnWidth = 15;
        std::cout << std::setw(columnWidth) << " ";
        for (const auto& terminal : tSymbols) {
            std::cout << std::setw(columnWidth) << terminal.getSymbol();
        }
        std::cout << "\n";
        std::cout << std::string(columnWidth * (tSymbols.size() + 1), '-') << "\n";
        for (size_t i = 0; i < parsingTable.size(); ++i) {
            std::cout << std::setw(columnWidth) << nonTerminals[i].getSymbol() << "|";
            for (size_t j = 0; j < parsingTable[i].size(); ++j) {
                std::cout << std::setw(columnWidth) << parsingTable[i][j] << "|";
            }
            std::cout << "\n";
        }
    }

    bool isEpsilon(const std::vector<State>& states) const {
        for (const auto& state : states) {
            if (state.getSymbol() == "epsilon") return true;
        }
        return false;
    }

    bool isEpsilon(const State& state) const {
        return state.getSymbol() == "epsilon";
    }

    bool isNotEpsilon(const State& state) const {
        return state.getSymbol() != "epsilon";
    }
};

#endif // CFG_H