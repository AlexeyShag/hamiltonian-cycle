#pragma once 

#include <memory>

namespace complexity::core::std {

template<typename allocator_type>
class segment_tree_t { 
protected:
    struct node_t {
        node_t(int64_t _sum = 0, uint64_t _left = 0, uint64_t _right = 0) 
                : sum(_sum), left(_left), right(_right) {}

        int64_t sum = 0;
        uint64_t left = 0;
        uint64_t right = 0;
        node_t* left_node = nullptr;
        node_t* right_node = nullptr;
    };

public:
    segment_tree_t(uint64_t _size) 
        : size_(_size), root_(0, 0, _size - 1) {
    }

    void apply(uint64_t _pos, int64_t _delta) {
        apply(_pos, _delta, &root_);
    }

    int64_t get_sum(uint64_t _left, uint64_t _right) {
        return get_sum(_left, _right, &root_);
    }

    ~segment_tree_t() {
        destroy(&root_);
    }

protected:
    uint64_t get_mid(node_t* _node) {
        return _node->left + ((_node->right - _node->left) >> 1);
    }

    void init_childs(node_t* _node) {
        uint64_t mid = get_mid(_node);
        if (_node->right_node == nullptr) {
            _node->right_node = allocator_.allocate(1);
            allocator_.construct(_node->right_node, 0, mid + 1, _node->right);
        }

        if (_node->left_node == nullptr) {
            _node->left_node = allocator_.allocate(1);
            allocator_.construct(_node->left_node, 0, _node->left, mid);
        }
    }

    void apply(uint64_t _pos, int64_t _delta, node_t* _root) {
        if (_root->left == _root->right) {
            _root->sum += _delta;
        } else {
            init_childs(_root);
            uint64_t mid = get_mid(_root);
            if (_pos <= mid) {
                apply(_pos, _delta, _root->left_node);
            } else {
                apply(_pos, _delta, _root->right_node);
            }
            _root->sum = _root->left_node->sum + _root->right_node->sum;
        }
    }

    int64_t get_sum(uint64_t _left, uint64_t _right, node_t* _root) {
        if (_root->left == _left && _root->right == _right) {
            return _root->sum;
        } else {
            init_childs(_root);
            uint64_t mid = get_mid(_root);
            if (_right <= mid) {
                return get_sum(_left, _right, _root->left_node);
            } else if (mid < _left) {
                return get_sum(_left, _right, _root->right_node);
            } else {
                return get_sum(_left, mid, _root->left_node) +
                       get_sum(mid + 1, _right, _root->right_node);
            }
        }
    }

    void destroy(node_t* _node) {
        if (_node->right_node) {
            destroy(_node->right_node);
            allocator_.destroy(_node->right_node);
            allocator_.deallocate(_node->right_node, 1);
        }
        if (_node->left_node) {
            destroy(_node->left_node);
            allocator_.destroy(_node->left_node);
            allocator_.deallocate(_node->left_node, 1);
        }
    }

protected:
    node_t root_;
    size_t size_{1};
    using allocator_t = typename allocator_type::template rebind<node_t>::other;
    allocator_t allocator_;
};

}