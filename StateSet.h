#ifndef STATESET_H
#define STATESET_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "State.h"

class StateSet {
private:
    State symbol;
    std::vector<std::string> firstSet;  // FIRST set
    std::vector<std::string> followSet; // FOLLOW set

public:
    // Constructors
    StateSet(State& sym) : symbol(sym) {}
    StateSet(const State& sym) : symbol(sym) {}

    // Add to FIRST set
    bool addToFirstSet(const State& state) {
        if (!existsInSet(firstSet, state)) {
            firstSet.push_back(state.getSymbol());
            return true;
        }
        return false;
    }

    // Add to FOLLOW set
    bool addToFollowSet(const State& state) {
        if (!existsInSet(followSet, state)) {
            followSet.push_back(state.getSymbol());
            return true;
        }
        return false;
    }

    // Merge another StateSet's first set into this one
    bool mergeFirstSet(const StateSet& other) {
        bool changed = false;
        for (const auto& s : other.firstSet) {
            if (!existsInSet(firstSet, s)) {
                firstSet.push_back(s);
                changed = true;
            }
        }
        return changed;
    }

    // Merge another StateSet's follow set into this one
    bool mergeFollowSet(const StateSet& other) {
        bool changed = false;
        for (const auto& s : other.followSet) {
            if (!existsInSet(followSet, s)) {
                followSet.push_back(s);
                changed = true;
            }
        }
        return changed;
    }

    // Check if a state exists in a given set
    bool existsInSet(const std::vector<std::string>& set, const State& state) const {
        return std::find(set.begin(), set.end(), state.getSymbol()) != set.end();
    }

    bool existsInSet(const std::vector<std::string>& set, const std::string& state) const {
        return std::find(set.begin(), set.end(), state) != set.end();
    }

    // Check if the first set contains epsilon
    bool hasEpsilon() const {
        return existsInSet(firstSet, "epsilon");
    }

    // Display FIRST set
    void showFirstSet() const {
        std::cout << "FIRST(" << symbol.getSymbol() << ") Set: { ";
        for (const auto& s : firstSet) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }

    // Display FOLLOW set
    void showFollowSet() const {
        std::cout << "FOLLOW(" << symbol.getSymbol() << ") Set: { ";
        for (const auto& s : followSet) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }
    void addToFollowSet(const std::string& s) {
        if (std::find(followSet.begin(), followSet.end(), s) == followSet.end()) {
            followSet.push_back(s);
        }
    }
    // Getters
    State getSymbol() const { return symbol; }
    const std::vector<std::string>& getSet() const { return firstSet; } // For first set
    const std::vector<std::string>& getFollowSet() const { return followSet; }

    // Copy another set into first set
    bool copySet(const std::vector<std::string>& set) {
        bool changed = false;
        for (const auto& s : set) {
            if (!existsInSet(firstSet, s)) {
                firstSet.push_back(s);
                changed = true;
            }
        }
        return changed;
    }
};

#endif // STATESET_H