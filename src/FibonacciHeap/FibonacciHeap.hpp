//
// Created by runfme on 13.11.18.
//

#include <stdexcept>
#include <memory>
#include <cassert>

template<class T>
class FibonacciHeap {
 private:
  class TreeNode;
  class Pointer;

 public:
  FibonacciHeap() {
    size_ = 0;
    min_root = nullptr;
  }

  bool empty() {
    return size_ == 0;
  }

  unsigned long size() {
    return size_;
  }

  std::shared_ptr<Pointer> insert(T value) {
    auto new_root = TreeNode::create_node(value);
    concatenate_node_lists(min_root, new_root);
    ++size_;

    if (new_root->value < min_root->value) {
      min_root = new_root;
    }

    return new_root->get_pointer();
  }

  T min() {
    if (empty()) {
      throw std::runtime_error("Heap is empty");
    }

    return min_root->value;
  }

  T extract_min() {
    if (empty()) {
      throw std::runtime_error("Heap is empty");
    }

    auto extracted_node = min_root;
    extract_node(min_root);
    --size_;

    if (min_root != nullptr) {
      consolidate();
    }
    find_new_min();

    T return_value = extracted_node->value;
    extracted_node->get_pointer()->valid = false;

    return return_value;
  }

  void remove(std::shared_ptr<Pointer> pointer) {
    if (!pointer->is_valid()) {
      throw std::runtime_error("Invalidated pointer to an element");
    }
    auto node = pointer->node;
    auto parent = node->parent;

    extract_node(node);
    --size_;
    cascading_cut(parent);
    find_new_min();

    pointer->valid = false;
  }

  void decrease_key(std::shared_ptr<Pointer> pointer, T new_value) {
    if (!pointer->is_valid()) {
      throw std::runtime_error("Invalidated pointer to an element");
    }
    if (new_value > pointer->value()) {
      throw std::invalid_argument("Value can only be decreased");
    }

    auto node = pointer->node;
    auto parent = node->parent;

    cut_out_node(node);
    node->value = new_value;
    concatenate_node_lists(min_root, node);

    cascading_cut(parent);
    find_new_min();
  }

  void merge(FibonacciHeap &heap) {
    if (heap.min_root != nullptr) {
      concatenate_node_lists(min_root, heap.min_root);
      size_ += heap.size_;
      find_new_min();

      heap.min_root = nullptr;
      heap.size_ = 0;
    }
  }

  ~FibonacciHeap() {
    recursive_delete(min_root);
  }

 private:
  unsigned long size_;
  std::shared_ptr<TreeNode> min_root;

  void static concatenate_node_lists(std::shared_ptr<TreeNode> &first_list, std::shared_ptr<TreeNode> second_list) {
    if (second_list == nullptr) {
      throw std::invalid_argument("Can not append empty list");
    }

    if (first_list == nullptr) {
      first_list = second_list;
    } else {
      first_list->left_sib->right_sib = second_list;
      second_list->left_sib->right_sib = first_list;

      std::swap(first_list->left_sib, second_list->left_sib);
    }
  }

  void cut_out_node(std::shared_ptr<TreeNode> node) {
    auto parent = node->parent;
    if (node->parent != nullptr) {
      node->parent = nullptr;
    }

    if (node->left_sib == node) {
      if (parent == nullptr) {
        min_root = nullptr;
      } else {
        parent->child = nullptr;
      }
    } else {
      if (parent != nullptr) {
        parent->child = node->right_sib;
      } else if (node == min_root) {
        min_root = node->right_sib;
      }

      node->left_sib->right_sib = node->right_sib;
      node->right_sib->left_sib = node->left_sib;

      node->left_sib = node->right_sib = node;
    }
  }

  unsigned long max_tree_degree() {
    unsigned long highest_bit = 0;
    while ((size_ >> highest_bit) > 0) {
      ++highest_bit;
    }

    return highest_bit;
  }

