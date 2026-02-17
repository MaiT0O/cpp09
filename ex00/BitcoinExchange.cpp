# include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy) : _data(copy._data)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        this->_data = other._data;
    }
    return *this;
}

void    BitcoinExchange::isValidDate(const std::string& date) const
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        throw std::runtime_error("Invalid date format");

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    bool isLeap = (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);

    int maxDay;
    if (month == 2)
        maxDay = isLeap ? 29 : 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        maxDay = 30;
    else
        maxDay = 31;

    if (year < 2009 || month < 1 || month > 12 || day < 1 || day > maxDay)
        throw std::runtime_error("Invalid date value");
}

void    BitcoinExchange::isValidValue(const std::string& value) const
{
    double val = std::strtod(value.c_str(), NULL);
    if (val < 0)
        throw std::runtime_error("not a positive number.");
    if (val > 1000)
        throw std::runtime_error("too large a number.");
}

double BitcoinExchange::getExchangeRate(const std::string& date) const
{
    std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
    if (it == _data.end() || it->first != date)
    {
        if (it == _data.begin())
            throw std::runtime_error("No exchange rate available for this date");
        --it;
    }
    return it->second;
}

void    BitcoinExchange::loadData(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open data file");

    std::string line;
    std::getline(file, line);
    if (line != "date,exchange_rate")
        throw std::runtime_error("Invalid data file header");
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date, valueStr;
        if (std::getline(ss, date, ',') && std::getline(ss, valueStr))
        {
            if (date.empty() || valueStr.empty())
                continue;
            isValidDate(date);
            double value = std::strtod(valueStr.c_str(), NULL);
            _data[date] = value;
        }
    }
    file.close();
}

std::string remove_whitespace(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!std::isspace(s[i]))
            out.push_back(static_cast<char>(s[i]));
    }
    return out;
}

void    BitcoinExchange::processInput(const std::string& filename) const
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open input file");

    std::string line;
    std::getline(file, line);
    if (line != "date | value")
        throw std::runtime_error("Invalid input file header");
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date, valueStr;
        if (std::getline(ss, date, '|') && std::getline(ss, valueStr))
        {
            if (date.empty() || valueStr.empty())
            {
                std::cerr << "Error : date or value empty" << std::endl;
                continue;
            }
            try
            {
                date = remove_whitespace(date);
                valueStr = remove_whitespace(valueStr);
                isValidDate(date);
                isValidValue(valueStr);
                double value = std::strtod(valueStr.c_str(), NULL);
                double rate = getExchangeRate(date);
                std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error : " << e.what() << std::endl;
                continue;
            }
        }
        else if (!line.empty())
        {
            std::getline(ss, line);
            std::cerr << "Error : bad input => " << line << std::endl;
            continue;
        }
    }
    file.close();
}
