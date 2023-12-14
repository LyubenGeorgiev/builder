#pragma once
class Operation
{
private:
    int precedence;
    char association;

public:
    Operation(int _precedence, char _association) : precedence(_precedence), association(_association) {}
    virtual double apply(const double &a, const double &b) const = 0;
    int getPrecedence() const
    {
        return this->precedence;
    }
    char getAssociation() const
    {
        return this->association;
    }
    bool operator==(const Operation &other)
    {
        return this->precedence == other.precedence && association == other.association;
    }
    ~Operation() {}
};