#pragma once 

#include <memory>
#include <iostream>

#include <complexity/core/std/segment_tree.hpp>

namespace complexity::core::std {

template<typename allocator_t = typename ::std::allocator<uint64_t>>
class ordered_set_t: private segment_tree_t<allocator_t> { 
    using node_t = segment_tree_t<allocator_t>::node_t;

public:
    ordered_set_t(uint64_t _size) : segment_tree_t<allocator_t>(_size) {
    }

    void insert(uint64_t _val) {
        this->apply(_val, 1, &(this->root_));
    }

    void erase(uint64_t _val) {
        this->apply(_val, -1, &(this->root_));
    }

    size_t find(uint64_t _id) {
        assert(this->root_.sum >= _id);
        return find(_id, &(this->root_));
    }

private:
    size_t find(uint64_t _id, node_t* _root) {
        if (_root->left == _root->right) {
            return _root->left;
        } else {
            this->init_childs(_root);
            uint64_t mid = this->get_mid(_root);
            if (_root->left_node->sum > _id) {
                return find(_id, _root->left_node);
            } else {
                return find(_id - _root->left_node->sum, _root->right_node);
            }
        }
    }

};

}