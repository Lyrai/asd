#ifndef LAB3_SET_H
#define LAB3_SET_H

#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>
#include <cassert>

template<class T>
class set {
private:
    class node;
    node *m_virtnode;

public:
    class iterator;

    set() {
        m_virtnode = (node*)malloc(sizeof(node));
        m_virtnode->m_parent = nullptr;
        m_virtnode->m_left = nullptr;
        m_virtnode->m_right = nullptr;
    }

    iterator lower_bound(T &elem);
    iterator upper_bound(T &elem);
    void remove(T elem);
    void insert(T elem);
    void print_by_levels();

    iterator begin() {
        return iterator(m_virtnode->m_left, this);
    }

    iterator end() {
        return iterator(m_virtnode, this);
    }

    T min() {
        return m_virtnode->m_left->m_data;
    }

    T max() {
        return m_virtnode->m_right->m_data;
    }

    iterator find(T elem) {
        auto e = find(elem, m_virtnode->m_parent);
        return e ? iterator(e, this) : end();
    }

    void print() {
        print(m_virtnode->m_parent);
        std::cout << std::endl;
    }

    bool contains(T &elem) {
        return find(elem, m_virtnode->m_parent) != nullptr;
    }

    bool operator==(set<T> &other) {
        if(&other == this)
            return true;

        return equals(m_virtnode->m_parent, other.m_virtnode->m_parent);
    }

    bool operator!=(set<T> &other) {
        return !(*this == other);
    }

public:
    class iterator: public std::iterator<std::bidirectional_iterator_tag, T> {
    private:
        node *m_node;
        set *m_set;

    public:
        iterator(node *n, set *s)
            : m_node(n), m_set(s)
        { }

        iterator(iterator &iter)
            : m_node(iter.m_node), m_set(iter.m_set)
        { }

        iterator(const iterator &iter)
            : m_node(iter.m_node), m_set(iter.m_set)
        { }

        iterator operator++();
        iterator operator--();
        T operator*() const {
            assert(m_node != m_set->m_virtnode);
            return m_node->m_data;
        }

        bool operator==(const iterator &rhs) const {
            return m_node == rhs.m_node;
        }

        bool operator!=(const iterator &rhs) const {
            return m_node != rhs.m_node;
        }
    };

private:
    void insert(T elem, node *root);
    void print(node *root) {
        if(root == m_virtnode || root == nullptr)
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
    if(m_virtnode->m_parent == nullptr) {
        m_virtnode->m_left = m_virtnode->m_right = m_virtnode->m_parent = new node(elem, nullptr);
        m_virtnode->m_parent->m_right = m_virtnode;
    } else {
        insert(elem, m_virtnode->m_parent);
    }
}

template<class T>
void set<T>::insert(T elem, set::node *root) {
    if(elem == root->m_data)
        return;

    if(elem < root->m_data) {
        if(root->m_left == nullptr) {
            root->m_left = new node(elem, root);
            if(root == m_virtnode->m_left)
                m_virtnode->m_left = root->m_left;
        } else {
            insert(elem, root->m_left);
        }
    } else {
        if(root->m_right == m_virtnode) {
            auto new_node = new node(elem, root);
            root->m_right = new_node;
            m_virtnode->m_right = new_node;
            new_node->m_right = m_virtnode;
        } else if(root->m_right == nullptr) {
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
    auto n = find(elem, m_virtnode->m_parent);
    if(n == nullptr)
        return;

    if(n->m_left == nullptr && (n->m_right == nullptr || n->m_right == m_virtnode)) {
        remove_leaf(n);
        return;
    }

    remove_general(n);
}

template<class T>
void set<T>::remove_leaf(set::node *leaf) {
    if(leaf->m_parent->m_left == leaf) {
        leaf->m_parent->m_left = nullptr;

        if(m_virtnode->m_left == leaf)
            m_virtnode->m_left = leaf->m_parent;
    } else if(leaf->m_right == m_virtnode) {
        leaf->m_parent->m_right = m_virtnode;
        m_virtnode->m_right = leaf->m_parent;
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
    } else if(replacement->m_right == m_virtnode) {
        replacement->m_parent->m_right = m_virtnode;
        m_virtnode->m_right = replacement->m_parent;
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
    q.push(m_virtnode->m_parent);
    while(!q.empty()) {
        auto n = q.front();
        q.pop();
        std::cout << n->m_data << " ";
        if(n->m_left != nullptr)
            q.push(n->m_left);
        if(n->m_right != nullptr && n->m_right != m_virtnode)
            q.push(n->m_right);
    }
    std::cout << std::endl;
}

template<class T>
bool set<T>::equals(set::node *n1, set::node *n2) {
    if(n1 == nullptr && n2 == nullptr)
        return true;

    if(n1 == m_virtnode && n2->m_right->m_parent != n2)
        return true;

    if(n1 == nullptr || n2 == nullptr)
        return false;

    if(n1->m_data == n2->m_data) {
        return equals(n1->m_left, n2->m_left) && equals(n1->m_right, n2->m_right);
    } else {
        return false;
    }
}

template<class T>
typename set<T>::iterator set<T>::upper_bound(T &elem) {
    auto iter = begin();
    while(iter != end() && *iter <= elem)
        ++iter;

    return iter;
}

template<class T>
typename set<T>::iterator set<T>::lower_bound(T &elem) {
    auto iter = begin();
    while(iter != end() && *iter < elem)
        ++iter;

    return iter;
}

template<class T>
typename set<T>::iterator set<T>::iterator::operator++() {
    assert(*this != m_set->end());
    if(m_node->m_right == m_set->m_virtnode) {
        m_node = m_node->m_right;
        return *this;
    }

    if(m_node->m_right != nullptr) {
        m_node = m_set->find_leftmost(m_node->m_right);
        return *this;
    }

    auto par = m_node->m_parent;
    while(par->m_left != m_node) {
        if(par->m_parent == nullptr) {
            par = par->m_right;
            break;
        }

        m_node = par;
        par = par->m_parent;
    }

    m_node = par;
    return *this;
}

template<class T>
typename set<T>::iterator set<T>::iterator::operator--() {
    assert(*this != m_set->begin());
    if(m_node == m_set->m_virtnode) {
        m_node = m_node->m_right;
        return *this;
    }

    if(m_node->m_left != nullptr) {
        m_node = m_set->find_rightmost(m_node->m_left);
        return *this;
    }

    auto par = m_node->m_parent;
    while(par->m_right != m_node) {
        if(par->m_parent == nullptr) {
            par = par->m_left;
            break;
        }

        par = par->m_parent;
    }

    m_node = par;
    return *this;
}

#endif //LAB3_SET_H
