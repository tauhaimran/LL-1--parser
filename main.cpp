#include <iostream>
#include <fstream>
#include "parser.h"
#include "CFGManual.h"

int main() {
    // Initialize CFG object
    CFG cfg;
    createManualCFG(cfg); // Populates cfg with grammar
    cfg.leftFactoring();
    cfg.eliminateLeftRecursion();
    cfg.computeALLFirstSets();
    // cfg.computeAllFollowSets(); // Uncomment if needed

    // Define non-terminals and terminals (include $)
    std::vector<std::string> nonTerminals = {"S", "StmtList", "Stmt", "Expr", "ExprPrime", "Term", "Cond", "RelOp"};
    std::vector<std::string> terminals = {"id", "number", "=", "+", "-", ";", "if", "(", ")", "{", "}", ">", "<", "==", "!=", "$"};

    // Create and initialize the parsing table
    ParsingTable table(nonTerminals, terminals);
    initializeParsingTable(table);

    // Display CFG and parsing table
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Grammar / CFG: " << std::endl;
    cfg.display();
    cfg.showNonTerminals();
    cfg.showTerminals();
    std::cout << "----------------------------------------" << std::endl;
    table.display();
    std::cout << "Parsing input file: input.txt" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Create and run parser
    Parser parser(table, "input.txt", cfg);
    parser.parse();

    return 0;
}