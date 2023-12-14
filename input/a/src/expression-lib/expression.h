// expression.h
#pragma once

#include <iostream>
#include <exception>
#include <map>
#include <vector>
#include <stack>
#include <sstream>

struct Operation {
    char opSymbol;
    char op;
    int priority;
    char associativity;
};

class incorrect_expression : public std::invalid_argument {
public:
    incorrect_expression(const std::string& what_arg)
        : invalid_argument(what_arg) {
        // Nothing to do here
    }
};

double evaluate(const char* expression, std::istream& ops);

void handleOperations(std::stack<char>& operators, std::stack<double>& values, char symbol, std::map<char, Operation>& operations, bool& expectingOp, char& prev);
void evaluateOperation(char op, std::stack<double>& values);
bool hasPrecedence(char op1, char op2, std::map<char, Operation>& operations);
