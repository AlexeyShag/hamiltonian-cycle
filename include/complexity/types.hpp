#pragma once 

#include <vector>
#include <set>

namespace complexity 
{

class small_graph_t {
public:
    small_graph_t(size_t _n) : matrix_(_n, std::vector<bool>(_n, 0)) {}

    void add_edge(size_t v, size_t u) {
        matrix_[v][u] = matrix_[u][v] = 1;
    }

    void del_edge(size_t v, size_t u) {
        matrix_[v][u] = matrix_[u][v] = 0;
    }

    bool contains(size_t v, size_t u) const {
        return matrix_[v][u];
    }

private:
    std::vector<std::vector<bool>> matrix_;
};

using graph_t = small_graph_t;

}