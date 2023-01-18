#pragma once 

#include <vector>
#include <cassert>
#include <algorithm>

namespace complexity::core::std {

class random {
public:
    static auto& get_generator() {
        return rnd;
    }

    static size_t get_int() {
        return rnd();
    }

private:
    static ::std::mt19937 rnd;
};

::std::mt19937 random::rnd(time(0));

}