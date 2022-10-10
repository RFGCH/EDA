#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

#define PI 3.141592

struct pnt {
    pnt(vector<double> valores) {
        v = valores;
        for (int i = 0; i < valores.size(); i++) {
            magnitud += valores[i] * valores[i];
        }
        magnitud = sqrt(magnitud);
    }
    vector<double> v;
    double magnitud = 0;
};
double producto_escalar(pnt a, pnt b) {
    double sum = 0;
    for (int i = 0; i < a.v.size(); i++) {
        sum += a.v[i] * b.v[i];
    }
    return sum;
}
template<class I>
I pitagoras(vector<I> a, vector<I> b) {
    I sum = 0;
    for (I i = 0; ++i < a.size(); sum += (a[i] - b[i]));
    return sqrt(sum);
}
pnt modulo(pnt a, pnt b) {
    vector<double> aux2;
    for (int i = 0; i < a.v.size(); i++) {
        aux2.push_back(a.v[i] - b.v[i]);
    }
    return pnt(aux2);
}
double angulo(vector<pnt> puntos, pnt key) {
    double angulo = 0;
    for (int i = 0; i < puntos.size(); i++) {
        pnt modulo1(modulo(key,puntos[i]));
        pnt modulo2(modulo(key,puntos[(i + 1) % (puntos.size() - 1)]));
        cout <<  (producto_escalar(modulo1, modulo2) /
            (modulo1.magnitud * modulo2.magnitud))
        << endl;
    }
    return angulo;
}
bool arriba_abajo(pnt a, pnt b, pnt key) {
    double pendiente = (b.v[1] - a.v[1]) / (b.v[0] - a.v[0]);
    double independiente = a.v[1] - a.v[0] * pendiente;
    double aux = (key.v[0] * pendiente) + independiente;
    if (aux <= key.v[1])  return true;
    return false;
}
bool adentro_afuera(vector<pnt> rectas, pnt key) {
    int sum = 0;
    for (int i = 0; i < rectas.size(); i++) {
        if (key.v == rectas[i].v)return true; 
        sum += arriba_abajo(rectas[i], rectas[(i + 1) % (rectas.size())], key);
    }
    if (sum == rectas.size()) return false;
    return sum % 2;
}
int main() {

    vector<pnt> rectas;
    vector<double> aux1 = { 7, 8 };
    rectas.push_back(pnt(aux1));
    vector<double> aux2 = { 6, 6 };
    rectas.push_back(pnt(aux2));
    vector<double> aux3 = { 8, 6 };
    rectas.push_back(pnt(aux3));

    vector<double> aux4 = { 6.0001,6 };
    pnt key(aux4);
    
    cout << adentro_afuera(rectas, key);


    return 0;
}