#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include <iomanip>
#include "ParsingTable.h"
#include "CFG.h"
#include <algorithm>

class Parser {
private:
    ParsingTable& parsingTable;  // Reference to the parsing table
    CFG& cfg;                    // Reference to CFG for terminal/non-terminal checks
    std::string inputFile;       // Input file name

    // Helper function to parse production string (RHS only)
    std::vector<std::string> parseProduction(const std::string& production) {
        std::vector<std::string> rhs;
        if (production.empty()) return rhs;

        std::stringstream ss(production);
        std::string token;
        while (ss >> token) {
            if (token == "epsilon") {
                return rhs; // Epsilon production means empty RHS
            }
            rhs.push_back(token);
        }
        return rhs;
    }

    // Helper function to print a parsing step in tabular format
    void printStep(const std::stack<std::string>& stack, const std::vector<std::string>& input, 
                   size_t input_idx, const std::string& action) {
        // Convert stack to string
        std::stack<std::string> temp_stack = stack;
        std::vector<std::string> stack_content;
        while (!temp_stack.empty()) {
            stack_content.push_back(temp_stack.top());
            temp_stack.pop();
        }
        std::reverse(stack_content.begin(), stack_content.end());
        std::string stack_str = "";
        for (const auto& s : stack_content) stack_str += s + " ";

        // Convert remaining input to string
        std::string input_str = "";
        for (size_t i = input_idx; i < input.size(); ++i) input_str += input[i] + " ";
        if (input_str.empty()) input_str = "$";

        // Print formatted step
        std::cout << std::left << std::setw(30) << stack_str 
                  << std::setw(30) << input_str 
                  << std::setw(40) << action << "\n";
    }

    // Debug function to print non-terminals and terminals
    void debugSymbols() {
        std::cout << "Debug: Non-terminals in CFG:\n";
        for (const auto& nt : cfg.getNonTerminals()) {
            std::cout << nt.getSymbol() << " (" << nt.getType() << "), ";
        }
        std::cout << "\nDebug: Terminals in CFG:\n";
        for (const auto& t : cfg.getTerminals()) {
            std::cout << t.getSymbol() << " (" << t.getType() << "), ";
        }
        std::cout << "\n";
    }

    // Parse a single line of tokens
    void parseLine(const std::vector<std::string>& tokens, int line_num) {
        std::stack<std::string> stack;
        stack.push("$"); // End marker
        stack.push("S"); // Start symbol

        std::vector<std::string> input = tokens;
        input.push_back("$"); // Append end marker
        size_t input_idx = 0;

        // Print table header
        std::cout << "\nParsing line " << line_num << ": " << (tokens.empty() ? "(empty)" : "");
        for (const auto& t : tokens) std::cout << t << " ";
        std::cout << "\n";
        debugSymbols(); // Debug non-terminals and terminals
        std::cout << std::left 
                  << std::setw(30) << "Stack" 
                  << std::setw(30) << "Input" 
                  << std::setw(40) << "Action" << "\n";
        std::cout << std::string(100, '-') << "\n";

        bool error = false;
        while (!stack.empty() && !error) {
            std::string top = stack.top();
            std::string current_input = (input_idx < input.size()) ? input[input_idx] : "$";

            std::string action = "";
            if (top == "$" && current_input == "$") {
                action = "Accept";
                printStep(stack, input, input_idx, action);
                stack.pop();
                break;
            } else if (top == "$" || cfg.isInTerminals(State(top, "terminal"))) {
                // Top is a terminal or end marker: match it with input
                if (top == current_input) {
                    action = "Match " + top;
                    printStep(stack, input, input_idx, action);
                    stack.pop();
                    input_idx++;
                } else {
                    action = "Error: Expected " + top + ", found " + current_input;
                    printStep(stack, input, input_idx, action);
                    error = true;
                }
            } else if (cfg.isInNonTerminals(State(top, "non-terminal")) || cfg.isInNonTerminals(State(top, "start-state"))) {
                // Top is a non-terminal or start symbol: expand using parsing table
                std::string production = parsingTable.getEntry(top, current_input);
                if (!production.empty()) {
                    action = "Expand " + top + " -> " + production;
                    printStep(stack, input, input_idx, action);
                    stack.pop();
                    std::vector<std::string> rhs = parseProduction(production);
                    for (auto it = rhs.rbegin(); it != rhs.rend(); ++it) {
                        stack.push(*it);
                    }
                } else {
                    action = "Error: No production for " + top + " on " + current_input;
                    printStep(stack, input, input_idx, action);
                    error = true;
                }
            } else {
                action = "Error: Invalid symbol " + top;
                printStep(stack, input, input_idx, action);
                error = true;
            }
        }

        std::cout << std::string(100, '-') << "\n";
        if (!error && stack.empty()) {
            std::cout << "Parsing successful!\n";
        } else {
            std::cout << "Parsing failed.\n";
        }
    }

public:
    // Constructor
    Parser(ParsingTable& table, const std::string& inputFile, CFG& cfgSet) 
        : parsingTable(table), cfg(cfgSet), inputFile(inputFile) {}

    // Parse the input file
    void parse() {
        std::ifstream file(inputFile);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << inputFile << "'\n";
            return;
        }

        // Concatenate lines to handle multi-line statements
        std::string full_input;
        int line_num = 1;
        std::string line;
        while (std::getline(file, line)) {
            full_input += line + " ";
            line_num++;
        }
        file.close();

        // Tokenize the full input
        std::stringstream ss(full_input);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            std::cout << "\nParsing: (empty)\n";
            std::cout << "Empty input, skipping.\n";
            return;
        }

        // Parse the concatenated tokens as a single input
        std::cout << "\nParsing concatenated input: ";
        for (const auto& t : tokens) std::cout << t << " ";
        std::cout << "\n";
        parseLine(tokens, 1);
    }
};

#endif // PARSER_H