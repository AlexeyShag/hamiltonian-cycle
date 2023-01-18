#include <complexity/generator.hpp>

#include <complexity/core/std/ordered_set.hpp>

#include <complexity/generator.hpp>
#include <complexity/genetic_algorithm/engine.hpp>
#include <complexity/solutions/step2.hpp>

using namespace complexity;
using complexity::core::std::ordered_set_t;

auto find_cycle(size_t vertex_count) {
    size_t m = vertex_count * (vertex_count + 1) / 2;
    auto graph = create_graph(vertex_count, std::max<size_t>(vertex_count + 1, m * 0.3));

    genetic_algorithm::engine_t engine;

    auto result = engine.fit(
        solution2::default_optimizer_t(50, 50),
        solution2::default_evaluator_t(graph),
        vertex_count,
        vertex_count,
        vertex_count * (vertex_count + 1) / 2,
        100000
    );

    return result.it_count;
}

size_t evaluate_iterations_number(size_t sample_size, size_t vertex_count) {
    std::vector<size_t> sample;
    for (size_t i = 0; i < sample_size; ++i) {
        sample.push_back(find_cycle(vertex_count));
    }
    std::sort(sample.begin(), sample.end());
    return sample[static_cast<size_t>(sample.size() * 0.5)];
}

signed main() {

    std::cout << "vertex count: 10, iteration_count: " << evaluate_iterations_number(300, 10) << '\n';
    std::cout << "vertex count: 20, iteration_count: " << evaluate_iterations_number(300, 20) << '\n';
    std::cout << "vertex count: 30, iteration_count: " << evaluate_iterations_number(300, 30) << '\n';
    std::cout << "vertex count: 40, iteration_count: " << evaluate_iterations_number(300, 40) << '\n';

    return 0;
}