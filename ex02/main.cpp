#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error usage" << std::endl;
        return 1;
    }

    try
    {
        PmergeMe pm(argv);
        pm.sortAndDisplay();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;    
}