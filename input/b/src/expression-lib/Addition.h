#pragma once
#include "Operation.h"

class Addition : public Operation
{
public:
    Addition(int _precedence, char _association) : Operation(_precedence, _association)
    {
    }

    virtual double apply(const double &a, const double &b) const override
    {
        return a + b;
    }
};