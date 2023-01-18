#pragma once 

#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>

#include <complexity/types.hpp>

#include <complexity/core/std/random.hpp>

namespace complexity 
{

using complexity::core::std::random;

graph_t create_graph(size_t _n, size_t _m) {
    assert(_m >= _n && _n * (_n - 1) / 2 >= _m);
    _m -= _n;
    graph_t graph(_n);

    std::vector<size_t> cyrcle(_n);
    for (size_t i = 0; i < _n; ++i) {
        cyrcle[i] = i;
    }
    std::shuffle(cyrcle.begin(), cyrcle.end(), random::get_generator());
    for (size_t i = 0; i < _n; ++i) {
        graph.add_edge(cyrcle[i], cyrcle[(i + 1) % _n]);
    }

    while (_m-- > 0) {
        size_t a = random::get_int() % _n;
        size_t b = random::get_int() % _n;
        if (!graph.contains(a, b) && a != b) {
            graph.add_edge(a, b);
        } else {
            ++_m;
        }
    }

    return graph;
}

}