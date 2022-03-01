#ifndef LAB3_SET_H
#define LAB3_SET_H

#include <cstdlib>

template<class T>
class set {
private:
    class node;

public:
    set() {
        m_root = (node*) malloc(sizeof(node));
        m_root->m_parent = m_root->m_left = m_root->m_right = nullptr;
    }

    void insert(T elem);
    void remove(T elem);
    void find(T &elem);

private:
    node *m_root;

private:
    class node {
    public:
        node *m_parent;
        T m_data;
        node *m_left;
        node *m_right;

    public:
        node(T data, node *parent, node *left, node *right)
            : m_data(data), m_parent(parent), m_left(left), m_right(right)
        { }
    };
};

template<class T>
void set<T>::insert(T elem) {
    if(elem < m_root->m_data) {
        if(m_root->m_left == nullptr)
            m_root->m_left = new node(elem, m_root, nullptr, nullptr);
    }
}


#endif //LAB3_SET_H
