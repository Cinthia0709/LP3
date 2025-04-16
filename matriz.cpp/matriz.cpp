#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int** crearMatriz(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) {
        cerr << "Error: Las dimensiones deben ser mayores que cero." << endl;
        return {};
    }

    int** matriz = new int*[filas]; 
    for (int i = 0; i < filas; ++i) {
        matriz[i] = new int[columnas]; 
    }
    return matriz;
}
void rellenarMatriz(int** arr, int filas, int columnas) {
    srand(time(0)); 

    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            arr[i][j] = rand() % 100; 
        }
    }
}
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int** crearMatriz(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) {
        cerr << "Error: Las dimensiones deben ser mayores que cero." << endl;
        return {};
    }

    int** matriz = new int*[filas]; 
    for (int i = 0; i < filas; ++i) {
        matriz[i] = new int[columnas]; 
    }
    return matriz;
}
void rellenarMatriz(int** arr, int filas, int columnas) {
    srand(time(0)); 

    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            arr[i][j] = rand() % 100; 
        }
    }
}


int sumarmatriz(int** arr, int filas, int columnas){

    int sum = 0;
    
        for(int i=0;i<filas;i++){
            for(int j=0;j<columnas;j++){
                sum=sum+arr[i][j];
            }
        }
    return sum;
}

void mostrarMatriz(int** matriz, int filas, int columnas) {
    cout << "La matriz es:" << endl;
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){

    int filas, columnas;

    cout << "Ingrese el nÃºmero de filas: ";
    cin >> filas;
    
    cout << "Ingrese el nÃºmero de columnas: ";
    cin >> columnas;

 int** matriz = crearMatriz(filas, columnas);
 
 if (matriz == nullptr) {
        return 1;
    }
    
 rellenarMatriz(matriz,filas,columnas);
 mostrarMatriz(matriz,filas,columnas);
 
 int result=sumarmatriz(matriz,filas,columnas);

    cout<<result<<endl;

return 0;
}

int sumarmatriz(int** arr, int filas, int columnas){

    int sum = 0;
    
        for(int i=0;i<filas;i++){
            for(int j=0;j<columnas;j++){
                sum=sum+arr[i][j];
            }
        }
    return sum;
}

void mostrarMatriz(int** matriz, int filas, int columnas) {
    cout << "La matriz es:" << endl;
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){

    int filas, columnas;

    cout << "Ingrese el nÃºmero de filas: ";
    cin >> filas;
    
    cout << "Ingrese el nÃºmero de columnas: ";
    cin >> columnas;

 int** matriz = crearMatriz(filas, columnas);
 
 if (matriz == nullptr) {
        return 1;
    }
    
 rellenarMatriz(matriz,filas,columnas);
 mostrarMatriz(matriz,filas,columnas);
 
 int result=sumarmatriz(matriz,filas,columnas);

    cout<<result<<endl;

return 0;
}
