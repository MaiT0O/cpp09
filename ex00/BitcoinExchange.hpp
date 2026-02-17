#pragma once

# include <map>
# include <string>
# include <exception>
# include <iostream>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <ctime>
# include <cstdlib>
# include <stdexcept>
# include <cctype>
# include <limits>
# include <cmath>
# include <algorithm>

class BitcoinExchange
{
    private:
        std::map<std::string, double> _data;
        void isValidDate(const std::string& date) const;
        void isValidValue(const std::string& value) const;
        double getExchangeRate(const std::string& date) const; 

    public:
        BitcoinExchange();
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);

        void    loadData(const std::string& filename);
        void    processInput(const std::string& filename) const;
};