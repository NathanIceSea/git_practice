#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__ 1

#include <cassert>
#include <stack>

template <typename T>
void deleteCurr(T* curr) { if (curr != nullptr) delete curr; }

template <typename T>
class binary_tree {
public:
    typedef binary_tree<T> *tree_node;
private:
    T *elem;
    tree_node left;
    tree_node right;
    tree_node parent;
public:
    binary_tree(): elem(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
    explicit binary_tree(T *elem);
    // 返回插入位置的节点
    binary_tree<T>* insert(T *elem);
    void preOrderedTraverse(void (*consumer)(T *e));
    void inOrderedTraverse(void (*consumer)(T *e));
    void postOrderedTraverse(void (*consumer)(T *e));
    void preOrderedLoop(void (*consumer)(T *e));
    void inOrderedLoop(void (*consumer)(T *e));
    void postOrderedLoop(void (*consumer)(T *e));
    ~binary_tree()
    {
        if (this->left != nullptr)
            delete this->left;
        if (this->right != nullptr)
            delete this->right;
        delete elem;
    }
};


// 模板实现
template <typename T>
binary_tree<T>::binary_tree(T *elem):elem(elem) {
    left = nullptr; right = nullptr;
    parent = nullptr;
}

template <typename T>
binary_tree<T>* binary_tree<T>::insert(T *elem) {
    tree_node curr_pos = this;
    while(true) {
        if (curr_pos->elem == nullptr) {
            curr_pos->elem = elem;
            return curr_pos;
        }
        if (*elem < *(curr_pos->elem))
            if (curr_pos->left != nullptr) {
                curr_pos = curr_pos->left;
                continue;
            } else {
                curr_pos->left = new binary_tree(elem);
                curr_pos->left->parent = curr_pos;
                return curr_pos->left;
            }
        else if (*elem > *(curr_pos->elem))
            if (curr_pos->right != nullptr) {
                curr_pos = curr_pos->right;
                continue;
            } else {
                curr_pos->right = new binary_tree(elem);
                curr_pos->right->parent = curr_pos;
                return curr_pos->right;
            }
        else
            // 相同元素，不支持的操作，强制退出
            assert(false);
    }
    return curr_pos;
}


template <typename T>
void binary_tree<T>::preOrderedTraverse(void (*consumer)(T *e)) {
    consumer(elem);
    if (this->left != nullptr)
        this->left->preOrderedTraverse(consumer);
    if (this->right != nullptr)
        this->right->preOrderedTraverse(consumer);
}

template <typename T>
void binary_tree<T>::inOrderedTraverse(void (*consumer)(T *e)) {
    if (this->left != nullptr)
        this->left->inOrderedTraverse(consumer);
    consumer(elem);
    if (this->right != nullptr)
        this->right->inOrderedTraverse(consumer);
}

template <typename T>
void binary_tree<T>::postOrderedTraverse(void (*consumer)(T *e)) {
    if (this->left != nullptr)
        this->left->postOrderedTraverse(consumer);
    if (this->right != nullptr)
        this->right->postOrderedTraverse(consumer);
    consumer(elem);
}

template <typename T>
void binary_tree<T>::preOrderedLoop(void (*consumer)(T *e)) {
    std::stack<tree_node> stack;
    tree_node curr_pos = this;
    while(true) {
        // 对于每一个非空当前节点，遍历节点，然后将右子节点（如果非空）推入栈中，置当前节点为左子节点
        if (curr_pos != nullptr) {
            consumer(curr_pos->elem);
            if (curr_pos->right != nullptr)
                stack.push(curr_pos->right);
            curr_pos = curr_pos->left;
        } else {
            // 当前节点为空，从栈中弹出节点用于处理
            if (stack.empty())
                return;
            curr_pos = stack.top();
            stack.pop();
        }
    }
}

template <typename T>
void binary_tree<T>::inOrderedLoop(void (*consumer)(T *e)) {
    std::stack<tree_node> stack;
    tree_node curr_pos = this;
    while(true) {
        // 1、检测例程
        if (curr_pos == nullptr) {
            // 3、栈例程
            if (stack.empty())
                return;
            // 弹出上次的当前节点，同时找出右子节点
            tree_node curr_node = stack.top(), right_node = curr_node->right;
            stack.pop();
            consumer(curr_node->elem);
            // 若右节点不为空，置当前节点为右节点，continue
            if (right_node != nullptr) {
                curr_pos = right_node;
                continue;
            }
        // 2、处理例程
        } else {
            // 将当前节点推入栈中，它将被栈例程处理；如果有左子节点，则置当前节点为左子节点
            stack.push(curr_pos);
            if (curr_pos->left != nullptr)
                // 当前节点的左子节点不为空，进入下一层
                curr_pos = curr_pos->left;
            else
                // 当前节点的左子节点为空，没有下一层，置当前节点为空，准备进入栈例程
                curr_pos = nullptr;
        }
    }
}

template <typename T>
void binary_tree<T>::postOrderedLoop(void (*consumer)(T *e)) {
    std::stack<tree_node> stack;
    tree_node curr_pos = this;
    while(true) {
        // 1、检测例程
        if (curr_pos == nullptr) {
            // 3、栈例程（每次处理一对节点）
            if (stack.empty())
                return;
            // 弹出上次的右子节点，读取上次的当前节点
            tree_node right_node = stack.top();
            stack.pop();
            tree_node curr_node = stack.top();
            // 若右节点不为空，需要沿着右节点方向处理，将当前节点置为右节点，然后把 NULL 压栈（从而下次右节点为 NULL），continue
            if (right_node != nullptr) {
                stack.push(nullptr);
                curr_pos = right_node;
                continue;
            // 右节点为空，遍历上次的当前节点，弹出节点，continue
            } else {
                consumer(curr_node->elem);
                stack.pop();
                continue;
            }
        // 2、处理例程
        } else {
            // 将当前节点和右子节点推入栈中，它将被栈例程处理；如果有左子节点，则置当前节点为左子节点
            stack.push(curr_pos);
            stack.push(curr_pos->right);    // 右子节点可能为 NULL
            if (curr_pos->left != nullptr)
                // 当前节点的左子节点不为空，进入下一层
                curr_pos = curr_pos->left;
            else
                // 当前节点的左子节点为空，没有下一层，置当前节点为空，准备进入栈例程
                curr_pos = nullptr;
        }
    }
}

#endif