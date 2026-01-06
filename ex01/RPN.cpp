#include "RPN.hpp"

RPN::RPN() {}
RPN::~RPN() {}

void RPN::evaluate(const std::string& expression)
{
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token)
    {
        if (isdigit(token[0]) && token.length() == 1)
            _stack.push(token[0] - '0');
        else if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            int _b = _stack.top(); _stack.pop();
            int _a = _stack.top(); _stack.pop();

            if (token == "+")
                _stack.push(_a + _b);
            else if (token == "-")
                _stack.push(_a - _b);
            else if (token == "*")
                _stack.push(_a * _b);
            else if (token == "/")
            {
                if (_b == 0)
                    throw std::runtime_error("Error");
                _stack.push(_a / _b);
            }
        }
        else
            throw std::runtime_error("Error");        
    }
    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    std::cout << _stack.top() << std::endl;
}