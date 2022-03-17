#ifndef LAB3_SET_H
#define LAB3_SET_H

#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>
#include <cassert>

template<class T, class Cmp = std::less<T>>
class set {
private:
    class node;
    node *m_virtnode;
    int m_size;
    Cmp m_cmp;

public:
    class iterator;
    using size_type = int;

    set() : m_size(0), m_cmp() {
        m_virtnode = (node*)malloc(sizeof(node));
        m_virtnode->parent() = nullptr;
        m_virtnode->left() = nullptr;
        m_virtnode->right() = nullptr;
    }

    set(const set &other);

    set(std::initializer_list<T> &&l): set() {
        for(auto &item : l)
            insert(item);
    }

    iterator lower_bound(T &&elem);
    iterator upper_bound(T &&elem);
    void remove(T elem);
    void insert(T elem);
    void print_by_levels() {
        print_by_levels(m_virtnode->parent(), 0);
    }

    int size() {
        return m_size;
    }

    iterator begin() {
        return iterator(m_virtnode->left(), this);
    }

    iterator end() {
        return iterator(m_virtnode, this);
    }

    T min() {
        return m_virtnode->left()->m_data;
    }

    T max() {
        return m_virtnode->right()->m_data;
    }

    iterator find(T elem) {
        auto e = find(elem, m_virtnode->parent());
        return e ? iterator(e, this) : end();
    }

    void print() {
        print(m_virtnode->parent());
        std::cout << std::endl;
    }

    bool contains(T &elem) {
        return find(elem, m_virtnode->parent()) != nullptr;
    }

    bool operator==(set<T, Cmp> &other) {
        if(&other == this)
            return true;

        return equals(m_virtnode->parent(), other.m_virtnode->parent());
    }

    bool operator!=(set<T, Cmp> &other) {
        return !(*this == other);
    }

    set<T, Cmp>& operator=(const set &other);
    set<T, Cmp>& operator=(set &&other);

    void clear() {
        free(m_virtnode->parent());
        m_virtnode = (node*)malloc(sizeof(node));
        m_virtnode->parent() = nullptr;
        m_virtnode->left() = nullptr;
        m_virtnode->right() = nullptr;
        m_size = 0;
    }

    ~set() {
        if(m_virtnode == nullptr)
            return;

        free(m_virtnode->parent());
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

        iterator(): m_node(nullptr), m_set(nullptr) { }

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

        print(root->left());
        std::cout << root->m_data << " ";
        print(root->right());
    }
    node* find(T &elem, node *root);
    void remove_leaf(node *leaf);
    void remove_general(node *n);
    node* find_rightmost(node *n);
    node* find_leftmost(node *n);
    bool equals(node *n1, node *n2);
    void free(node *root);
    void clone(node *dest, node *src);
    void print_by_levels(node *root, uint8_t depth);

    class node {
    public:
        node *m_parent;
        T m_data;
        node *m_left;
        node *m_right;
        uint8_t m_height;

    public:
        node(T data, node *parent)
            : m_data(data), m_parent(parent), m_left(nullptr), m_right(nullptr), m_height(0)
        { }

        inline node*& right() { return m_right; }
        inline node*& left() { return m_left; }
        inline node*& parent() { return m_parent; }
        inline node*& parent_right() { return m_parent->m_right; }
        inline node*& parent_left() { return m_parent->m_left; }
        inline node*& left_parent() { return m_left->m_parent; }
        inline node*& right_parent() { return m_right->m_parent; }

        node* set_left(node *n, node *&&par) {
            auto l = m_left;
            if(n == nullptr) {
                m_left = n;
                par = nullptr;
                return l;
            }

            par = n->parent();
            m_left = n;
            n->parent() = this;

            return l;
        }

        node* set_right(node *n, node *&&par) {
            auto r = m_right;
            if(n == nullptr) {
                m_right = n;
                par = nullptr;
                return r;
            }

            par = n->parent();
            m_right = n;
            n->parent() = this;

            return r;
        }

        signed char balance_factor() {
            if(m_height == 0)
                return 0;

            signed char hl = left() ? left()->m_height : -1;
            signed char hr = right() ? right()->m_height : -1;

            return hr - hl;
        }

