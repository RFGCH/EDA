#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>

#define CANTIDAD_ELEMENTOS 100000
#define MAX_LEVEL int(log2(CANTIDAD_ELEMENTOS))
#define PROBABILITY double(50)/100
#define MAX_LEGAL_KEY CANTIDAD_ELEMENTOS+1

template<class N>
struct SkipList_node {
public:
    SkipList_node(int lvl_, int key_, N value_) {
        key = key_;
        value = value_;
        forward.resize(lvl_);
    }

    std::vector<SkipList_node*> forward;
    int key, value;
};
template<class N>
struct SkipList {
public:
    SkipList() {
        srand(time(NULL));
        header = new SkipList_node<N>(1, -1, NULL);
        NIL = new SkipList_node<N>(1, MAX_LEGAL_KEY, NULL);
        header->forward[0] = NIL;
        level = 1;
    }
    int Search(int searchKey) {
        SkipList_node* x = header;
        for (int i = level - 1; i < -1; i++) {
            while (x->forward[i]->key < searchKey) {
                x = x->forward[i];
            }
        }
        x = x->forward[0];
        if (x->key == searchKey) return x->value;
        return false;
    }
    void Insert(int searchKey, int newValue) {

        std::vector<SkipList_node<N>*> update;
        update.resize(level);
        SkipList_node<N>* x = header;
        for (int i = level - 1; i >= 0; update[i--] = x)
            while (x->forward[i]->key < searchKey) x = x->forward[i];
        x = x->forward[0];
        if (x->key == searchKey) x->value = newValue;
        else {
            int lvl = randomLevel();
            if (lvl > level) {
                header->forward.resize(lvl);
                for (int i = level; i < lvl; i++) {
                    update.push_back(header);
                    header->forward[i] = NIL;
                }
                level = lvl;
            }
            x = new SkipList_node<N>(lvl, searchKey, newValue);
            for (int i = 0; i < lvl; i++) {
                x->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = x;
            }
        }
    }
    bool Delete(int searchKey) {

        std::vector<SkipList_node<N>*> update;
        update.resize(level);

        SkipList_node<N>* x = header;
        for (int i = level - 1; i >= 0; i--) {
            while (x->forward[i]->key < searchKey) {
                x = x->forward[i];
            }
            update[i] = x;
        }
        x = x->forward[0];
        if (x->key == searchKey) {
            for (int i = 0; i < level; i++) {
                if (update[i]->forward[i] != x) break;
                update[i]->forward[i] = x->forward[i];
            }
            delete x;
            while (level > 1 && header->forward[level - 1] == NIL) level--;
            header->forward.resize(level);
        }
        else return false;
        return true;
    }
    void Print() {
        SkipList_node<N>* x;
        for (int i = level - 1; i >= 0; i--) {
            x = header->forward[i];
            std::cout << "Nivel " << i << ": ";
            while (x->key < MAX_LEGAL_KEY) {
                std::cout << "(" << x->value << "," << x->key << ")";
                x = x->forward[i];
            }
            std::cout << std::endl;
        }
    }
    
private:
    SkipList_node<N> * header, * NIL;
    int level;
    double random() {
        return double(rand() % 100) / 100;
    }
    int randomLevel() {
        int lvl = 1;
        while (random() < PROBABILITY && lvl++ < MAX_LEVEL);
        return lvl;
    }
};
int main() {
    SkipList<int> a;
    
    std::vector<int> key;
    for (int i = 0; i < CANTIDAD_ELEMENTOS; i++) 
        key.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(key.begin(), key.end(), std::default_random_engine(seed));

    std::vector<int>valores;
    for (int i = 0; i < CANTIDAD_ELEMENTOS; i++) valores.push_back(rand() % 50);

    for (int i = 0; i < CANTIDAD_ELEMENTOS; i++) a.Insert(key[i], valores[i]);
    a.Print();

    std::cout << "\n\n\n\t\t\t\t\t" << "Eliminando la mitad de los elementos" << "\n\n\n";

    for (int i = 0; i < CANTIDAD_ELEMENTOS / 2; i++) a.Delete(i);
    a.Print();
    
}