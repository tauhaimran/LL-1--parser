#include <iostream>
#include <fstream>
#include "CFG.h"
#include "CFGReader.h"

int main() {
    // Redirect std::cout to a file
    std::ofstream outFile("output.txt");
    if (!outFile) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }
    std::streambuf* coutBuf = std::cout.rdbuf(); // Save the original buffer
    std::cout.rdbuf(outFile.rdbuf()); // Redirect std::cout to outFile

    // Define some states
    State S("S", "start-state");
    State A("A", "non-terminal");
    State B("B", "non-terminal");
    State a("a", "terminal");
    State b("b", "terminal");

    // Create production rules
    ProductionRule rule1(S, { A, B });
    ProductionRule rule2(A, { a });
    ProductionRule rule3(B, { b });

    // Create the CFG
    CFG cfg(S);

    // Add terminals, non-terminals, and rules
    cfg.addNonTerminal(A);
    cfg.addNonTerminal(B);
    cfg.addTerminal(a);
    cfg.addTerminal(b);

    cfg.addProductionRule(rule1);
    cfg.addProductionRule(rule2);
    cfg.addProductionRule(rule3);

    // Display the CFG
    //cfg.display();

    std::cout << "---now--reading--file---" << std::endl;

    State startSymbol("S", "non-terminal");
    CFG cfg2(startSymbol);
    

    if (CFGReader::readFromFile("input.txt", cfg2)) {
        std::cout << "CFG successfully loaded!" << std::endl;
        cfg2.display();
        std::cout << "-------------------------" << std::endl;
        
        std::cout << "Step#1 -- left factoring.." << std::endl;
        cfg2.leftFactoring();
        //cfg2.showNonTerminals();
        // cfg2.showTerminals();
        cfg2.display();
        
        std::cout << "Step#2 -- left recursion.." << std::endl;
        cfg2.eliminateLeftRecursion();
        cfg2.display();

        std::cout << "Step#3 -- computing first set.." << std::endl;
        //cfg2.computeFirstSet();
        cfg2.computeALLFirstSets();
        //std::cout << "First sets computed!" << std::endl;
        cfg2.displayFirstSets();
        //std::cout << "First sets displayed!" << std::endl;

        std::cout << "Step#4 -- computing follow set.." << std::endl;
        //cfg2.computeFollowSet();
        cfg2.computeAllFollowSets();
        cfg2.displayFollowSets();


        std::cout << "Step#5 -- LL(1) parsing table.." << std::endl;
        cfg2.computeLL1ParsingTable();

    } else {
        std::cerr << "Failed to load CFG from file." << std::endl;
    }

    // Restore the original buffer
    std::cout.rdbuf(coutBuf);
    outFile.close();

    return 0;
}