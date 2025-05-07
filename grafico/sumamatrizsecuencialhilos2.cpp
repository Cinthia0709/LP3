#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
using namespace std;
using namespace chrono;

void sumarBloque(const vector<vector<int>>& matriz, int inicioFila, int finFila, long long& resultado) {
    long long suma = 0;
    for (int i = inicioFila; i < finFila; ++i) {
        for (int j = 0; j < matriz[i].size(); ++j) {
            suma += matriz[i][j];
        }
    }
    resultado = suma;
}

int main() {
    int maxTam;
    cout << "Ingrese el tamaño maximo de la matriz (nxn): ";
    cin >> maxTam;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100); 

    unsigned int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;

    for (int tam = 1; tam <= maxTam; ++tam) {
        // Generar la matriz tam x tam
        vector<vector<int>> matriz(tam, vector<int>(tam));
        for (int i = 0; i < tam; ++i) {
            for (int j = 0; j < tam; ++j) {
                matriz[i][j] = dis(gen);
            }
        }

        int filasPorHilo = tam / numThreads;
        int resto = tam % numThreads;

        vector<thread> hilos;
        vector<long long> resultados(numThreads, 0);

        auto inicio = high_resolution_clock::now();

        int filaInicio = 0;
        for (unsigned int i = 0; i < numThreads; ++i) {
            int filaFin = filaInicio + filasPorHilo + (i < resto ? 1 : 0);
            hilos.emplace_back(sumarBloque, cref(matriz), filaInicio, filaFin, ref(resultados[i]));
            filaInicio = filaFin;
        }

        for (auto& h : hilos) {
            h.join();
        }

        long long sumaTotal = 0;
        for (auto s : resultados) {
            sumaTotal += s;
        }

        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);

        cout << tam <<" "<< duracion.count() << endl;
    }

    return 0;
}

