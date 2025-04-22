// ProductionRule.h
#ifndef PRODUCTION_RULE_H
#define PRODUCTION_RULE_H

#include <vector>
#include <string>
#include <iostream>
#include "State.h"

class ProductionRule {
private:
    State lhs; // Left-hand side (LHS): A single State
    std::vector<State> rhs; // Right-hand side (RHS): A list of States

public:
    // Constructor
    ProductionRule(const State& lhsState, const std::vector<State>& rhsStates)
        : lhs(lhsState), rhs(rhsStates) {}

    // Get LHS
    const State& getLHS() const { return lhs; }

    // Get RHS
    const std::vector<State>& getRHS() const { return rhs; }

    // Display the Production Rule
    void display() const {
        lhs.showSymbol();
        std::cout << " -> ";
        for (const auto& state : rhs) {
            state.showSymbol();
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    // Define operator== for ProductionRule
    bool operator==(const ProductionRule& other) const {
        return this->getLHS() == other.getLHS() && this->getRHS() == other.getRHS();
    }

     // Method to return the production rule as a string
     std::string getProductionAsString() const {
        std::string result = getLHS().getSymbol() + " -> ";
        for (const auto& state : getRHS()) {
            result += state.getSymbol() + " ";
        }
        return result;
    }
    
};

#endif // PRODUCTION_RULE_H
