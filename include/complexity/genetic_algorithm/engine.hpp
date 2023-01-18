#pragma once 

#include <complexity/genetic_algorithm/types.hpp>
#include <complexity/core/std/random.hpp>

namespace complexity::genetic_algorithm {

using complexity::core::std::random;

class engine_t {
    struct result_t {
        gen_t gen;
        size_t it_count{0};
    };

public:
    auto fit(
        optimizer_t _optimizer,
        evaluator_t _evaluator,
        size_t _gen_size = 10,
        size_t _batch_size = 10,
        size_t _expected_cost = 100,
        size_t _max_iteration = 10) 
    {
        auto batch = get_random_gens(_batch_size, _gen_size);
        _evaluator(batch);

        for (size_t it = 0; it < _max_iteration; ++it) {
            _optimizer(batch);
            _evaluator(batch);
            while (batch.size() > _batch_size) {
                batch.pop_back();
            }
            if (batch[0].cost() >= _expected_cost) {
                return result_t {
                    .gen = batch[0],
                    .it_count = it
                };
            }
        }

        return result_t {
            .gen = batch[0],
            .it_count = _max_iteration
        };
    }

private:
    batch_t get_random_gens(size_t _batch_size, size_t _gen_size) {
        batch_t gens(_batch_size, gen_t(_gen_size));
        for (size_t i = 0; i < _batch_size; ++i) {
            for (size_t j = 0; j < _gen_size; ++j) {
                gens[i][j] = random::get_int() % (_gen_size - j);
            }
        }
        return gens;
    }
};

}