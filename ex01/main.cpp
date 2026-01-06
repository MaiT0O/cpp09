#include "RPN.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error usage : ./RPN [string]" << std::endl;
        return 1;
    }

    try
    {
        RPN obj;
        obj.evaluate(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return (0);
}