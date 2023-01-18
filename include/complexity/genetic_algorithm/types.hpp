#pragma once 

#include <complexity/core/std/stack_allocator.hpp>

namespace complexity::genetic_algorithm {

using complexity::core::std::stack_allocator_t;

class gen_t {
public:
    gen_t(size_t _count) : gen_(_count) {}
 
    gen_t(const gen_t&) = default;
    gen_t& operator=(const gen_t&) = default;
    gen_t(gen_t&&) = default;
    gen_t& operator=(gen_t&&) = default;


    uint32_t& operator[](size_t i) {
        return gen_[i];
    }

    const uint32_t& operator[](size_t i) const {
        return gen_[i];
    }

    uint32_t& cost() {
        return cost_;
    }

    const uint32_t& cost() const {
        return cost_;
    }

    void reset_cost() {
        cost_ = std::numeric_limits<uint32_t>::max();
    }

    size_t size() const {
        return gen_.size();
    }

    auto& raw_data() {
        return gen_;
    }

private:
    std::vector<uint32_t, stack_allocator_t<uint32_t>> gen_;
    uint32_t cost_{std::numeric_limits<uint32_t>::max()};
};

using batch_t = std::vector<gen_t>;

template<typename callback_t>
concept optimizer_t = requires(batch_t _batch, callback_t _callback) {
    _callback(_batch);
};

template<typename callback_t>
concept evaluator_t = requires(batch_t _batch, callback_t _callback) {
    _callback(_batch);
};

}