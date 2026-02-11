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

void    makePairs(const std::vector<int> & input, std::vector<int> & mainChain, std::vector<int> & pend, int& straggler)
{
    size_t i = 1;
    for (; i < input.size(); i += 2)
    {
        if (input[i] < input[i + 1])
        {
            mainChain.push_back(input[i + 1]);
            pend.push_back(input[i]);
        }
        else
        {
            mainChain.push_back(input[i]);
            pend.push_back(input[i + 1]);
        }
    }
    straggler = (i < input.size()) ? input[i] : -1;
}

std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right)
{
    std::vector<int> result;
    size_t i = 0; size_t j = 0;

    while (i < left.size() && j < right.size())
    {
        if (left[i] < right[j])
            result.push_back(left[i++]);
        else
            result.push_back(right[j++]);
    }

    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);

    return (result);
}

std::vector<int> mergeSort(const std::vector<int>& v)
{
    if (v.size() <= 1)
        return v;

    size_t mid = v.size() / 2;
    std::vector<int> left(v.begin(), v.begin() + mid);
    std::vector<int> right(v.begin() + mid, v.end());
    return (merge(mergeSort(left), mergeSort(right)));
}