        void fix_height() {
            signed char hl = left() ? left()->m_height : -1;
            signed char hr = right() ? right()->m_height : -1;

            m_height = (hl > hr ? hl : hr) + 1;
        }

        void rotate_right();
        void rotate_left();
        void big_rotate_right();
        void big_rotate_left();
    };
};

template<class T, class Cmp>
void set<T, Cmp>::insert(T elem) {
    if(m_virtnode->parent() == nullptr) {
        m_virtnode->left() = m_virtnode->right() = m_virtnode->parent() = new node(elem, m_virtnode);
    } else {
        insert(elem, m_virtnode->parent());
    }

    ++m_size;
}

template<class T, class Cmp>
void set<T, Cmp>::insert(T elem, set::node *root) {
    if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem)) {
        --m_size;
        return;
    }

    if(m_cmp(elem, root->m_data)) {
        if(root->left() == nullptr) {
            root->left() = new node(elem, root);
            if(root == m_virtnode->left())
                m_virtnode->left() = root->left();

            root->fix_height();
        } else {
            insert(elem, root->left());
            root->fix_height();
            if(root->balance_factor() <= -2) {
                if(root->left()->balance_factor() > 0)
                    root->big_rotate_right();
                else
                    root->rotate_right();
            }
        }
    } else {
        if(root->right() == nullptr) {
            auto new_node = new node(elem, root);
            root->right() = new_node;
            if(elem > m_virtnode->right()->m_data) {
                m_virtnode->right() = new_node;
            }
            root->fix_height();
        } else {
            insert(elem, root->right());
            root->fix_height();
            if(root->balance_factor() >= 2) {
                if(root->right()->balance_factor() < 0)
                    root->big_rotate_left();
                else
                    root->rotate_left();
            }
        }
    }
}

template<class T, class Cmp>
typename set<T, Cmp>::node* set<T, Cmp>::find(T &elem, node *root) {
    if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
        return root;

    if(m_cmp(elem, root->m_data)) {
        if(root->left() == nullptr) {
            return nullptr;
        } else {
            return find(elem, root->left());
        }
    } else {
        if(root->right() == nullptr) {
            return nullptr;
        } else {
            return find(elem, root->right());
        }
    }
}

template<class T, class Cmp>
void set<T, Cmp>::remove(T elem) {
    auto n = find(elem, m_virtnode->parent());
    if(n == nullptr)
        return;

    if(n->left() == nullptr && (n->right() == nullptr || n->right() == m_virtnode)) {
        remove_leaf(n);
        return;
    }

    remove_general(n);
}

template<class T, class Cmp>
void set<T, Cmp>::remove_leaf(set::node *leaf) {
    if(leaf->parent_left() == leaf) {
        leaf->parent_left() = nullptr;

        if(m_virtnode->left() == leaf)
            m_virtnode->left() = leaf->parent();
    } else if(leaf->right() == m_virtnode) {
        leaf->parent_right() = m_virtnode;
        m_virtnode->right() = leaf->parent();
    } else {
        leaf->parent_right() = nullptr;
    }

    delete leaf;
}

template<class T, class Cmp>
void set<T, Cmp>::remove_general(set::node *n) {
    node *replacement;

    if(n->left() != nullptr)
        replacement = find_rightmost(n->left());
    else
        replacement = find_leftmost(n->right());

    std::swap(n->m_data, replacement->m_data);
    if(replacement->left() != nullptr) {
        replacement->parent_right() = replacement->left();
    } else if(replacement->right() == m_virtnode) {
        replacement->parent_right() = m_virtnode;
        m_virtnode->right() = replacement->parent();
    } else if(replacement->right() != nullptr) {
        replacement->parent_left() = replacement->right();
    } else {
        remove_leaf(replacement);
        return;
    }

    delete replacement;
}

template<class T, class Cmp>
typename set<T, Cmp>::node *set<T, Cmp>::find_rightmost(set::node *n) {
    while(n->right() != nullptr)
        n = n->right();

    return n;
}

