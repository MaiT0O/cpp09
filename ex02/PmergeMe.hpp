#include <string>
#include <vector>
#include <deque>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <climits>
#include <cctype>
#include <cstdlib>
#include <ctime>

class PmergeMe
{
    private:
        std::vector<int> _vec;
        std::deque<int>  _deq;

    public:
        PmergeMe(char **argv);
        ~PmergeMe();
        void sortAndDisplay();
};
