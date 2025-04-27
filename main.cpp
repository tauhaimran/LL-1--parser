#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CFGManual.h"

int main() {

    // Initialize CFG object
    CFG cfg;

    // Create the manual CFG using the function from CFGManual.h
    createManualCFG(cfg);

    // Testing CFG
    cfg.leftFactoring();
    cfg.eliminateLeftRecursion();
    cfg.computeALLFirstSets();
    //cfg.computeAllFollowSets();
    //cfg.displayFirstSets();
    //cfg.displayFollowSets();
    //cfg.computeLL1ParsingTable();
    //cfg.display();
    //cfg.showNonTerminals();
    //cfg.showTerminals();
    //cfg.displayFirstSets();

    //std::vector<std::string> nonTerminals = {"S", "StmtList", "Stmt", "Expr", "ExprPrime", "Term", "Cond", "RelOp"};
    //std::vector<std::string> terminals = {"id", "number", "=", "+", "-", ";", "if", "(", ")", "{", "}", ">", "<", "==", "!="};

    // Create the parsing table
    //ParsingTable table(nonTerminals, terminals);

    // Initialize the parsing table with grammar rules
    //initializeParsingTable(table);

    // Display the parsing table
    //table.display();

    // Define the non-terminals and terminals
    std::vector<std::string> nonTerminals = {"S", "StmtList", "Stmt", "Expr", "ExprPrime", "Term", "Cond", "RelOp"};
    std::vector<std::string> terminals = {"id", "number", "=", "+", "-", ";", "if", "(", ")", "{", "}", ">", "<", "==", "!="};

    // Create the parsing table
    ParsingTable table(nonTerminals, terminals);

    // Initialize the parsing table with grammar rules
    initializeParsingTable(table);

    // Specify the input file to parse (e.g., "input.txt")
    std::string inputFile = "input.txt";

    // Create the parser
    Parser parser(table, inputFile);

    // Parse the input file
    table.display(); // Display the parsing table
    std::cout << "Parsing input file: " << inputFile << std::endl;
    std::cout << "Parsing content: " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    parser.displayInput();
    std::cout << "----------------------------------------" << std::endl;
    parser.displayStack();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Parsing result: " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    parser.parse();

    return 0;
}
