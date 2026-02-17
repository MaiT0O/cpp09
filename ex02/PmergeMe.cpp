#include "PmergeMe.hpp"

bool isNumber(const std::string& s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.length(); i++)
        if (!std::isdigit(s[i]))
            return false;
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

        _vec.push_back(static_cast<int>(n));
        _deq.push_back(static_cast<int>(n));
    }
    if (_vec.empty())
        throw std::runtime_error("Error: empty sequence");
}

PmergeMe::~PmergeMe() {}

std::vector<size_t> jacobsthalOrder(size_t size)
{
    std::vector<size_t> jac;
    jac.push_back(0);
    jac.push_back(1);
    while (jac.back() < size)
    {
        size_t n = jac.size();
        jac.push_back(jac[n - 1] + 2 * jac[n - 2]);
    }

    std::vector<size_t> order;
    std::vector<bool>   inserted(size, false);

    for (size_t k = 2; k < jac.size(); k++)
    {
        size_t high = jac[k] - 1;
        if (high >= size)
            high = size - 1;

        for (long i = (long)high; i >= (long)jac[k - 1]; i--)
        {
            if ((size_t)i < size && !inserted[(size_t)i])
            {
                order.push_back((size_t)i);
                inserted[(size_t)i] = true;
            }
        }
    }
    for (size_t i = 0; i < size; i++)
        if (!inserted[i])
            order.push_back(i);
    return order;
}

template <typename Iterator>
Iterator binarySearch(Iterator begin, Iterator end, int val)
{
    while (begin < end)
    {
        Iterator mid = begin + (end - begin) / 2;
        if (*mid < val)
            begin = mid + 1;
        else
            end = mid;
    }
    return begin;
}

template <typename Container>
void fordJohnson(Container& v)
{
    size_t n = v.size();
    if (n <= 1)
        return;

    int  straggler    = -1;
    bool hasStraggler = (n % 2 != 0);

    Container mainChain;
    Container pend;

    for (size_t i = 0; i + 1 < n; i += 2)
    {
        if (v[i] > v[i + 1])
        {
            mainChain.push_back(v[i]);
            pend.push_back(v[i + 1]);
        }
        else
        {
            mainChain.push_back(v[i + 1]);
            pend.push_back(v[i]);
        }
    }
    if (hasStraggler)
        straggler = v[n - 1];

    fordJohnson(mainChain);

    Container sorted(mainChain.begin(), mainChain.end());

    sorted.insert(
        binarySearch(sorted.begin(), sorted.end(), pend[0]),
        pend[0]
    );

    size_t pendSize = pend.size();
    if (pendSize > 1)
    {
        std::vector<size_t> order = jacobsthalOrder(pendSize - 1);

        for (size_t k = 0; k < order.size(); k++)
        {
            size_t idx = order[k] + 1;
            if (idx >= pendSize)
                continue;

            int val = pend[idx];
            sorted.insert(
                binarySearch(sorted.begin(), sorted.end(), val),
                val
            );
        }
    }

    if (hasStraggler)
        sorted.insert(
            binarySearch(sorted.begin(), sorted.end(), straggler),
            straggler
        );

    v = sorted;
}

void PmergeMe::sortAndDisplay()
{
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;

    struct timespec startVec, endVec;
    clock_gettime(CLOCK_MONOTONIC, &startVec);
    fordJohnson(_vec);
    clock_gettime(CLOCK_MONOTONIC, &endVec);

    struct timespec startDeq, endDeq;
    clock_gettime(CLOCK_MONOTONIC, &startDeq);
    fordJohnson(_deq);
    clock_gettime(CLOCK_MONOTONIC, &endDeq);

    std::cout << "After:  ";
    for (size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;

    double timeVec = (endVec.tv_sec - startVec.tv_sec) * 1e6
                   + (endVec.tv_nsec - startVec.tv_nsec) / 1e3;
    double timeDeq = (endDeq.tv_sec - startDeq.tv_sec) * 1e6
                   + (endDeq.tv_nsec - startDeq.tv_nsec) / 1e3;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : " << timeVec << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque  : " << timeDeq << " us" << std::endl;
}