template<class T, class Cmp>
typename set<T, Cmp>::node *set<T, Cmp>::find_leftmost(set::node *n) {
    while(n->left() != nullptr)
        n = n->left();

    return n;
}

template<class T, class Cmp>
void set<T, Cmp>::print_by_levels(node *root, uint8_t depth) {
    if(root == nullptr)
        return;

    print_by_levels(root->right(), depth + 1);
    std::cout << " ";
    for(int i = 0; i < depth; ++i)
        std::cout << "    ";
    std::cout << root->m_data << "(" << (int)root->m_height << ")" << std::endl;
    print_by_levels(root->left(), depth + 1);
}

template<class T, class Cmp>
bool set<T, Cmp>::equals(set::node *n1, set::node *n2) {
    if(n1 == nullptr && n2 == nullptr)
        return true;

    if(n1 == m_virtnode && n2->right_parent() != n2)
        return true;

    if(n1 == nullptr || n2 == nullptr)
        return false;

    if(!m_cmp(n1->m_data, n2->m_data) && !m_cmp(n2->m_data, n1->m_data)) {
        return equals(n1->left(), n2->left()) && equals(n1->right(), n2->right());
    } else {
        return false;
    }
}

template<class T, class Cmp>
typename set<T, Cmp>::iterator set<T, Cmp>::upper_bound(T &&elem) {
    auto root = m_virtnode->parent();
    if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
        return ++iterator(root, this);

    if(m_cmp(elem, root->m_data)) {
        while(root != nullptr) {
            while (elem < root->m_data) {
                if(root->left() == nullptr)
                    return iterator(root, this);

                root = root->left();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return ++iterator(root, this);

            while(!m_cmp(elem, root->m_data)) {
                if(root->right() == nullptr)
                    return iterator(root, this);

                root = root->right();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return ++iterator(root, this);
        }
    } else {
        while(root != nullptr) {
            while (!m_cmp(elem, root->m_data)) {
                if (root->right() == m_virtnode)
                    return end();

                if (root->right() == nullptr)
                    return iterator(root, this);

                root = root->right();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return ++iterator(root, this);

            while (m_cmp(elem, root->m_data)) {
                if (root->left() == nullptr)
                    return iterator(root, this);

                root = root->left();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return ++iterator(root, this);
        }
    }
}

template<class T, class Cmp>
typename set<T, Cmp>::iterator set<T, Cmp>::lower_bound(T &&elem) {
    auto root = m_virtnode->parent();
    if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
        return iterator(root, this);

    if(m_cmp(elem, root->m_data)) {
        while(root != nullptr) {
            while (m_cmp(elem, root->m_data)) {
                if(root->left() == nullptr)
                    return iterator(root, this);

                root = root->left();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return iterator(root, this);


            while(m_cmp(root->m_data, elem)) {
                if(root->right() == nullptr)
                    return iterator(root, this);

                root = root->right();
            }

            if(!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return iterator(root, this);
        }
    } else {
        while(root != nullptr) {
            while (m_cmp(root->m_data, elem)) {
                if (root->right() == m_virtnode)
                    return end();

                if (root->right() == nullptr)
                    return iterator(root, this);

                root = root->right();
            }

            if (!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return iterator(root, this);

            while (m_cmp(elem, root->m_data)) {
                if (root->left() == nullptr)
                    return iterator(root, this);

                root = root->left();
            }

            if (!m_cmp(elem, root->m_data) && !m_cmp(root->m_data, elem))
                return iterator(root, this);
        }
    }
}

template<class T, class Cmp>
void set<T, Cmp>::free(set::node *root) {
    if(root == nullptr)
        return;

    if(root->left() != nullptr)
        free(root->left());

    if(root->right() == m_virtnode) {
        delete m_virtnode;
        root->right() = nullptr;
    }

    if(root->right() != nullptr)
        free(root->right());

    delete root;
}

template<class T, class Cmp>
set<T, Cmp>::set(const set &s): m_size(s.m_size) {
    m_cmp = Cmp();
    m_virtnode = (node*) malloc(sizeof(node));
    if(s.m_virtnode->parent() == nullptr) {
        m_virtnode->parent() = m_virtnode->left() = m_virtnode->right() = nullptr;
        return;
    }

    m_virtnode->parent() = new node(s.m_virtnode->parent()->m_data, nullptr);
    m_virtnode->left() = m_virtnode->right() = m_virtnode->parent();
    clone(m_virtnode->parent(), s.m_virtnode->parent());
}

template<class T, class Cmp>
void set<T, Cmp>::clone(node *dest, set::node *src) {
    if(src->left() != nullptr) {
        dest->left() = new node(src->left()->m_data, dest);
        clone(dest->left(), src->left());
    } else if(dest->m_data < m_virtnode->left()->m_data) {
        m_virtnode->left() = dest;
    }

    if(src->right() != nullptr) {
        if(src->right_parent() != src) {
            dest->right() = m_virtnode;
            m_virtnode->right() = dest;
        } else {
            dest->right() = new node(src->right()->m_data, dest);
            clone(dest->right(), src->right());
        }
    }
}

template<class T, class Cmp>
set<T, Cmp> &set<T, Cmp>::operator=(const set &other) {
    if(this == &other)
        return *this;

    set<T, Cmp> s(other);
    std::swap(s.m_virtnode->left(), this->m_virtnode->left());
    std::swap(s.m_virtnode->right(), this->m_virtnode->right());
    std::swap(s.m_virtnode->parent(), this->m_virtnode->parent());
    m_size = s.m_size;

    return *this;
}

template<class T, class Cmp>
set<T, Cmp> &set<T, Cmp>::operator=(set &&other) {
    m_virtnode = other.m_virtnode;
    m_size = other.m_size;

    other.m_virtnode = nullptr;
    other.m_size = 0;

    return *this;
}

template<class T, class Cmp>
typename set<T, Cmp>::iterator set<T, Cmp>::iterator::operator++() {
    assert(*this != m_set->end());
    if(m_node == m_set->m_virtnode->right()) {
        m_node = m_set->m_virtnode;
        return *this;
    }

    if(m_node->right() != nullptr) {
        m_node = m_set->find_leftmost(m_node->right());
        return *this;
    }

    auto par = m_node->parent();
    while(par->left() != m_node) {
        if(par->parent()->parent() == par) {
            par = par->right();
            break;
        }

        m_node = par;
        par = par->parent();
    }

    m_node = par;
    return *this;
}

template<class T, class Cmp>
typename set<T, Cmp>::iterator set<T, Cmp>::iterator::operator--() {
    assert(*this != m_set->begin());
    if(m_node == m_set->m_virtnode) {
        m_node = m_node->right();
        return *this;
    }

    if(m_node->left() != nullptr) {
        m_node = m_set->find_rightmost(m_node->left());
        return *this;
    }

    auto par = m_node->parent();
    while(par->right() != m_node) {
        if(par->parent()->parent() == par) {
            par = par->left();
            break;
        }

        par = par->parent();
    }

    m_node = par;
    return *this;
}

template<class T, class Cmp>
void set<T, Cmp>::node::rotate_right() {
    auto q = left();

    if(parent()->parent() == this) {
        parent()->parent() = q;
        q->parent() = parent();
    } else {
        if(parent_right() == this)
            parent()->set_right(q, nullptr);
        else
            parent()->set_left(q, nullptr);
    }

    auto lr = q->set_right(this, nullptr);
    set_left(lr, nullptr);

    fix_height();
    q->fix_height();
}

template<class T, class Cmp>
void set<T, Cmp>::node::rotate_left() {
    auto r = right();

    if(parent()->parent() == this) {
        parent()->parent() = r;
        r->parent() = parent();
    } else {
        if(parent_right() == this)
            parent()->set_right(r, nullptr);
        else
            parent()->set_left(r, nullptr);
    }

    auto rl = r->set_left(this, nullptr);
    set_right(rl, nullptr);

    fix_height();
    r->fix_height();
}

template<class T, class Cmp>
void set<T, Cmp>::node::big_rotate_right() {
    left()->rotate_left();
    rotate_right();
}

template<class T, class Cmp>
void set<T, Cmp>::node::big_rotate_left() {
    right()->rotate_right();
    rotate_left();
}

#endif //LAB3_SET_H
