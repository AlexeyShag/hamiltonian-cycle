#pragma once 

#include <complexity/core/std/ordered_set.hpp>
#include <complexity/core/std/stack_allocator.hpp>
#include <complexity/core/std/random.hpp>
#include <complexity/types.hpp>

namespace complexity::solution1
{

using complexity::core::std::ordered_set_t;
using complexity::core::std::stack_allocator_t;
using complexity::core::std::random;

class default_optimizer_t {
public:
    default_optimizer_t(size_t _mutation_prob = 40,
                        size_t _cross_prob = 40,
                        size_t _seed = 42) 
                        : mutation_prob_(_mutation_prob)
                        , cross_prob_(_cross_prob) {}

    void operator()(auto& _batch) {
        auto batch_size = _batch.size();
        auto gen_size = _batch[0].size();

        auto mid = (batch_size + 1) / 2;
        auto n = _batch.size();
        for (size_t i = 0; i < n; ++i) {
            size_t magic_number = random::get_int() % 100;

            if (magic_number < mutation_prob_) {
                size_t id = random::get_int() % gen_size;
                size_t new_g = random::get_int() % (gen_size - id);

                _batch[i][id] = new_g;
                _batch[i].reset_cost();

            } else if (magic_number < mutation_prob_ + cross_prob_) {
                size_t parent1_id = random::get_int() % mid;
                size_t parent2_id = random::get_int() % mid;

                size_t split_id = random::get_int() % gen_size;

                _batch.push_back(_batch.back());
                _batch.push_back(_batch.back());
                auto last = _batch.end();
                auto& gen1 = *(--last);
                gen1.reset_cost();
                auto& gen2 = *(--(--last));
                gen2.reset_cost();

                for (size_t j = 0; j < split_id; ++j) {
                    gen1[j] = _batch[parent2_id][j];
                    gen2[j] = _batch[parent1_id][j];
                }

                for (size_t j = split_id; j < gen_size; ++j) {
                    gen2[j] = _batch[parent2_id][j];
                    gen1[j] = _batch[parent1_id][j];
                }

            }
        }
    }

private:
    size_t mutation_prob_{40};
    size_t cross_prob_{40};
};

class default_evaluator_t {
public:
    default_evaluator_t(const graph_t& _graph) : graph_(_graph) {}

    void operator()(auto& _batch) {
        auto batch_size = _batch.size();
        auto gen_size = _batch[0].size();

        for (auto& gen: _batch) {
            if (gen.cost() != std::numeric_limits<uint32_t>::max()) {
                continue;
            }
            gen.cost() = 0;
            ordered_set_t<stack_allocator_t<uint64_t>> set(gen_size); 
            for (size_t i = 0; i < gen_size; ++i) {
                set.insert(i);
            }

            size_t start_id = set.find(gen[0]);
            set.erase(start_id);
            size_t current_id = start_id;

            for (size_t i = 1; i < gen_size; ++i) {
                size_t id = set.find(gen[i]);
                set.erase(id);
                gen.cost() += graph_.contains(current_id, id);
                current_id = id;
            }
            gen.cost() += graph_.contains(current_id, start_id);
        }

        std::sort(_batch.begin(), _batch.end(), [](auto& a, auto& b){
            return a.cost() > b.cost();
        });
    }

private:
    const graph_t& graph_;
};

}