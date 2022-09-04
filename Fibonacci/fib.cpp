#include <iostream>
#include <vector>

using namespace std;

template <class V>
struct matriz {

	matriz(int x_, int y_) {
		valores.resize(x_);
		for (int i = 0; i < valores.size(); valores[i].resize(y_), i++);
		x = x_; y = y_;
	}
	matriz(vector<vector<V>> valores_) {
		valores = valores_;
		x = valores.size();
		y = valores[0].size();
	}
	friend matriz<V>& operator *(const matriz<V>& p1, const matriz<V>& p2);
	friend matriz<V>& operator *=(const matriz<V>& p1, const matriz<V>& p2);

	vector<vector<V>> valores;
	int x, y;

};
template <class V>
matriz<V>& operator *(matriz<V>& p1, matriz<V>& p2)
{
	matriz<V>* resultado = new matriz<V>(p1.x, p1.y);
	for (int i = 0; i < p1.x; i++)
		for (int j = 0; j < p2.y; j++) {
			(resultado->valores)[i][j] = 0;
			for (int k = 0; k < p1.y; k++)
				(resultado->valores)[i][j] += p1.valores[i][k] * p2.valores[k][j];
		}
	return *resultado;
}
template <class V>
void operator *=(matriz<V>& p1, matriz<V>& p2) { p1 = p1 * p2; }

template<class V>
matriz<V>& matriz_identidad(int x) {
	matriz<V>* rpta = new matriz<V>(x, x);
	for (int i = 0; i < x; (*rpta).valores[i][i] = 1, i++);
	return *rpta;
}

template<class I>
I exp_rapida(I a, int n, I& r = 1) {
	for (; true; a *= a, n = n >> 1)
		if (n % 2) r *= a;
		else if (n == 0) return r;
}

//	----		Declaración de los fibonacci		----

int fib(int i) {
	if (i < 2)
		return 1;
	else return fib(i - 1) + fib(i - 2);
}
int fib_it(int i) {
	int a = 1, b = 1;
	int r = 1;
	while (i > 1) {
		r = a + b;
		b = a;
		a = r;
		i--;
	}
	return r;
}

vector<int> fib_vector;

int fib_lineal(int i) {
	if (i < 2)
		return fib_vector[i];
	if (fib_vector.size() > i) {
		return fib_vector[i];
	}
	else if (fib_vector.size() == i + 1) {
		fib_vector.push_back(fib_vector[i - 1] + fib_vector[i]);
		return fib_vector[i];
	}
	else return fib_lineal(i - 1) + fib_lineal(i - 2);
}

template<class V>
V fib_matrx(int x) {

	matriz<V> matriz_fib(vector<vector<V>>{
		vector<V>{ 1, 1 },
		vector<V>{ 1, 0 }
	});
	return exp_rapida<matriz<V>>(matriz_fib, x, matriz_identidad<V>(2)).valores[0][0];

}

int main() {
		cout << fib_matrx<long long>(91); // Maximo valor con long long
}