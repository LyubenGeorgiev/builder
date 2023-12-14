#include "expression.h"
#include <map>
#include <vector>
#include <stack>
#include <sstream>

double evaluate(const char* expression, std::istream& ops) {
    std::map<char, Operation> operations;

    char symbol;
    Operation operation;
    while (ops >> symbol >> operation.op >> operation.priority >> operation.associativity) {
        operations[symbol] = operation;
    }

    std::stack<char> operators;
    std::stack<double> values;
    char prev = '\0';
    bool expectingOp = false;

    for (const char* c = expression; *c; ++c) {
        if (*c == ' ')
            continue;

        if (*c == '(') {
            if (expectingOp && prev != '(') {
                throw incorrect_expression("Incorrect expression");
            }
            operators.push('(');
            expectingOp = false;
        }
        else if (*c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                handleOperations(operators, values, *c, operations, expectingOp, prev);
            }
            if (operators.empty()) {
                throw incorrect_expression("Incorrect expression");
            }
            operators.pop();
        }
        else if (isdigit(*c)) {
            char* end;
            double number = strtod(c, &end);
            values.push(number);
            c = end - 1;
            if (expectingOp) {
                throw incorrect_expression("Incorrect expression");
            }
            expectingOp = true;
        }
        else if (isalpha(*c)) {
            if (operations.find(*c) == operations.end()) {
                throw incorrect_expression("Incorrect expression");
            }
            handleOperations(operators, values, *c, operations, expectingOp, prev);
        }
        else {
            throw incorrect_expression("Incorrect expression");
        }
        prev = *c;
    }

    while (!operators.empty()) {
        if (operators.top() == '(') {
            throw incorrect_expression("Incorrect expression");
        }
        handleOperations(operators, values, operators.top(), operations, expectingOp, prev);
    }

    if (values.size() != 1) {
        throw incorrect_expression("Incorrect expression");
    }

    return values.top();
}

void handleOperations(std::stack<char>& operators, std::stack<double>& values, char symbol, std::map<char, Operation>& operations, bool& expectingOp, char& prev) {
    while (!operators.empty() && operators.top() != '(' && hasPrecedence(symbol, operators.top(), operations)) {
        evaluateOperation(operators.top(), values);
        operators.pop();
    }
    operators.push(symbol);
    expectingOp = false;
    prev = symbol;
}

void evaluateOperation(char op, std::stack<double>& values) {
    if (values.size() < 2) {
        throw incorrect_expression("Incorrect expression");
    }
    double val2 = values.top();
    values.pop();
    double val1 = values.top();
    values.pop();
    switch (op) {
    case '+':
        values.push(val1 + val2);
        break;
    case '-':
        values.push(val1 - val2);
        break;
    case '*':
        values.push(val1 * val2);
        break;
    case '/':
        if (val2 == 0) {
            throw incorrect_expression("Incorrect expression");
        }
        values.push(val1 / val2);
        break;
    default:
        throw incorrect_expression("Incorrect expression");
    }
}

bool hasPrecedence(char op1, char op2, std::map<char, Operation>& operations) {
    Operation operation1 = operations[op1];
    Operation operation2 = operations[op2];
    if (operation1.priority == operation2.priority) {
        return operation1.associativity == 'L';
    }
    return operation1.priority > operation2.priority;
}
