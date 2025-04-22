#ifndef STATESET_H
#define STATESET_H

#include <iostream>
#include <string>
#include <vector>
#include "State.h"

class StateSet {
private:
    State symbol;
    std::vector<std::string> Set;
    std::vector<State> followSet;

public:
    // Constructor
    StateSet(State& sym) : symbol(sym) {}
    StateSet(const State& sym) : symbol(sym) {}

    // Adds a State to the FIRST set if it doesn't already exist
    void addToFirstSet(const State& state) {
        //std::cout << "Adding to first set: " << state.getSymbol() << std::endl;
        if (!existsInSet(Set, state)) {
            Set.push_back(state.getSymbol());
        }
    }

    // Adds a State to the FOLLOW set if it doesn't already exist
    void addToFollowSet(const State& state) {
        //std::cout << "Adding to follow set: " << state.getSymbol() << std::endl;
        if (!existsInSet(Set, state)) {
            //std::cout << "hereee";
            Set.push_back(state.getSymbol());
            //std::cout << "hereee";
        }
    }

    // Checks if a State already exists in a given set
    bool existsInSet(const std::vector<std::string>& stateSet, const State& state) const {
        for (const auto& s : stateSet) {
            if (s == state.getSymbol()) {

                return true;
            }
        }
        return false;
    }

    bool existsInSet(const std::vector<std::string>& stateSet, const std::string& state) const {
        for (const auto& s : stateSet) {
            if (s == state) {

                return true;
            }
        }
        return false;
    }

    // Displays the FIRST set
    void showFirstSet() const {
        std::cout << "FIRST(" << symbol.getSymbol() <<  ") Set: { ";
        for (const auto& s : Set) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }

    // Displays the FOLLOW set
    void showFollowSet() const {
        std::cout << "FOLLOW(" << symbol.getSymbol()<<") Set: { ";
        for (const auto& s : Set) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }

    State getSymbol() const {
        return symbol;
    }
    std::vector<std::string> getSet() const {
        return Set;
    }

    void copySet(const std::vector<std::string>& set) {
        for(const auto& s : set){
            if(!existsInSet(Set, s)){
                Set.push_back(s);
            }
        }
    }


};

#endif // STATESET_H
