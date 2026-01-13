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

PmergeMe::~PmergeMe() {}

template<typename Container>
void makePairs(const Container& input, Container& mainChain, Container& pend, int& straggler)
{
    size_t i = 0;
    for (; i + 1 < input.size(); i += 2)
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

template<typename Container>
Container merge(const Container& left, const Container& right)
{
    Container result;
    size_t i = 0, j = 0;

    while (i < left.size() && j < right.size())
    {
        if (left[i] < right[j])
            result.push_back(left[i++]);
        else
            result.push_back(right[j++]);
    }

    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);

    return result;
}

template<typename Container>
Container mergeSort(const Container& v)
{
    if (v.size() <= 1)
        return v;

    size_t mid = v.size() / 2;
    Container left(v.begin(), v.begin() + mid);
    Container right(v.begin() + mid, v.end());
    return merge(mergeSort(left), mergeSort(right));
}

std::vector<size_t> generateJacobsthalSequence(size_t n)
{
    std::vector<size_t> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);
    
    size_t next = 1;
    while (next < n)
    {
        next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
        jacobsthal.push_back(next);
    }
    
    return jacobsthal;
}

template<typename Container>
void binaryInsert(Container& vec, int value)
{
    typename Container::iterator it = std::lower_bound(vec.begin(), vec.end(), value);
    vec.insert(it, value);
}

template<typename Container>
void insertPendElements(Container& mainChain, Container& pend)
{
    if (pend.empty())
        return;

    mainChain.insert(mainChain.begin(), pend[0]);

    std::vector<size_t> jacob = generateJacobsthalSequence(pend.size());
    std::vector<bool> inserted(pend.size(), false);
    inserted[0] = true;

    for (size_t i = 2; i < jacob.size() && jacob[i] < pend.size(); i++)
    {
        size_t pos = jacob[i];
        for (size_t j = pos; j > jacob[i - 1] && j < pend.size(); j--)
        {
            if (!inserted[j])
            {
                binaryInsert(mainChain, pend[j]);
                inserted[j] = true;
            }
        }
    }

    for (size_t i = 1; i < pend.size(); i++)
    {
        if (!inserted[i])
            binaryInsert(mainChain, pend[i]);
    }
}

void PmergeMe::mergeInsertSortVector()
{
    if (_vec.size() <= 1)
    return ;
    
    std::vector<int> mainChain;
    std::vector<int> pend;
    int straggler;
    
    makePairs(_vec, mainChain, pend, straggler);

    mainChain = mergeSort(mainChain);
    
    insertPendElements(mainChain, pend);
    
    if (straggler != -1)
    binaryInsert(mainChain, straggler);
    
    _vec = mainChain;
}

void PmergeMe::mergeInsertSortDeque()
{
    if (_deq.size() <= 1)
    return ;
    
    std::deque<int> mainChain;
    std::deque<int> pend;
    int straggler;
    
    makePairs(_deq, mainChain, pend, straggler);

    mainChain = mergeSort(mainChain);
    
    insertPendElements(mainChain, pend);
    
    if (straggler != -1)
    binaryInsert(mainChain, straggler);
    
    _deq = mainChain;
}

void PmergeMe::sortAndDisplay()
{
    std::cout << "Before: ";
    size_t max = (_vec.size() > 10) ? 5 : _vec.size();
    for (size_t i = 0; i < max; i++)
        std::cout << _vec[i] << " ";
    if (max == 5)
        std::cout << "[...]";
    std::cout << std::endl;
    
    clock_t start = clock();
    mergeInsertSortVector();
    clock_t end = clock();
    double timeVec = static_cast<double>(end - start) / CLOCKS_PER_SEC ;
    
    start = clock();
    mergeInsertSortDeque();
    end = clock();
    double timeDeq = static_cast<double>(end - start) / CLOCKS_PER_SEC ;

    std::cout << "After: ";
    for (size_t i = 0; i < max; i++)
        std::cout << _vec[i] << " ";
    if (max == 5)
        std::cout << "[...]";
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size() 
     << " elements with std::vector : " << std::fixed 
     << std::setprecision(5) << timeVec << " us" << std::endl;
              
    std::cout << "Time to process a range of " << _deq.size() 
     << " elements with std::deque : " << std::fixed 
     << std::setprecision(5) << timeDeq << " us" << std::endl;
}