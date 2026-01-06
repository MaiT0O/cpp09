#include "PmergeMe.hpp"

bool isNumber(const std::string& s)
{
    if (s.empty())
        return false;

    for (size_t i = 0; i < s.length(); i++)
    {
        if (!std::isdigit(s[i]))
            return false;
    }
    return true;
}

PmergeMe::PmergeMe(char **argv)
{
    size_t i = 0;
    while (argv[++i])
    {
        std::string token(argv[i]);
        if (!isNumber(token))
            throw std::runtime_error("Error");
            
        long n = strtol(token.c_str(), NULL, 10);
        if (n < 0 || n > INT_MAX)
            throw std::runtime_error("Error");
        
        _vec.push_back(n);
        _deq.push_back(n);
    }
}

void PmergeMe::sortAndDisplay()
{
    std::cout << "Before : ";
    for (size_t i = 0; i < _vec.size(); i++)
    {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "After : ";
    for (size_t i = 0; i < _vec.size(); i++)
    {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;
}