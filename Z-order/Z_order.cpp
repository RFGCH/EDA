#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bitset>
#include <math.h>
#include <queue>

// Abreviaciones
#define hastafor(i,fin) for(int i = 0; i < fin; i++)    // For de 0 hasta fin
#define vector_short vector<unsigned short>             // Vector que guarda enteros short sin signo
#define bitini bitset<bit_ini>                         // Bit que se usara para los datos multidimensionales convertidos
#define bitfin bitset<bit_fin>                         // Bit que se usara para los datos del z-order

using namespace std;

// Datos globales
int
cant_datos = 50000,
cant_dim = 10,
int_max = 8;
int const
bit_ini = 3,
bit_fin = 32;
int const iteraciones = 4;
int cant_vecinos[iteraciones] = { 5,10,15,20 };

//      ---- Estructura para gestionar los indices y las distancias ----


// Estructura nodo para guardar las distancias y los indices
struct nodo {
    nodo(int dato_, int index_) { dato = dato_, index = index_; }
    int dato, index;
};

// Operador para comparar nodos en la cola de prioridad
bool compare(const nodo& a, const nodo& b) {
    return a.dato < b.dato;
}


// Funcion para rellenar x datos de n dimensiones
vector<vector_short> rellenar(int datos, int dimensiones) {

    vector_short dato_uni_dim;
    vector<vector_short> dato_final;
    srand(time(NULL));

    hastafor(i, datos) {
        hastafor(j, dimensiones)
            dato_uni_dim.push_back(rand() % int_max);
        dato_final.push_back(dato_uni_dim);
        dato_uni_dim.clear();
    }

    return dato_final;

}


//      ---- Funciones para hallar el z-order ----


// Funcion: vector<num> -> vector<bit> 
vector<bitini> to_bit(vector_short dato) {

    vector<bitini> dato_aux;

    hastafor(i, dato.size())
        dato_aux.push_back(bitini(dato[i]));

    return dato_aux;
}

// Funcion: bit -> int 
int to_int(bitfin dato) {
    int exp = 0, decimal = 0;

    do decimal += dato[0] * pow(2.0, exp++);
    while ((dato >>= 1) != 0);

    return decimal;
}

// Funciona para combinar datos multidimensionales
int combinar(vector<bitini> dato) {
    bitfin bit_final;

    hastafor(i, cant_dim * bit_ini)
        bit_final[i] = dato[i % cant_dim][i / cant_dim];

    return to_int(bit_final);
}

// Transformación a z_order
vector<int> z_order(vector<vector_short> dato_multidimensional) {
    // 0001 0100 = 00 01 00 10
    vector<int> datos_final;
    vector<bitini> dato_aux;

    // Convertir a bit y combinar los bits
    hastafor(i, cant_datos) {
        dato_aux = to_bit(dato_multidimensional[i]);
        datos_final.push_back(combinar(dato_aux));
        dato_aux.clear();
    }

    return datos_final;
}


//      ---- Funciones para Hallar los n-vecinos más cercanos ----


int distancia_euclides(vector_short dato_multidimensional, vector_short dato) {
    int sum = 0;

    hastafor(i, cant_dim)
        sum += pow((dato[i] - dato_multidimensional[i]), 2);

    return sqrt(sum);
}

// Los n-vecinos más cercanos en un espacio multidimensional
vector<int> nvecinos_cercano_multi(vector<vector_short> datos_multidimensionales, vector_short dato, int cant_vecinos_) {
    //0 0 0 0 0 INT32_MAX 
    // Cola de prioridad
    cant_vecinos_++; // Para ignorar el dato seleccionado (distancia 0)
    priority_queue<nodo, vector<nodo>, decltype(compare)*> q1(compare);
    hastafor(i, cant_vecinos_) q1.push(nodo(INT32_MAX, -1));

    // Hallamos las distancias
    hastafor(i, cant_datos) {
        nodo distancia(distancia_euclides(datos_multidimensionales[i], dato), i);
        q1.push(distancia);
        q1.pop();
    }

    // Extraemos los indices
    vector<int> indices;
    hastafor(i, cant_vecinos_-1) {
        indices.push_back(q1.top().index);
        q1.pop();
    }

    return indices;
}

// Los n-vecinos más cercanos en un espacio unidimensional
vector<int> nvecinos_cercano_uni(vector<int> datos_z_order, int dato, int cant_vecinos_) {

    // Cola de prioridad
    cant_vecinos_++; // Para ignorar el dato seleccionado (distancia 0)
    priority_queue<nodo, vector<nodo>, decltype(compare)*> q1(compare);
    hastafor(i, cant_vecinos_) q1.push(nodo(INT32_MAX, -1));

    // Hallamos las distancias
    hastafor(i, cant_datos) {
        nodo distancia(abs(datos_z_order[i] - dato), i);
        q1.push(distancia);
        q1.pop();
    }

    // Extraemos los indices
    vector<int> indices;
    hastafor(i, cant_vecinos_-1){
        indices.push_back(q1.top().index);
        q1.pop();
    }

    return indices;
}

int main() {

    // Matriz de datos multidimensionales
    vector<vector_short> datos_multi = rellenar(cant_datos, cant_dim);

    // Generamos el z-order
    vector<int> datos_z_order = z_order(datos_multi);

    // Indices de los vecinos más cercanos
    vector<vector<int>> vecinos_multi, vecinos_uni;

    hastafor(i, iteraciones) {
        int dato_random = rand() % cant_datos;
        vecinos_multi.push_back(nvecinos_cercano_multi(datos_multi, datos_multi[dato_random], cant_vecinos[i]));
        vecinos_uni.push_back(nvecinos_cercano_uni(datos_z_order, datos_z_order[dato_random], cant_vecinos[i]));
    }

    // Imprimimos los porcentajes de acierto para 5 10 15 y 20
    int ini = 0;
    float result;

    hastafor(i, iteraciones) {

        result = 0;
        hastafor(j, cant_vecinos[i]) hastafor(k, cant_vecinos[i])
            if (vecinos_uni[i][j] == vecinos_multi[i][k]) result++;

        ini = cant_vecinos[i]-1;

        cout << "El porcentaje de acierto para los " << cant_vecinos[i] 
        << " vecinos mas cercanos es:" << int((result / cant_vecinos[i]) * 100) 
        << "%" << endl;

    }

}