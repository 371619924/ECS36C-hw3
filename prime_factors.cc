#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "multiset.h"

bool factorize(unsigned int n, Multiset<unsigned int> &factors)
{
    unsigned int original = n;
    for (unsigned int d = 2; d * d <= n; d++)
    {
        while (n % d == 0)
        {
            factors.Insert(d);
            n /= d;
        }
    }
    if (n > 1)
    {
        factors.Insert(n);
    }

    if (!factors.Empty() && factors.Min() == original)
        return false;
    return !factors.Empty();
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

    Multiset<unsigned int> factors;
    bool hasFactors = factorize(number, factors);
    if (!hasFactors)
    {
        std::cout << "No prime factors" << std::endl;
        return 0;
    }
    if (command == "all")
    {

        try
        {
            unsigned int key = factors.Min();
            while (true)
            {
                std::cout << key << " (x" << factors.Count(key) << "), ";

                key = factors.Ceil(key + 1);
            }
        }
        catch (std::runtime_error &e)
        {
            std::cout << std::endl;
        }
    }
    else if (command == "min")
    {
        unsigned int key = factors.Min();
        std::cout << key << " (x" << factors.Count(key) << ")" << std::endl;
    }
    else if (command == "max")
    {
        unsigned int key = factors.Max();
        std::cout << key << " (x" << factors.Count(key) << ")" << std::endl;
    }
    else if (command == "near")
    {
        int target;
        bool findGreater = false, findSmaller = false;
        std::string arg = nearArg;
        if (!arg.empty() && (arg[0] == '+' || arg[0] == '-'))
        {
            if (arg[0] == '+')
            {
                findGreater = true;
            }
            else
            {
                findSmaller = true;
            }
            arg = arg.substr(1);
        }
        try
        {
            target = std::stoi(arg);
        }
        catch (const std::exception &)
        {
            std::cerr << "Invalid prime" << std::endl;
            return 1;
        }

        if (target <= 0)
        {
            std::cout << "No match" << std::endl;
        }
        else
        {
            unsigned int utarget = static_cast<unsigned int>(target);
            if (findGreater)
            {

                try
                {
                    unsigned int candidate = factors.Ceil(utarget + 1);
                    std::cout << candidate << " (x" << factors.Count(candidate) << ")" << std::endl;
                }
                catch (std::runtime_error &e)
                {
                    std::cout << "No match" << std::endl;
                }
            }
            else if (findSmaller)
            {

                if (factors.Min() >= utarget)
                {
                    std::cout << "No match" << std::endl;
                }
                else
                {
                    try
                    {

                        unsigned int candidate = factors.Floor(utarget - 1);
                        std::cout << candidate << " (x" << factors.Count(candidate) << ")" << std::endl;
                    }
                    catch (std::runtime_error &e)
                    {
                        std::cout << "No match" << std::endl;
                    }
                }
            }
            else
            {
                if (factors.Contains(utarget))
                {
                    std::cout << utarget << " (x" << factors.Count(utarget) << ")" << std::endl;
                }
                else
                {
                    std::cout << "No match" << std::endl;
                }
            }
        }
    }

    return 0;
}
