#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include <stdlib.h>

#define MAX_KEY 100000

using namespace std;

template<class N>
struct lista {

    lista();
    bool Insert(int,N);
    bool Delete(int);
    void Print();

private:
    struct nodo {
        nodo(int key_, N val_) :key(key_), val(val_), next(0) {}
        nodo* next;
        mutex candado;
        N val;
        int key;
    };
    bool search(int, nodo**&);
    void weak_search(int, nodo*&, nodo*&);
    void strong_search(int, nodo*&, nodo*&);
    nodo* begin, *end;
};
template<class N>
lista<N>::lista() {
    begin = new nodo(-1,0);
    end = new nodo(MAX_KEY,0);
    begin->next = end;
}
template<class N>
bool lista<N>::Insert(int key, N val) {
    nodo* x, * y, * aux;
    strong_search(key, x, y);
    if (y->key == key) 
        y->val = val;
    else
    {
        aux = new nodo(key, val);
        x->next = aux;
        aux->next = y;
    }
    x->candado.unlock();
    return 1;
}
template<class N>
bool lista<N>::Delete(int key) {
    nodo* x, * y;
    strong_search(key, x, y);
    if (y->key == key) {
        y->candado.lock();
        x->next = y->next;
        y->next = x;
        y->candado.unlock();
        delete y;
    }
    x->candado.unlock();
    return 1;
}
template<class N>
bool lista<N>::search(int key, nodo**& y) {
    for (y = &begin; *y && (*y)->key < key; (y) = &((*y)->next));
    return *y && (*y)->key == key;
}
template<class N>
void lista<N>::weak_search(int key, nodo*& x, nodo*& y) {
    x = begin;
    y = x->next;
    while (y->key < key) {
        x = y;
        y = x->next;
    }
}
template<class N>
void lista<N>::strong_search(int key, nodo*& x, nodo*& y) {
    x = begin;
    y = x->next;
    while (y->key < key) {
        x = y;
        y = x->next;
    }
    x->candado.lock();
    while (y->key < key) {
        x->candado.unlock();
        x = y;
        x->candado.lock();
        y = x->next;
    }
}
template<class N>
void lista<N>::Print() {
    nodo** y;
    for (y = &begin; *y && (*y)->key < MAX_KEY; (y) = &((*y)->next)) {
        cout << (*y)->key << "->";
    }
}


int get_random(int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(low, high);
    return distribution(gen);
}
struct Add {
    int min_, max_;
    lista<int>* ptr_;
    Add(int min, int max, lista<int>* ptr) :min_(min), max_(max), ptr_(ptr) {}
    void operator()(int operaciones) {
        for (int i = 0; i < operaciones; i++) {
            ptr_->Insert(get_random(min_, max_), get_random(min_, max_));
        }
    }
};
struct Delete {
    int min_, max_;
    lista<int>* ptr_;
    Delete(int min, int max, lista<int>* ptr) :min_(min), max_(max), ptr_(ptr) {}
    void operator()(int operaciones) {
        for (int i = 0; i < operaciones; i++) {
            ptr_->Delete(get_random(min_, max_));
        }
    }
};

int main() {
    lista<int> a;
    thread first(Add(1, 99999, &a), 30000);
    thread segundo(Delete(1, 99999, &a), 30000);

    first.join();
    segundo.join();
    a.Print();
    return 0;
}