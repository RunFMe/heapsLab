//
// Created by runfme on 13.11.18.
//

#include <stdexcept>
#include <memory>
template <class T>
class BinomialHeap {
  class Pointer;

  class BinomialTreeNode {
   public:
    T value;
    BinomialTreeNode* parent;
    BinomialTreeNode* sibling;
    BinomialTreeNode* child;
    std::shared_ptr<Pointer> pointer;

    explicit BinomialTreeNode(T value) {
      this->value = value;
      this->parent = nullptr;
      this->sibling = nullptr;
      this->child = nullptr;
      this->degree = 0;
      this->pointer = std::shared_ptr<Pointer> (new Pointer(this));
    }

    BinomialTreeNode* hang(BinomialTreeNode* second_tree) {
      if (this->degree != second_tree->degree) {
        throw std::runtime_error ("Can not hang trees with different degrees");
      }

      BinomialTreeNode* new_root = this;
      BinomialTreeNode* sub_tree = second_tree;
      if (sub_tree->value < new_root->value) {
        std::swap(new_root, sub_tree);
      }
      new_root->add_child(sub_tree);

      return new_root;
    }

    unsigned long get_degree() {
      return degree;
    }

    void reset_degree() {
      degree = 0;
    }

   private:
    unsigned long degree;

    void add_child(BinomialTreeNode* sub_tree) {
      sub_tree->sibling = this->child;
      sub_tree->parent = this;
      this->child = sub_tree;
      this->degree += 1;
    }
  };

  class Pointer{
    friend class BinomialHeap<T>;
   public:
    explicit Pointer(BinomialTreeNode* node) {
      this->node = node;
      this->valid = true;
    }

    bool is_valid() {
      return valid;
    }

    T value() {
      return node->value;
    }
   private:
    BinomialTreeNode* node;
    bool valid{};
  };


 public:
  std::shared_ptr<Pointer> insert(T value) {
    auto new_node = new BinomialTreeNode(value);
    add_roots(new_node);
    ++size_;

    return new_node->pointer;
  }

  unsigned long size() {
    return size_;
  }

  bool empty() {
    return size_ == 0;
  }

  T min() {
    if (empty()) {
      throw std::runtime_error ("No minimal element. Heap is empty.");
    }
    return min_root->value;
  }

  T extract_min() {
    if (empty()) {
      throw std::runtime_error ("No minimal element. Heap is empty.");
    }
    auto removed_root = extract_min_root();

    T return_value = removed_root->value;
    delete removed_root;

    return return_value;
  }

  void change(std::shared_ptr<Pointer> pointer, T new_value) {
    if (!pointer->is_valid()) {
      throw std::runtime_error ("Invalidated pointer to an element");
    }

    if (new_value <= pointer->value()) {
      pointer->node->value = new_value;
      sift_up(pointer->node);
    } else {
      extract_value(pointer->node);
      //node is outside of heap so we can change its value
      pointer->node->value = new_value;
      add_roots(pointer->node);
      ++size_;
    }
  }

  void remove(std::shared_ptr<Pointer> pointer) {
    if (!pointer->is_valid()) {
      throw std::runtime_error ("Invalidated pointer to an element");
    }
    auto node = pointer->node;
    extract_value(node);

    delete pointer->node;
    pointer->valid = false;
  }

  void merge(BinomialHeap& second_heap) {
    if (second_heap.size() == 0) {
      return;
    }
    add_roots(second_heap.minimal_tree);

    if (min_root == nullptr ||
        second_heap.min_root != nullptr && second_heap.min_root->value < min_root->value) {
      min_root = second_heap.min_root;
    }
    size_ += second_heap.size_;

    second_heap.min_root = second_heap.minimal_tree = nullptr;
    second_heap.size_ = 0;
  }

  ~BinomialHeap() {
    recursive_delete(minimal_tree);
  }

 private:
  unsigned long size_ = 0;
  BinomialTreeNode* minimal_tree;
  BinomialTreeNode* min_root;

