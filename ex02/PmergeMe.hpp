#include <string>
#include <vector>
#include <deque>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <climits>
#include <cctype>
#include <cstdlib>

class PmergeMe
{
    private:
        std::vector<int> _vec;
        std::deque<int>  _deq;

        void mergeInsertSortVector();
        void mergeInsertSortDeque();

    public:
        PmergeMe(char **argv);
        ~PmergeMe();
        void sortAndDisplay();
};
