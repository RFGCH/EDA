#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <math.h>
#include <vector>
#include <map>

#include <fstream>
#include <string>

#define tipo_dato_ini int
#define tipo_dato_fin int
#define tipo_ini_fin tipo_dato_ini,tipo_dato_fin
 
#define short_for(i,ini,fin) for(int i = ini; i< fin; i++)

using namespace std;

const int cnt_datos[] = { 10000,20000,30000,40000,50000 };
const int dim[] = { 50 };
const int max_rand = 1000;

vector<vector<vector<vector<tipo_dato_ini>>>> lista(size(dim));
vector<vector<vector<tipo_dato_fin>>> lista_dist(size(dim));
map<tipo_dato_fin, int> mapa;
vector<tipo_dato_fin> key_mapa;

// Genera la lista con numeros random
// Genera la lista final con 0
template<class I, class F>
void generar() {
	short_for(i, 0, lista.size()) {
		vector<vector<vector<I>>> aux1(size(cnt_datos));
		short_for(j, 0, size(cnt_datos)) {
			vector<vector<I>> aux2(cnt_datos[j]);
			short_for(k, 0, cnt_datos[j]) {
				vector<I> aux3(dim[i]);
				generate(aux3.begin(), aux3.end(), rand);
				short_for(l, 0, dim[i]) aux3[l] = aux3[l] % max_rand;
				aux2[k] = aux3;
			}
			aux1[j] = aux2;
		}
		lista[i] = aux1;
	}
	short_for(i, 0, lista.size()) {
		vector<vector<I>> aux1(size(cnt_datos));
		short_for(j, 0, size(cnt_datos)) {
			vector<I> aux2(cnt_datos[j]);
			aux1[j] = aux2;
		}
		lista_dist[i] = aux1;
	}
}

template <class I, class F>
F calculo_dst_eucl(vector<I> a, vector<I> b) {

	double sum = 0;
	short_for(i, 0, a.size()) 
		sum += pow((a[i] - b[i]),2);
	
	return F(sqrt(sum));
}

void dist_eucl() {
	short_for(i, 0, lista.size()) 
		short_for(j, 0, lista[i].size()) 
			short_for(k, 1, lista[i][j].size()) 
				lista_dist[i][j][k] = calculo_dst_eucl<tipo_ini_fin>(lista[i][j][0], lista[i][j][k]);
}

void print_map() {

	for (int i : key_mapa) 
		cout << i << " " << mapa[i] << endl;
	
}

void crear_archivo(int i, int j) {
	string filename;
	filename = "C:/Users/RUBEN GUZMAN/source/repos/ConsoleApplication2/ConsoleApplication2/archivo";
	filename.append(to_string(i));
	filename.append(to_string(j));
	filename.append(".txt");
	fstream file;
	file.open(filename);

	for (int i : key_mapa)
		file << to_string(i) << " " << to_string(mapa[i]) << "\n";

}

void frecuencia_dist() {
	short_for(i, 0, lista_dist.size()) {
		cout << " Dimension: " << dim[i] << endl;
		short_for(j, 0, lista_dist[i].size()) {
			cout << "  Cantidad de datos: " << cnt_datos[j] << endl;
			short_for(k, 1, lista_dist[i][j].size()) {
				if (!mapa[lista_dist[i][j][k]])
					key_mapa.push_back(lista_dist[i][j][k]);
				mapa[lista_dist[i][j][k]]++;
			}

			crear_archivo(i,j);

			//print_map();
			mapa.clear();
			key_mapa.clear();
			cout << endl;
		}
		cout << endl;
	}
}
// Dimensiones
// 1d Dimensión de cantidad de datos
// 2d Cantidad de datos
// 3d Dimensión de cantidad de dimensiones
// 4d Cantidad de dimensiones
int main()
{
	srand((unsigned int)time(NULL));
	generar<tipo_ini_fin>();
	dist_eucl();
	frecuencia_dist();
}