#include <vector>
#include <iostream>
#include <math.h>

using namespace std;
//----------------------------- Nodo ------------------------------//
#define valores vector<N>
#define Nodo KDNode<N>
#define template1 template<class N>
#define push(a,b,c) a.push_back(b); a.push_back(c)
#define f(i,ini,fin) for(int i = ini; i < fin; i++)

template1
struct KDNode {

    valores value;
    KDNode* nodes[2];

    KDNode(valores);
    ~KDNode() = default;

};

template1
KDNode<N>::KDNode(valores pt) {
    value = pt;
    nodes[0] = nodes[1] = 0;
}
//------------------------------Tree-------------------------------//

template1
struct KDTree {

    Nodo* root;
    int eje = 0;
    int dim;

    KDTree(int dim_, int eje_ = 0) {
        root = 0;
        dim = dim_;
        eje = eje_;
    }
    Nodo** Rep(Nodo**);
    bool Buscar(valores, Nodo**&);
    bool Insert(valores);
    bool Borrar(valores);

    bool to_array(Nodo*,vector<valores>&);
    bool NN_core(valores, valores&,vector<valores>);
    bool NN(valores, valores&);
    bool KNN(valores,int,vector<valores>&);
    bool Range_query(valores, int, vector<valores>&);
};
template1
bool KDTree<N>::Buscar(valores x, Nodo** &p){
    int i = eje;

    // Se inicia con root y eje en dim 0
    // Se para si es vacio o hoja, o si es igual al valor
    // Se itera p hacia sus hijos dependiendo de la dimension, eje itera en 1

    for (p = &root; *p && (*p)->value != x; i = (i + 1) % dim)
        p = &((*p)->nodes[x[i] > (*p)->value[i]]);
    return *p != 0;

}
template1
Nodo** KDTree<N>::Rep(Nodo** p) {
    bool b = rand() % 2;
    for (p = &((*p)->nodes[b]); (*p)->nodes[!b]; p = &((*p)->nodes[!b]));
    return p;
}
template1
bool KDTree<N>::Insert(valores x) {
    Nodo** p;
    if (Buscar(x, p)) return 0;
    *p = new Nodo(x);
    return 1;
}
template1
bool KDTree<N>::Borrar(valores x) {
    Nodo** p;
    if (!Buscar(x, p)) return 0;
    // CBinNode** q;

    if ((*p)->nodes[0]) 
        if ((*p)->nodes[1]) {
            Nodo** q = Rep(p);
            (*p)->value = (*q)->value;
            p = q;
        }

    Nodo* temp = *p;                 //*q=*p;
    *p = (*p)->nodes[!(*p)->nodes[0]];

    delete temp;        //delete *q;
    return 1;
}
template1
bool KDTree<N>::NN_core(valores x,valores &ptr, vector<valores> Puntos) {
    
    Nodo** p;
    if (!Buscar(x, p)) return 0;
    if ((**p).value != x) return 0;

    double aux, min = UINT16_MAX;
    valores y;

    f(i, 0, Puntos.size()) {
        aux = 0;
        f(j, 0, dim)
            aux += pow((x[j] - Puntos[i][j]), 2);

        aux = sqrt(aux);
        if (aux < min && aux != 0) {
            min = aux;
            ptr = Puntos[i];
        }
    }
    
    return 1;

}
template1
bool KDTree<N>::to_array(Nodo* n, vector<valores>& ptr) {

    if (!n) return 0;
    to_array(n->nodes[0],ptr);
    ptr.push_back(n->value);
    to_array(n->nodes[1],ptr);

}
template1
bool KDTree<N>::NN(valores x, valores& ptr) {
    vector<valores> Array;
    to_array(root, Array);
    return NN_core(x, ptr, Array);
}
template1
int find_vector(valores x,vector<valores> lista) {
    valores aux;
    int indice = 0;
    while (aux != x) {
        aux = lista[indice];
        indice++;
    }
    return indice - 1;
}
template1
bool KDTree<N>::KNN(valores x, int tam, vector<valores>& ptr) {
    valores aux;
    vector<valores> Array;
    to_array(root, Array);

    f(i, 0, tam) {
        if (NN_core(x, aux, Array)) {
            ptr.push_back(aux);
            Array.erase(Array.begin()+find_vector<N>(aux,Array));
        }
        else return 0;
    }
    return 1;
}
template1
bool KDTree<N>::Range_query(valores x, int rango, vector<valores>& ptr) {
    valores aux;
    double auxint;
    bool parada = true;
    vector<valores> Array;
    to_array(root, Array);

    while(parada){
        if (NN_core(x, aux, Array)) {

            auxint = 0;
            f(j, 0, dim)
                auxint += pow((x[j] - aux[j]), 2);
            auxint = sqrt(auxint);

            if (auxint > rango) return 1;
        
            ptr.push_back(aux);
            Array.erase(Array.begin() + find_vector<N>(aux, Array));
        }
    }
    return 1;
}
int main(){

    // Se ingresan datos de la forma:
    // - Puntos Dimensionalidad
    // - x , y, ...

    int t, d, x;
    KDTree<int> Arbol(2);
    vector<int> a;
    
    // Se agregan de manera automatica al arból

    for (cin >> t >> d; t--; a.clear()) {
        f(i, 0, d) {
            cin >> x;
            a.push_back(x);
        }
        Arbol.Insert(a);
    }
    
    //Probando las funciones Borrar y NN
 
    vector<int> b;
    vector<int> c;
    vector<vector<int>> e;

    cout << "Borrar" << endl;
    f(i, 0, d) {
        cin >> x;
        b.push_back(x);
    }
    Arbol.Borrar(b);
    cout << endl;
    b.clear();

    cout << "NN" << endl;
    f(i, 0, d) {
        cin >> x;
        b.push_back(x);
    }
    Arbol.NN(b, c);
    f(i, 0, d) cout << c[i] << " ";
    cout << endl;
    b.clear();
    c.clear();

    cout << "KNN" << endl;
    cin >> t;
    f(i, 0, d) {
        cin >> x;
        b.push_back(x);
    }
    Arbol.KNN(b, t, e);
    f(j, 0, e.size()) {
        f(i, 0, d) cout << e[j][i] << " ";
        cout << endl;
    }
    e.clear();
    b.clear();

    cout << "Range_Queary" << endl;
    cin >> t;
    f(i, 0, d) {
        cin >> x;
        b.push_back(x);
    }
    Arbol.Range_query(b, t, e);
    f(j, 0, e.size()) {
        f(i, 0, d) cout << e[j][i] << " ";
        cout << endl;
    }
    b.clear();
    e.clear();

    return 0;
}