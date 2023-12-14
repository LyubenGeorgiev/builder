#include "expression.h"
#include "OperationSet.h"
#include "Stack.h"
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include <string>

bool lowerOrEqualPrecedenceOperationUpcomming(const OperationSet &opSet, const Stack<char> &ops, const std::string &tokens, const int i)
{
  return ops.top() != '(' && opSet.get(ops.top())->getPrecedence() >= opSet.get(tokens[i])->getPrecedence();
}

void applyTopOperation(Stack<double> &values,
                       Stack<char> &operations,
                       const OperationSet &opSet)
{
  double val2 = values.top();
  values.pop();

  double val1 = values.top();
  values.pop();

  char op = operations.top();
  operations.pop();

  values.push(opSet.get(op)->apply(val1, val2));
}

bool checkExpression(const std::string &expression, const OperationSet &allowedOperations)
{
  Stack<double> operands;
  Stack<char> operations;
  bool isLastSymbolMinus = false;
  bool isPreviousNonWhiteSpace = true;
  for (int i = 0; i < expression.length(); i++)
  {
    if (isLastSymbolMinus && !isdigit(expression[i]))
      throw incorrect_expression("A number is expected after minus " + expression);
    if (expression[i] == ' ')
    {
      isPreviousNonWhiteSpace = true;
      continue;
    }

    if (isalpha(expression[i]))
    {
      if (allowedOperations.contains(expression[i]) && isPreviousNonWhiteSpace)
        operations.push(expression[i]);
      else
        throw incorrect_expression("Unknown operation. " + expression);
    }

    if (expression[i] == '(')
      if (isPreviousNonWhiteSpace)
        operations.push(expression[i]);
      else
        throw incorrect_expression("Two opening brackets close to oneanother, " + expression);

    if (expression[i] == ')')
    {
      if (operations.empty() || !isPreviousNonWhiteSpace)
        throw incorrect_expression("Unopened closing bracket found or two opening brackets close to one another. " + expression);
      if (operations.top() == '(' && operands.empty())
        throw incorrect_expression("Empty brackets encountered.  " + expression);
      while (!operations.empty() && operations.top() != '(')
      {
        if (operands.getSize() < 2)
          throw incorrect_expression("Line 58 " + expression);
        operands.pop();
        operations.pop();
      }
      if (operations.empty())
        throw incorrect_expression("Line 63 " + expression);
      operations.pop();
    }
    if (expression[i] == '-')
    {
      isLastSymbolMinus = true;
    }
    if (isdigit(expression[i]) && (isPreviousNonWhiteSpace || isLastSymbolMinus))
    {
      int val = 0;
      while (i < expression.length() &&
             isdigit(expression[i]))
      {
        val = (val * 10) + (expression[i] - '0');
        i++;
      }
      if (isLastSymbolMinus)
      {
        val = -val;
        isLastSymbolMinus = false;
      }
      operands.push(val);
      i--;
    }
    isPreviousNonWhiteSpace = expression[i] == ' ';
  }
  while (!operations.empty())
  {
    if (!isalpha(operations.top()) || operands.getSize() < 2)
      throw incorrect_expression("Insuficient operands or operations. " + expression);
    operands.pop();
    operations.pop();
  }

  return operations.empty() && operands.getSize() == 1;
}

///
/// @brief Evaluates an expression.
///
/// @param expression
///   A null-terminated string that contains the expression.
/// @param ops
///   An input stream which contains the descriptions of all operations used in the expression.
///
/// @return The calculated value of the expression
double evaluate(const char *expression, std::istream &ops)
{
  if (expression == nullptr)
    throw incorrect_expression("The expression is nullptr.");
  std::string exp(expression);
  // If the string is empty or contains only intervals return 0.
  if (exp.empty() || exp.find_first_not_of(' ') == std::string::npos)
    return 0;
  OperationSet opSet = OperationSet();
  opSet.init(ops);
  if (!checkExpression(exp, opSet))
    throw incorrect_expression(exp);

  bool isLastSymbolMinus = false;
  // stack to store integer values.
  Stack<double> values;

  // stack to store operators.
  Stack<char> parsedOps;

  for (int i = 0; i < exp.length(); i++)
  {
    // Current character is a whitespace,
    // skip it.
    if (exp[i] == ' ')
      continue;

    // Current character is an opening
    // brace, push it to 'parsedOps'
    else if (exp[i] == '(')
    {
      parsedOps.push(exp[i]);
    }
    else if (exp[i] == '-')
    {
      isLastSymbolMinus = true;
    }
    // Current character is a digit, push
    // it to stack for numbers.
    else if (isdigit(exp[i]))
    {
      int val = 0;

      // There may be more than one
      // digits in number.
      while (i < exp.length() &&
             isdigit(exp[i]))
      {
        val = (val * 10) + (exp[i] - '0');
        i++;
      }
      if (isLastSymbolMinus)
      {
        val = -val;
        isLastSymbolMinus = false;
      }
      values.push(val);

      // The 'i' points to the character
      // next to the digit,
      // since the while loop also
      // increases the counter, we would skip one
      // character position.
      // We need to decrease the value of i by 1 to
      // correct the offset.
      i--;
    }

    // Closing brace encountered, solve
    // expression within.
    else if (exp[i] == ')')
    {
      while (!parsedOps.empty() && parsedOps.top() != '(')
      {
        applyTopOperation(values, parsedOps, opSet);
      }

      // Pop the opening brace.
      if (!parsedOps.empty())
        parsedOps.pop();
    }

    // Current token is an operator.
    else
    {
      // While top of 'parsedOps' has the same or greater
      // precedence to current operator, apply operator on top
      // of 'parsedOps' to top two elements in values stack.
      while (!parsedOps.empty() && lowerOrEqualPrecedenceOperationUpcomming(opSet, parsedOps, exp, i))
      {
        if (opSet.get(parsedOps.top())->getAssociation() == 'R' && *opSet.get(exp[i]) == *opSet.get(parsedOps.top()))
          break;
        applyTopOperation(values, parsedOps, opSet);
      }

      // Push current token to 'ops'.
      parsedOps.push(exp[i]);
    }
  }

  // Entire expression has been parsed at this
  // point, apply remaining ops to remaining
  // values.
  while (!parsedOps.empty())
  {
    applyTopOperation(values, parsedOps, opSet);
  }

  // Top of 'values' contains result, return it.
  return values.top();
}