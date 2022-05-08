#include <iostream>
#include <vector>

using namespace std;
#define f(i,ini,fin) for(int i = ini; i < fin; i++)


struct QuadNode {
    QuadNode(int e[4]) { 
        ejes[0] = e[0]; ejes[1] = e[1]; ejes[2] = e[2]; ejes[3] = e[3];
        nodes[0] = nodes[1] = nodes[2] = nodes[3] = 0; 
    }
    int ejes[4];
    int puntos = 0;
    QuadNode* nodes[4];
};
class QuadTree {
public:
    QuadTree(int,int,int);

    bool Insert(int,int);
    bool PrintPuntos(int, int);

private:

    QuadNode* root;
    int x, y;

};
void generar(QuadNode* a, int nivel) {
    if (nivel == 0) return;
    int xmid = (a->ejes[0] + a->ejes[1]) / 2;
    int ymid = (a->ejes[2] + a->ejes[3]) / 2;

    int ejes0[4] = { a->ejes[0],xmid,ymid,a->ejes[3] };
    a->nodes[0] = new QuadNode(ejes0);

    int ejes1[4] = { xmid,a->ejes[1],ymid,a->ejes[3] };
    a->nodes[1] = new QuadNode(ejes1);

    int ejes2[4] = { a->ejes[0],xmid,a->ejes[2],ymid };
    a->nodes[2] = new QuadNode(ejes2);

    int ejes3[4] = { xmid,a->ejes[1],a->ejes[2],ymid };
    a->nodes[3] = new QuadNode(ejes3);

    generar(a->nodes[0], nivel - 1);
    generar(a->nodes[1], nivel - 1);
    generar(a->nodes[2], nivel - 1);
    generar(a->nodes[3], nivel - 1);

}
QuadTree::QuadTree(int x_, int y_, int niveles) {

    int ejes[4] = { 0,x_,0,y_ };
    root = new QuadNode(ejes);
    generar(root, niveles - 1);

}
bool QuadTree::Insert(int x, int y) {
    QuadNode** p;
    if (x > root->ejes[1] || y > root->ejes[3]) return 0;

    for (p = &root; *p; ) {
        int xmid = ((*p)->ejes[0] + (*p)->ejes[1]) / 2;
        int ymid = ((*p)->ejes[2] + (*p)->ejes[3]) / 2;
        (*p)->puntos++;

        if (x > xmid) 
            if (y > ymid) 
                p = &((*p)->nodes[1]);
            else p = &((*p)->nodes[3]);
        else if (y < ymid)
            p = &((*p)->nodes[2]);
        else p = &((*p)->nodes[0]);

    }

    return 1;
}
bool QuadTree::PrintPuntos(int x, int y) {
    QuadNode** p;
    if (x > root->ejes[1] || y > root->ejes[3]) return 0;
    int nivel = 1;
    for (p = &root; *p; ) {
        int xmid = ((*p)->ejes[0] + (*p)->ejes[1]) / 2;
        int ymid = ((*p)->ejes[2] + (*p)->ejes[3]) / 2;
        cout << "Puntos en el nivel " << nivel++ << ": ";
        cout << (*p)->puntos << endl;

        if (x > xmid)
            if (y > ymid)
                p = &((*p)->nodes[1]);
            else p = &((*p)->nodes[3]);
        else if (y < ymid)
            p = &((*p)->nodes[2]);
        else p = &((*p)->nodes[0]);

    }

    return 1;
}
int main() {

    // Se ingresan datos de la forma:
    // - Puntos Dimensionalidad
    // - x , y, ...
    int a, b, d, t;

    cout << "  Ingresar los limites del mapa" << endl;
    cout << "  seguido de los niveles" << endl; cout << endl;
    cin >> a >> b;
    cin >> d;

    QuadTree Arbol(a,b,d);

    // Se agregan de manera automatica al arból
    cout << "  Ingresar la cantidad de puntos" << endl;
    cout << "  seguido de los puntos en si" << endl; cout << endl;
    for (cin >> t; t--; Arbol.Insert(a, b))
        cin >> a >> b;


    cout << "  -- Probando la funcion PrintPuntos --" << endl;
    cout << "    Se ingresa un punto y el programa" << endl;
    cout << "    recorre el arbol imprimiendo la " << endl;
    cout << "    cantidad de puntos por nivel recorrido" << endl;
    cout << "  Ingresar un punto que choque con el" << endl;
    cout << "  cuadrado a evaluar" << endl; cout << endl;
    cin >> a >> b;
    Arbol.PrintPuntos(a, b);

    return 0;
}