#pragma once 

#include <vector>
#include <algorithm>

namespace complexity::core::std {

class impl_stack_allocator_t {
public:
    static impl_stack_allocator_t& get_instance() {
        static impl_stack_allocator_t allocator;
        return allocator;
    }

    template<typename type_t>
    type_t* allocate(size_t _size) {
        auto block = ::std::max<size_t>(sizeof(type_t), alignof(type_t)) * _size;
        if (last_index_ + block >= buffer_.size()) {
            last_index_ = 0;
        }
        auto* ptr = reinterpret_cast<type_t*>(&buffer_[last_index_]);
        last_index_ += block;
        return ptr;
    }

    template<typename type_t, typename... args_t>
    void construct(type_t* _ptr, args_t&&... _args) {
        new(_ptr) type_t(::std::forward<args_t>(_args)...);
    }

    template<typename type_t>
    void destroy(type_t* _ptr) {
        _ptr->~type_t();
    }

    void deallocate(auto* _ptr, size_t _size) {
    }

private:
    impl_stack_allocator_t() : buffer_(k_size) {};

private:
    static constexpr const size_t k_size = 10'000'000;

    ::std::vector<char> buffer_;
    size_t last_index_{0};
};

template<typename type_t>
class stack_allocator_t {
public:
    stack_allocator_t() 
        : allocator_{&impl_stack_allocator_t::get_instance()} {};

    using value_type = type_t;
    using pointer = type_t*;
    using const_pointer = const type_t*;
    using reference = type_t&;
    using const_reference = const type_t&;

    template<class U> 
    struct rebind {
        using other = stack_allocator_t<U>;
    };

    type_t* allocate(size_t _size) {
        return allocator_->allocate<type_t>(_size);
    }

    template<typename... args_t>
    void construct(type_t* _ptr, args_t&&... _args) {
        allocator_->construct<type_t>(_ptr, ::std::forward<args_t>(_args)...);
    }

    void destroy(type_t* _ptr) {
        allocator_->destroy<type_t>(_ptr);
    }

    void deallocate(type_t* _ptr, size_t _size) {
        allocator_->deallocate<type_t>(_ptr, _size);
    }

    bool operator==(const auto& _alloc) const noexcept {
        return true;
    }

private:
    impl_stack_allocator_t* allocator_{nullptr};
};

}