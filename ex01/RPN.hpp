#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

class RPN
{
    private:
        std::stack<int> _stack;
    
    public:
        RPN();
        ~RPN();

        void    evaluate(const std::string& expression);
};

#endif