  void consolidate() {
    if (min_root == nullptr) {
      throw std::runtime_error("Can not consolidate empty heap");
    }

    unsigned long highest_bit = max_tree_degree();
    std::shared_ptr<TreeNode> roots[highest_bit];
    for (unsigned long i = 0; i < highest_bit; ++i) {
      roots[i] = nullptr;
    }

    while (min_root != nullptr) {
      auto cur_root = min_root;
      cut_out_node(min_root);

      while (roots[cur_root->degree] != nullptr) {
        auto second_root = roots[cur_root->degree];
        roots[cur_root->degree] = nullptr;

        cur_root = cur_root->hang(second_root);
      }
      roots[cur_root->degree] = cur_root;
    }

    for (unsigned long j = 0; j < highest_bit; ++j) {
      if (roots[j] != nullptr) {
        concatenate_node_lists(min_root, roots[j]);
        if (roots[j]->value < min_root->value) {
          min_root = roots[j];
        }
      }
    }
  }

  void add_children_as_roots(std::shared_ptr<TreeNode> node) {
    if (node->child != nullptr) {
      auto cur_child = node->child;
      do {
        cur_child->parent = nullptr;
        cur_child->mark = false;
        cur_child = cur_child->right_sib;
      } while (cur_child != node->child);

      concatenate_node_lists(min_root, node->child);
      node->child = nullptr;
    }
  }

  void find_new_min() {
    if (min_root != nullptr) {
      auto cur_root = min_root;
      auto new_min = cur_root;

      do {
        if (cur_root->value < new_min->value) {
          new_min = cur_root;
        }
        cur_root = cur_root->right_sib;
      } while (cur_root != min_root);

      min_root = new_min;
    }
  }

  void extract_node(std::shared_ptr<TreeNode> node) {
    cut_out_node(node);
    add_children_as_roots(node);
  }

  void recursive_delete(std::shared_ptr<TreeNode> node) {
    if (node == nullptr) {
      return;
    }
    recursive_delete(node->child);
    node->left_sib->right_sib = nullptr;
    recursive_delete(node->right_sib);
  }

  void cascading_cut(std::shared_ptr<TreeNode> node) {
    if (node == nullptr) {
      return;
    }
    if (node->parent == nullptr) {
      node->degree -= 1;
    }

    if (node->mark) {
      auto parent = node->parent;

      cut_out_node(node);
      node->mark = false;
      concatenate_node_lists(min_root, node);

      cascading_cut(parent);
    } else {
      node->mark = true;
      node->degree -= 1;
    }
  }

  class TreeNode {
   public:
    friend class FibonacciHeap;
    T value;
    bool mark;
    std::shared_ptr<TreeNode> left_sib;
    std::shared_ptr<TreeNode> right_sib;
    std::shared_ptr<TreeNode> parent;
    std::shared_ptr<TreeNode> child;
    unsigned long degree;

    static std::shared_ptr<TreeNode> create_node(T value) {
      auto new_node = std::shared_ptr<TreeNode>(new TreeNode(value));
      new_node->pointer = std::shared_ptr<Pointer>(new Pointer(new_node));
      new_node->left_sib = new_node->right_sib = new_node;

      return new_node;
    }

    std::shared_ptr<TreeNode> hang(std::shared_ptr<TreeNode> second_tree) {
      if (degree != second_tree->degree) {
        throw std::runtime_error("Trees should have the same degree to hang one to another");
      }

      std::shared_ptr<TreeNode> root_tree = get_pointer()->node;
      if (second_tree->value < root_tree->value) {
        std::swap(root_tree, second_tree);
      }

      concatenate_node_lists(root_tree->child, second_tree);
      second_tree->parent = root_tree;

      return root_tree;
    }

    std::shared_ptr<Pointer> get_pointer() {
      return pointer;
    }

   private:
    std::shared_ptr<Pointer> pointer;

    explicit TreeNode(T value) {
      this->value = value;
      degree = 0;
      mark = false;
      pointer = nullptr;

      parent = nullptr;
      child = nullptr;
    }
  };

  class Pointer {
    friend class FibonacciHeap;
   public:
    explicit Pointer(std::shared_ptr<TreeNode> node) {
      this->node = node;
      this->valid = true;
    }

    bool is_valid() {
      return valid;
    }

    T value() {
      if (!is_valid()) {
        throw std::runtime_error("Invalidated pointer to an element");
      }
      return node->value;
    }
   private:
    std::shared_ptr<TreeNode> node;
    bool valid;
  };

};


