#ifndef HASH_H
#define HASH_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5]{983132572, 1468777056, 552714139, 984953261, 261934300};
    MyStringHash(bool debug = true) {
        if (false == debug) {
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const {

        // Add your code here
        std::string cpy = k;        // copy of the key string
        int idx = (int)k.length();  // Get the current length of the string
        unsigned long long hash = 0;
        int w_idx = 4;  // index for sum

        unsigned long long sum[5];  // stores w values
        for (int j = 0; j <= 4; j++) {
            sum[j] = 0;
        }

        while (idx >= 0) {
            unsigned long long w = 0;
            if (idx >= 6) {
                w = letterDigitToNumber(cpy[idx - 6]);  // convert using ASCII
            }

            for (int k = 5; k != 0; k--) {  // length is less than 6

                if (idx >= k) {
                    w = (36 * w) + letterDigitToNumber(cpy[idx - k]);
                }
            }

            if (w != 0) {
                sum[w_idx] = w;
            }
            // produce the h(k)
            hash = hash + sum[w_idx] * rValues[w_idx];
            w_idx -= 1;
            idx -= 6;  // move 6 indices
        }

        return hash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const {
        // Add code here or delete this helper function if you do not want it
        if (isalpha(letter)) {
            return (tolower(letter) - 97);  // letter char -> convert it lower case
        } else {
            return (letter - 22);  // entered a numeric char
        }
    }

    // Code to generate the random R values
    void generateRValues() {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for (int i{0}; i < 5; ++i) {
            rValues[i] = generator();
        }
    }
};

#endif
