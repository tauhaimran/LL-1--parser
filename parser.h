#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include "ParsingTable.h"
#include "CFG.h"

class Parser {
private:
    ParsingTable& parsingTable;  // Reference to the parsing table
    std::vector<std::string> inputTokens;  // List of input tokens to be parsed
    std::stack<std::string> parseStack;  // Stack to hold the parse process
public:
    // Helper function to read and tokenize input from a file
    void tokenizeInput(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            while (iss >> token) {
                inputTokens.push_back(token);  // Tokenize and store
            }
        }
    }

    // Helper function to get the next token from the input
    std::string getNextToken() {
        if (!inputTokens.empty()) {
            return inputTokens.front();  // Return the first token
        }
        return "$";  // End marker if no tokens are left
    }

    // Helper function to display the current state of the stack
    void displayStack() const {
        std::stack<std::string> tempStack = parseStack;
        std::cout << "Stack: ";
        while (!tempStack.empty()) {
            std::cout << tempStack.top() << " ";
            tempStack.pop();
        }
        std::cout << std::endl;
    }

    // Helper function to display the current state of the input
    void displayInput() const {
        std::cout << "Input Tokens: ";
        for (const auto& token : inputTokens) {
            std::cout << token << " , ";
        }
        std::cout << "$" << std::endl;
    }


    // Constructor that initializes the parser with the parsing table and input file
    Parser(ParsingTable& table, const std::string& inputFile) : parsingTable(table) {
        tokenizeInput(inputFile);  // Tokenize the input file
        parseStack.push("$");  // End marker
        parseStack.push("S");  // Start symbol (assumed)
    }

    // Function to perform LL(1) parsing
    // Function to perform LL(1) parsing
void parse() {
    std::string currentToken = getNextToken();

    while (!parseStack.empty()) {
        std::string topOfStack = parseStack.top();  // Peek (don't pop yet)

        // Show the current state
        displayStack();
        displayInput();
        std::cout << "--------------------------" << std::endl;
        std::cout << "Top of stack: " << topOfStack << std::endl;
        std::cout << "Current token: " << currentToken << std::endl;

        // Case 1: If both are $, successful parse
        if (topOfStack == "$" && currentToken == "$") {
            std::cout << "Parsing successful! Reached end of input." << std::endl;
            return;
        }

        // Case 2: Top of stack is a terminal
        if (isTerminal(topOfStack)) {
            if (topOfStack == currentToken) {
                std::cout << "Matched terminal: " << currentToken << std::endl;
                parseStack.pop();  // Now pop
                inputTokens.erase(inputTokens.begin());  // Move to next input token
                currentToken = getNextToken();  // Fetch next token
            } else {
                std::cout << "Error: Unexpected token '" << currentToken << "' when expecting '" << topOfStack << "'." << std::endl;
                return;  // Parsing failed
            }
        }
        // Case 3: Top of stack is a non-terminal
        else {
            std::string rule = parsingTable.getEntry(topOfStack, currentToken);
            if (!rule.empty()) {
                std::cout << "Expanding non-terminal '" << topOfStack << "' with rule: " << rule << std::endl;
                parseStack.pop();  // Pop AFTER getting the rule

                // Push the rule's RHS onto the stack in reverse
                std::vector<std::string> symbols;
                std::istringstream ruleStream(rule);
                std::string symbol;
                while (ruleStream >> symbol) {
                    if (symbol != "epsilon") {
                        symbols.push_back(symbol);
                    }
                }
                // Push in reverse order
                for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
                    parseStack.push(*it);
                }
            } else {
                if(currentToken == "$") {
                    std::cout << "Error: Unexpected end of input when expecting non-terminal '" << topOfStack << "'." << std::endl;
                    std::cout << "Parsing Kinda Passed: Stack is empty but input is not completely parsed." << std::endl;
                    return;  // Parsing sorta passed but not really
                }
                std::cout << "Error: No matching rule for non-terminal '" << topOfStack << "' with input '" << currentToken << "'." << std::endl;
                return;  // Parsing failed
            }
        }
    }

    // If stack is empty but input isn't consumed fully
    if (currentToken != "$") {
        std::cout << "Error: Input not fully consumed. Remaining token: " << currentToken << std::endl;
    }


    // If we've exhausted the stack without fully parsing the input
    std::cout << "Parsing failed: Stack is empty but input is not completely parsed." << std::endl;
}


    // Helper function to check if a symbol is a terminal
    bool isTerminal(const std::string& symbol) {
        bool isTerminal = false;
        std::vector<std::string> terminals = parsingTable.getTerminals();  // Get the list of terminals
        for (const auto& terminal : terminals) {
            if (symbol == terminal) {
                isTerminal = true;  // Found a match in the list of terminals
                break;
            }
        }
        return isTerminal;
    }
};

#endif
// PARSER_H