#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

class MyMultiset
{
public:
    using Element = std::pair<unsigned int, unsigned int>;

    void insert(unsigned int x)
    {
        auto it = std::lower_bound(data.begin(), data.end(), x,
                                   [](const Element &elem, unsigned int value)
                                   {
                                       return elem.first < value;
                                   });
        if (it != data.end() && it->first == x)
        {
            it->second++;
        }
        else
        {
            data.insert(it, Element(x, 1));
        }
    }

    std::vector<Element>::iterator begin() { return data.begin(); }
    std::vector<Element>::iterator end() { return data.end(); }
    std::vector<Element>::const_iterator begin() const { return data.begin(); }
    std::vector<Element>::const_iterator end() const { return data.end(); }

    bool empty() const { return data.empty(); }

    size_t size() const { return data.size(); }

    Element min() const { return data.front(); }

    Element max() const { return data.back(); }

    std::vector<Element>::iterator find(unsigned int x)
    {
        auto it = std::lower_bound(data.begin(), data.end(), x,
                                   [](const Element &elem, unsigned int value)
                                   {
                                       return elem.first < value;
                                   });
        if (it != data.end() && it->first == x)
            return it;
        return data.end();
    }

    std::vector<Element>::iterator lower_bound_gt(unsigned int x)
    {

        return std::upper_bound(data.begin(), data.end(), x,
                                [](unsigned int value, const Element &elem)
                                {
                                    return value < elem.first;
                                });
    }

    std::vector<Element>::iterator upper_bound_lt(unsigned int x)
    {
        auto it = std::lower_bound(data.begin(), data.end(), x,
                                   [](const Element &elem, unsigned int value)
                                   {
                                       return elem.first < value;
                                   });
        if (it == data.begin())
            return data.end();
        --it;
        return it;
    }

private:
    std::vector<Element> data;
};

bool factorize(unsigned int n, MyMultiset &factors)
{
    unsigned int original = n;

    for (unsigned int d = 2; d * d <= n; d++)
    {
        while (n % d == 0)
        {
            factors.insert(d);
            n /= d;
        }
    }

    if (n > 1)
    {
        factors.insert(n);
    }

    if (factors.size() == 1 && factors.begin()->first == original)
        return false;

    return !factors.empty();
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <number> <command> [<args>]" << std::endl;
        return 1;
    }

    unsigned int number = 0;
    try
    {

        number = std::stoul(argv[1]);
    }
    catch (const std::exception &)
    {
        std::cerr << "Invalid number" << std::endl;
        return 1;
    }

    std::string command(argv[2]);

    if (command != "all" && command != "min" && command != "max" && command != "near")
    {
        std::cerr << "Command '" << command << "' is invalid" << std::endl;
        std::cerr << "Possible commands are: all|min|max|near" << std::endl;
        return 1;
    }

    std::string nearArg;
    if (command == "near")
    {
        if (argc < 4)
        {
            std::cerr << "Command 'near' expects another argument: [+/-]prime" << std::endl;
            return 1;
        }
        nearArg = argv[3];
    }

    MyMultiset factors;
    bool hasProperFactors = factorize(number, factors);

    if (!hasProperFactors)
    {
        std::cout << "No prime factors" << std::endl;
        return 0;
    }

    if (command == "all")
    {

        for (const auto &elem : factors)
        {
            std::cout << elem.first << " (x" << elem.second << "), ";
        }
        std::cout << std::endl;
    }
    else if (command == "min")
    {
        auto elem = factors.min();
        std::cout << elem.first << " (x" << elem.second << ")" << std::endl;
    }
    else if (command == "max")
    {
        auto elem = factors.max();
        std::cout << elem.first << " (x" << elem.second << ")" << std::endl;
    }
    else if (command == "near")
    {

        int target = 0;
        bool findGreater = false, findSmaller = false;
        std::string numStr = nearArg;
        if (!numStr.empty() && (numStr[0] == '+' || numStr[0] == '-'))
        {
            if (numStr[0] == '+')
            {
                findGreater = true;
            }
            else
            {
                findSmaller = true;
            }

            numStr = numStr.substr(1);
        }
        try
        {
            target = std::stoi(numStr);
        }
        catch (const std::exception &)
        {
            std::cerr << "Invalid prime" << std::endl;
            return 1;
        }

        std::vector<MyMultiset::Element>::iterator it;
        if (findGreater)
        {
            it = factors.lower_bound_gt(static_cast<unsigned int>(target));
        }
        else if (findSmaller)
        {
            it = factors.upper_bound_lt(static_cast<unsigned int>(target));
        }
        else
        {

            it = factors.find(static_cast<unsigned int>(target));
        }
        if (it != factors.end())
        {
            std::cout << it->first << " (x" << it->second << ")" << std::endl;
        }
        else
        {
            std::cout << "No match" << std::endl;
        }
    }

    return 0;
}
