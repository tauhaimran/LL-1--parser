#include <iostream>
#include <fstream>
#include "CFG.h"
#include "CFGReader.h"
#include "ParsingTable.h"
#include "parser.h"

int main() {
    CFG cfg;
    if (!CFGReader::readFromFile("cfg.txt", cfg)) {
        std::cerr << "Failed to read CFG from file" << std::endl;
        return 1;
    }

    cfg.eliminateLeftRecursion();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Grammar / CFG: " << std::endl;
    cfg.display();
    cfg.showNonTerminals();
    cfg.showTerminals();
    std::cout << "----------------------------------------" << std::endl;

    cfg.computeALLFirstSets();
    cfg.computeAllFollowSets();

    std::cout << "FIRST Sets:\n";
    cfg.displayFirstSets();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "FOLLOW Sets:\n";
    cfg.displayFollowSets();
    std::cout << "----------------------------------------" << std::endl;

    std::vector<std::vector<std::string>> tableData = cfg.computeLL1ParsingTable();

    ParsingTable table(tableData, cfg.getNonTerminals(), cfg.getTerminals());
    table.display();

    Parser parser(table, "input.txt", cfg);
    parser.parse();

    return 0;
}