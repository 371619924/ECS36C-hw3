#include <iostream>     // For standard I/O.
#include <string>       // For std::string.
#include <stdexcept>    // For exception handling.
#include <cstdlib>      // For std::stoul and other conversions.
#include "multiset.h"   // For the Multiset class.

// -----------------------------------------------------------------------------
// Function: factorize
// Description:
//   Factorizes the given unsigned integer n into its prime factors and inserts each
//   prime factor (with multiplicity) into the provided multiset 'factors'.
// Parameters:
//   - n: The number to factorize.
//   - factors: A reference to a Multiset where the prime factors will be stored.
// Returns:
//   - true if the factorization yields factors (and n is not prime itself),
//     false otherwise (e.g. when n is prime).
// -----------------------------------------------------------------------------
bool factorize(unsigned int n, Multiset<unsigned int> &factors) {
    unsigned int original = n;  // Save original number for comparison.
    // Loop from 2 to sqrt(n) to extract factors.
    for (unsigned int d = 2; d * d <= n; d++) {
        // While d divides n, record d and reduce n.
        while (n % d == 0) {
            factors.Insert(d);  // Insert factor d.
            n /= d;             // Divide n by d.
        }
    }
    // If n is greater than 1, then n is a prime factor.
    if (n > 1) {
        factors.Insert(n);
    }
    // Check: if the only factor is the original number, then n was prime.
    if (!factors.Empty() && factors.Min() == original)
        return false;
    return !factors.Empty();
}

// -----------------------------------------------------------------------------
// Function: main
// Description:
//   The main function processes command-line arguments to factorize a number and
//   perform various queries on its prime factors. Supported commands include:
//     - "all": print all prime factors with their counts.
//     - "min": print the smallest prime factor.
//     - "max": print the largest prime factor.
//     - "near": print a prime factor near a given target (with options to search for
//               the next greater or smaller prime).
// -----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    // Check that at least two additional arguments are provided.
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <number> <command> [<args>]" << std::endl;
        return 1;
    }

    unsigned int number = 0;
    // Convert the first argument to an unsigned integer.
    try {
        number = std::stoul(argv[1]);
    } catch (const std::exception &) {
        std::cerr << "Invalid number" << std::endl;
        return 1;
    }

    // Retrieve the command argument.
    std::string command(argv[2]);
    // Validate the command.
    if (command != "all" && command != "min" && command != "max" && command != "near") {
        std::cerr << "Command '" << command << "' is invalid" << std::endl;
        std::cerr << "Possible commands are: all|min|max|near" << std::endl;
        return 1;
    }

    std::string nearArg;
    // If the command is "near", an additional argument is expected.
    if (command == "near") {
        if (argc < 4) {
            std::cerr << "Command 'near' expects another argument: [+/-]prime" << std::endl;
            return 1;
        }
        nearArg = argv[3];
    }

    // Create a Multiset to hold the prime factors.
    Multiset<unsigned int> factors;
    bool hasFactors = factorize(number, factors);
    if (!hasFactors) {
        std::cout << "No prime factors" << std::endl;
        return 0;
    }

    // Process the specified command.
    if (command == "all") {
        // Print all prime factors with their counts.
        try {
            // Start with the minimum (smallest) prime factor.
            unsigned int key = factors.Min();
            // Loop indefinitely until an exception indicates no further factors.
            while (true) {
                std::cout << key << " (x" << factors.Count(key) << "), ";
                // Get the next prime factor greater than current key.
                key = factors.Ceil(key + 1);
            }
        } catch (std::runtime_error &e) {
            // When no further factor exists, break out of loop and end the line.
            std::cout << std::endl;
        }
    }
    else if (command == "min") {
        // Print the smallest prime factor.
        unsigned int key = factors.Min();
        std::cout << key << " (x" << factors.Count(key) << ")" << std::endl;
    }
    else if (command == "max") {
        // Print the largest prime factor.
        unsigned int key = factors.Max();
        std::cout << key << " (x" << factors.Count(key) << ")" << std::endl;
    }
    else if (command == "near") {
        // Handle the "near" command which supports finding a factor near a target.
        int target;
        bool findGreater = false, findSmaller = false;
        std::string arg = nearArg;
        // If the argument begins with '+' or '-', set the search direction.
        if (!arg.empty() && (arg[0] == '+' || arg[0] == '-')) {
            if (arg[0] == '+') {
                findGreater = true;
            } else {
                findSmaller = true;
            }
            // Remove the sign from the argument.
            arg = arg.substr(1);
        }
        try {
            target = std::stoi(arg);
        } catch (const std::exception &) {
            std::cerr << "Invalid prime" << std::endl;
            return 1;
        }
        if (target <= 0) {
            std::cout << "No match" << std::endl;
        } else {
            unsigned int utarget = static_cast<unsigned int>(target);
            if (findGreater) {
                // Find the smallest factor greater than utarget.
                try {
                    unsigned int candidate = factors.Ceil(utarget + 1);
                    std::cout << candidate << " (x" << factors.Count(candidate) << ")" << std::endl;
                } catch (std::runtime_error &e) {
                    std::cout << "No match" << std::endl;
                }
            } else if (findSmaller) {
                // Find the greatest factor less than utarget.
                if (factors.Min() >= utarget) {
                    std::cout << "No match" << std::endl;
                } else {
                    try {
                        unsigned int candidate = factors.Floor(utarget - 1);
                        std::cout << candidate << " (x" << factors.Count(candidate) << ")" << std::endl;
                    } catch (std::runtime_error &e) {
                        std::cout << "No match" << std::endl;
                    }
                }
            } else {
                // If no sign is given, check for an exact match.
                if (factors.Contains(utarget)) {
                    std::cout << utarget << " (x" << factors.Count(utarget) << ")" << std::endl;
                } else {
                    std::cout << "No match" << std::endl;
                }
            }
        }
    }

    return 0;
}
