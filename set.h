#ifndef LAB3_SET_H
#define LAB3_SET_H

#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>

template<class T>
class set {
private:
    class node;
    node *m_root;

public:
    set() {
        m_root = nullptr;
    }

    void insert(T elem);
    void remove(T elem);
    T find_min() {
        return find_leftmost(m_root);
    }

    T find_max() {
        return find_rightmost(m_root);
    }

    node *find(T elem) {
        return find(elem, m_root);
    }

    void print() {
        print(m_root);
        std::cout << std::endl;
    }

    void find_prev_and_next(node *elem, node *&prev, node *&next) {
        prev = elem->m_left == nullptr ? nullptr : find_rightmost(elem->m_left);
        next = elem->m_right == nullptr ? nullptr : find_leftmost(elem->m_right);
    }

    void print_by_levels();

    bool operator==(set<T> &other) {
        return equals(m_root, other.m_root);
    }

private:
    void insert(T elem, node *root);
    void print(node *root) {
        if(root == nullptr)
            return;

        print(root->m_left);
        std::cout << root->m_data << " ";
        print(root->m_right);
    }
    node *find(T &elem, node *root);
    void remove_leaf(node *leaf);
    void remove_general(node *n);
    node* find_rightmost(node *n);
    node *find_leftmost(node *n);
    bool equals(node *n1, node *n2);

    class node {
    public:
        node *m_parent;
        T m_data;
        node *m_left;
        node *m_right;

    public:
        node(T data, node *parent)
            : m_data(data), m_parent(parent), m_left(nullptr), m_right(nullptr)
        { }
    };
};

template<class T>
void set<T>::insert(T elem) {
    if(m_root == nullptr)
        m_root = new node(elem, nullptr);
    else
        insert(elem, m_root);
}

template<class T>
void set<T>::insert(T elem, set::node *root) {
    if(elem == root->m_data)
        return;

    if(elem < root->m_data) {
        if(root->m_left == nullptr) {
            root->m_left = new node(elem, root);
        } else {
            insert(elem, root->m_left);
        }
    } else {
        if(root->m_right == nullptr) {
            root->m_right = new node(elem, root);
        } else {
            insert(elem, root->m_right);
        }
    }
}

template<class T>
typename set<T>::node* set<T>::find(T &elem, node *root) {
    if(elem == root->m_data)
        return root;

    if(elem < root->m_data) {
        if(root->m_left == nullptr) {
            return nullptr;
        } else {
            return find(elem, root->m_left);
        }
    } else {
        if(root->m_right == nullptr) {
            return nullptr;
        } else {
            return find(elem, root->m_right);
        }
    }
}

template<class T>
void set<T>::remove(T elem) {
    auto n = find(elem);
    if(n == nullptr)
        return;

    if(n->m_left == nullptr && n->m_right == nullptr) {
        remove_leaf(n);
        return;
    }

    remove_general(n);
}

template<class T>
void set<T>::remove_leaf(set::node *leaf) {
    if(leaf->m_parent->m_left == leaf) {
        leaf->m_parent->m_left = nullptr;
    } else {
        leaf->m_parent->m_right = nullptr;
    }

    delete leaf;
}

template<class T>
void set<T>::remove_general(set::node *n) {
    node *replacement;

    if(n->m_left != nullptr)
        replacement = find_rightmost(n->m_left);
    else
        replacement = find_leftmost(n->m_right);

    std::swap(n->m_data, replacement->m_data);
    if(replacement->m_left != nullptr) {
        replacement->m_parent->m_right = replacement->m_left;
    } else if(replacement->m_right != nullptr) {
        replacement->m_parent->m_left = replacement->m_right;
    } else {
        remove_leaf(replacement);
        return;
    }

    delete replacement;
}

template<class T>
typename set<T>::node *set<T>::find_rightmost(set::node *n) {
    while(n->m_right != nullptr)
        n = n->m_right;

    return n;
}

template<class T>
typename set<T>::node *set<T>::find_leftmost(set::node *n) {
    while(n->m_left != nullptr)
        n = n->m_left;

    return n;
}

template<class T>
void set<T>::print_by_levels() {
    std::queue<node*> q;
    q.push(m_root);
    while(!q.empty()) {
        auto n = q.front();
        q.pop();
        std::cout << n->m_data << " ";
        if(n->m_left != nullptr)
            q.push(n->m_left);
        if(n->m_right != nullptr)
            q.push(n->m_right);
    }
    std::cout << std::endl;
}

template<class T>
bool set<T>::equals(set::node *n1, set::node *n2) {
    if(n1 == nullptr && n2 == nullptr)
        return true;

    if(n1 == nullptr || n2 == nullptr)
        return false;

    if(n1->m_data == n2->m_data) {
        return equals(n1->m_left, n2->m_left) && equals(n1->m_right, n2->m_right);
    } else {
        return false;
    }
}

#endif //LAB3_SET_H