  void add_roots(BinomialTreeNode* second_min_root) {
    BinomialTreeNode* prev_root = nullptr;
    BinomialTreeNode* current_root = minimal_tree;
    BinomialTreeNode* free_root = second_min_root;
    second_min_root = second_min_root->sibling;

    while (free_root != nullptr) {
      if (second_min_root != nullptr && free_root->get_degree() == second_min_root->get_degree()) {
        //try to merge free_tree with tree from second heap
        auto new_second_root = second_min_root->sibling;
        free_root = free_root->hang(second_min_root);

        second_min_root = new_second_root;
      } else {
        //try to insert free_tree into this heap
        if (current_root == nullptr || free_root->get_degree() < current_root->get_degree()) {
          if (prev_root == nullptr) {
            minimal_tree = free_root;
          } else {
            prev_root->sibling = free_root;
          }
          free_root->sibling = current_root;
          update_min_if_needed(free_root);

          current_root = free_root;
          free_root = second_min_root;
          if (second_min_root != nullptr) {
            second_min_root = second_min_root->sibling;
          }
        } else if (free_root->get_degree() > current_root->get_degree()) {
          prev_root = current_root;
          current_root = current_root->sibling;
        } else {
          auto new_current_root = current_root->sibling;
          cut_out_root(prev_root, current_root);
          free_root = free_root->hang(current_root);

          current_root = new_current_root;
        }
      }
    }
  }

  void cut_out_root(BinomialTreeNode* prev_root, BinomialTreeNode* current_root) {
    if (prev_root != nullptr && prev_root->sibling != current_root) {
      throw std::runtime_error ("Roots are not subsequent");
    }

    if (prev_root != nullptr) {
      prev_root->sibling = current_root->sibling;
    } else {
      minimal_tree = current_root->sibling;
    }
    current_root->sibling = nullptr;
  }

  void update_min_if_needed(BinomialTreeNode* root) {
    if (min_root == nullptr || root->value <= min_root->value) {
      min_root = root;
    }
  }

  void find_new_min() {
    for (auto root = minimal_tree;  root != nullptr ; root = root->sibling) {
      update_min_if_needed(root);
    }
  }

  BinomialTreeNode* reverse_siblings(BinomialTreeNode* first, bool recursive_call=false) {
    if (first->sibling == nullptr) {
      return first;
    } else {
      BinomialTreeNode* new_first = reverse_siblings(first->sibling);
      first->sibling->sibling = first;

      //we need to manually set first node's sibling to null
      if (!recursive_call) {
        first->sibling = nullptr;
      }

      return new_first;
    }
  }

  void swap_values(BinomialTreeNode* first, BinomialTreeNode* second) {
    std::swap(first->value, second->value);
    std::swap(first->pointer->node, second->pointer->node);
    std::swap(first->pointer, second->pointer);
  }

  void sift_up(BinomialTreeNode* node, bool rise_to_top=false) {
    while (node->parent != nullptr && (node->value < node->parent->value || rise_to_top)) {
      swap_values(node, node->parent);
      node = node->parent;
    }

    if (rise_to_top || node->value < min_root->value) {
      min_root = node;
    }
  }

  BinomialTreeNode* extract_min_root() {
    BinomialTreeNode* root_before_min = nullptr;
    if (minimal_tree != min_root) {
      //if min_root is first then there is no root_before_min to find
      root_before_min = minimal_tree;
      while (root_before_min->sibling != min_root) {
        root_before_min = root_before_min->sibling;
      }
    }

    cut_out_root(root_before_min, min_root);
    --size_;
    auto removed_root = min_root;
    min_root = nullptr;

    if (removed_root->child != nullptr) {
      //to use add_roots we need to reverse order of children of min_root
      auto minimal_tree_sibling = reverse_siblings(removed_root->child);

      //get rid of parent-child relationship
      removed_root->child = nullptr;
      removed_root->reset_degree();
      auto current_node = minimal_tree_sibling;
      while (current_node != nullptr) {
        current_node->parent = nullptr;
        current_node = current_node->sibling;
      }

      add_roots(minimal_tree_sibling);
    }
    find_new_min();

    return removed_root;
  }

  void extract_value(BinomialTreeNode* node) {
    //pretend we changed value in required node to minus infinity,
    //sift it to the to and extract
    sift_up(node, true);
    extract_min_root();
  }

  void recursive_delete(BinomialTreeNode* node) {
    if (node == nullptr) {
      return;
    }
    recursive_delete(node->sibling);
    recursive_delete(node->child);

    delete node;
  }
};


