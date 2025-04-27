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

    // Helper function to read and tokenize input from a file
    // Tokenizing with handling of operators and delimiters
void tokenizeInput(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char c;
        std::string token;
        while (iss.get(c)) {
            if (std::isspace(c)) {
                if (!token.empty()) {
                    inputTokens.push_back(token);
                    token.clear();
                }
            } else if (std::ispunct(c)) { // Treat punctuation as separate tokens
                if (!token.empty()) {
                    inputTokens.push_back(token);
                    token.clear();
                }
                inputTokens.push_back(std::string(1, c)); // Push single character
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            inputTokens.push_back(token); // Add any remaining token
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

public:
    // Constructor that initializes the parser with the parsing table and input file
    Parser(ParsingTable& table, const std::string& inputFile) : parsingTable(table) {
        tokenizeInput(inputFile);  // Tokenize the input file
        parseStack.push("$");  // End marker
        parseStack.push("S");  // Start symbol (assumed)
    }

    // Function to display the current state of the parse stack
    void displayStack() const {
        std::stack<std::string> tempStack = parseStack;
        std::cout << "Parse Stack: ";
        while (!tempStack.empty()) {
            std::cout << tempStack.top() << " ";
            tempStack.pop();
        }
        std::cout << std::endl;
    }

    // Function to display the current state of the input tokens
    void displayInput() const {
        std::cout << "Input Tokens: ";
        for (const auto& token : inputTokens) {
            std::cout << token << " ";
        }
        std::cout << std::endl;
    }

    // Function to perform LL(1) parsing
    void parse() {
        std::string currentToken = getNextToken();
        
        while (!parseStack.empty()) {
            std::string topOfStack = parseStack.top();  // Get the symbol at the top of the stack
            parseStack.pop();  // Pop it off the stack

            // Check if we've successfully parsed the entire input
            if (topOfStack == "$" && currentToken == "$") {
                std::cout << "Parsing successful! Reached end of input." << std::endl;
                return;
            }

            // If the top of the stack is a terminal
            if (isTerminal(topOfStack)) {
                if (topOfStack == currentToken) {
                    std::cout << "Matched terminal: " << currentToken << std::endl;
                    inputTokens.erase(inputTokens.begin());  // Move to next token
                    currentToken = getNextToken();  // Get the next token
                } else {
                    std::cout << "Error: Unexpected token '" << currentToken << "' when expecting '" << topOfStack << "'." << std::endl;
                    return;  // Parsing failed due to a mismatch
                }
            } else {
                // If the top of the stack is a non-terminal, apply the production rule
                std::string rule = parsingTable.getEntry(topOfStack, currentToken);
                if (!rule.empty()) {
                    std::cout << "Expanding non-terminal '" << topOfStack << "' with rule: " << rule << std::endl;

                    // Push the rule's right-hand side in reverse order onto the stack
                    std::istringstream ruleStream(rule);
                    std::string symbol;
                    while (ruleStream >> symbol) {
                        if (symbol != "epsilon") {  // Skip epsilon (empty) rules
                            parseStack.push(symbol);  // Push non-terminals to the stack
                        }
                    }
                } else {
                    std::cout << "Error: No matching rule for '" << topOfStack << "' with input '" << currentToken << "'." << std::endl;
                    return;  // Parsing failed due to no matching rule
                }
            }
        }

        // If we've exhausted the stack without fully parsing the input
        std::cout << "Parsing failed: Stack is empty but input is not completely parsed." << std::endl;
    }

    // Helper function to check if a symbol is a terminal
    bool isTerminal(const std::string& symbol) {
        return std::find(parsingTable.getTerminals().begin(), parsingTable.getTerminals().end(), symbol) != parsingTable.getTerminals().end();
    }
};

#endif
