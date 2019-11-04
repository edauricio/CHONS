#include "boost/assert.hpp"
#include <iostream>

namespace boost {
    void assertion_failed_msg(char const* expr, char const* msg,
        char const* function, char const* file, long line) {
        std::cout << "ASSERTION FAILED!\n";
        std::cout << "\"" << msg << "\"\n";
        std::cout << "failed: (" << expr << ")\n";
        std::cout << "in: " << file << ":" << line << ": " << function << "\n";
        std::abort();
    }
}