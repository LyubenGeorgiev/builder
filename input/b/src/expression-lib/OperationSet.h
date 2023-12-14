#pragma once
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include <sstream>
#include <string>

class OperationSet
{
private:
    // Operations can range from 'a' to 'z'
    // therefore they are finite and not that many.
    // We use an array to store them and
    // retrieve them for time O(c)
    Operation *operations[26] = {};

    int calculatePriority(const char *operationString)
    {
        std::string str(operationString);
        int result = 0;
        result = str[5] - '0';
        if (isdigit(str[4]))
        {
            return (str[4] - '0') * 10 + result;
        }
        return result;
    }

public:
    OperationSet() = default;

    void init(std::istream &operationStream)
    {
        while (operationStream.good() && !operationStream.eof())
        {
            char operationString[9];
            operationStream.getline(operationString, 10);

            Operation *operation = nullptr;
            switch (operationString[2])
            {
            case '+':
                operation = new Addition(calculatePriority(operationString), operationString[7]);
                break;
            case '-':
                operation = new Subtraction(calculatePriority(operationString), operationString[7]);
                break;
            case '*':
                operation = new Multiplication(calculatePriority(operationString), operationString[7]);
                break;
            case '/':
                operation = new Division(calculatePriority(operationString), operationString[7]);
                break;
            }
            add(operationString[0], operation);
        }
    }

    void add(const char &operationName, Operation *operation)
    {
        this->operations[int(tolower(operationName)) - 97] = operation;
    }
    Operation *get(const char &operationName) const
    {
        return this->operations[int(tolower(operationName)) - 97];
    }
    bool contains(const char &operationName) const
    {
        return this->get(operationName) != nullptr;
    }
    ~OperationSet()
    {
        for (int i = 0; i < 26; i++)
        {
            delete this->operations[i];
        }
    }
};