// Copyright

#ifndef SRC_KDTREE_HPP_
#define SRC_KDTREE_HPP_

#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include "Point.hpp"

template <size_t N, typename ElemType>
class KDTree {
public:
    typedef std::pair<Point<N>, ElemType> value_type;

    KDTree();

    ~KDTree();

    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);

    size_t dimension() const; //

    size_t size() const;      //
    bool empty() const;       //

    bool contains(const Point<N>& pt) const;                //

    void insert(const Point<N>& pt, const ElemType& value); //

    ElemType& operator[](const Point<N>& pt);

    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;

    ElemType knn_value(const Point<N>& key, size_t k) const;

    std::vector<ElemType> knn_query(const Point<N>& key, size_t k) const;

private:

    //          Estructura auxiliar
    struct KDTreeNode {
        KDTreeNode(const Point<N>& punto, const ElemType& value) { p = punto; v = value; }
        Point<N> p;
        ElemType v;
        KDTreeNode* h[2] = { nullptr,nullptr };
    };

    KDTreeNode* root;      // Raiz auxiliar
    size_t dimension_;
    size_t size_;

    //          Funciones auxiliares
    bool find(KDTreeNode**&, const Point<N>&);
    bool find(KDTreeNode**&, const Point<N>&)const;
    void preOrderInsert(KDTreeNode*);
    void posOrderDelete(KDTreeNode*);
    bool comparePair(std::pair<KDTreeNode*, double>, std::pair<KDTreeNode*, double>);
    void distancias(KDTreeNode*, std::vector<std::pair<KDTreeNode*, double>>&, const Point<N>&)const;
};

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::find(KDTreeNode**& p, const Point<N>& x) {
    int iter = 0;
    for (p = &root; *p && (*p)->p != x; iter = (++iter) % N) {
        p = &((*p)->h[x[iter] > (*p)->p[iter]]);
    }
    return *p != 0;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::find(KDTreeNode**& p, const Point<N>& x) const {
    int iter = 0;
    p = const_cast<KDTreeNode**>(&root);
    for (; *p && (*p)->p != x; iter = (++iter) % N) {
        p = &((*p)->h[x[iter] > (*p)->p[iter]]);
    }
    return *p != 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::preOrderInsert(KDTreeNode* n) {
    if (!n) return;
    insert(n->p, n->v);
    preOrderInsert(n->h[0]);
    preOrderInsert(n->h[1]);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::posOrderDelete(KDTreeNode* n) {
    if (!n) return;
    posOrderDelete(n->h[0]);
    posOrderDelete(n->h[1]);
    delete n;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    dimension_ = N;
    size_ = 0;
    root = 0;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    posOrderDelete(root);
    root = 0; dimension_ = 0; size_ = 0;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree& rhs) {
    preOrderInsert(rhs.root);
    size_ = rhs.size_;
    dimension_ = rhs.dimension_;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& rhs) {
    posOrderDelete(root); root = 0;
    preOrderInsert(rhs.root);
    size_ = rhs.size_;
    dimension_ = rhs.dimension_;
    return *this;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return dimension_;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return !size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    KDTreeNode** p;
    return find(p, pt);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
    KDTreeNode** p;
    if (find(p, pt)) {
        (*p)->v = value;
        return;
    };
    (*p) = new KDTreeNode(pt, value);
    size_++;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt) {
    KDTreeNode** p;
    if (!find(p, pt)) {
        (*p) = new KDTreeNode(pt, 0);
        size_++;
    }
    return (*p)->v;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) {
    KDTreeNode** p;
    if (!find(p, pt)) throw std::out_of_range("Fuera de rango");
    else return (*p)->v;
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const {
    KDTreeNode** p;
    if (!find(p, pt)) throw std::out_of_range("Fuera de rango");
    else return (*p)->v;
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::knn_value(const Point<N>& key, size_t k) const {
    std::vector<ElemType> values = knn_query(key, k);
    std::vector<std::pair<ElemType, int>> contador;

    bool flag = false;
    int max = -1;
    ElemType valor = 0;
    for (auto i : values) {
        for (int j = 0; j < contador.size(); j++)
            if (contador[j].first == i) {
                flag = true;
                if (++contador[j].second > max) {
                    valor = contador[j].first;
                    max = contador[j].second;
                }
            }
        if (!flag) {
            contador.push_back(std::pair<ElemType, int>(i, 1));
            if (1 > max) {
                valor = i;
                max = 1;
            }
        }
        else flag = false;
    }

    return valor;
}
template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::comparePair(std::pair<KDTreeNode*, double> i1, std::pair<KDTreeNode*, double> i2) {
    return (i1.second < i2.second);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::distancias(KDTreeNode* n,std::vector<std::pair<KDTreeNode*, double>>& dist, const Point<N>& key) const{
    if (!n) return;
    std::pair<KDTreeNode*, double> aux; aux.first = n; aux.second = distance(key,n->p);
    dist.push_back(aux);
    distancias(n->h[0],dist,key);
    distancias(n->h[1],dist,key);
}

template <size_t N, typename ElemType>
std::vector<ElemType> KDTree<N, ElemType>::knn_query(const Point<N>& key, size_t k) const {

    std::vector<ElemType> values;
    std::vector<std::pair<KDTreeNode*, double>> dist;
    std::vector<std::pair<KDTreeNode*, double>> dist_aux;
    KDTreeNode** p = const_cast<KDTreeNode**>(&root);
    if (k > size_)k = size_;
    //Find
    /*
    int iter = 0;
    double min_aux = INT_FAST32_MAX;
    for (; *p ; iter = (++iter) % N) {
        std::pair<KDTreeNode*, double> aux; aux.first = *p; aux.second = distance(key, (*p)->p);
        if (min_aux > aux.second) min_aux = aux.second;
        dist_aux.push_back(aux);
        p = &((*p)->h[key[iter] > (*p)->p[iter]]);
    }
    */
    distancias(*p, dist, key);
    double min = INT_FAST32_MAX;
    int index = 0;
    for (int i = 0; i < size_ && i < k; i++) {
        for (int j = 0; j < dist.size(); j++) {
            if (dist[j].second < min) {
                min = dist[j].second;
                index = j;
            }
        }
        values.push_back(dist[index].first->v);
        dist.erase(dist.begin() + index);
        min = INT_FAST32_MAX;
    }
    return values;
}

#endif  // SRC_KDTREE_HPP_