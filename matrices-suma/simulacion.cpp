#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Clase Matriz que genera una matriz cuadrada con valores aleatorios entre 0 y 9
class Matriz {
private:
    int tam;
    vector<vector<int>> datos;

public:
    Matriz(int t) : tam(t), datos(t, vector<int>(t)) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 9);

        for (int i = 0; i < tam; ++i)
            for (int j = 0; j < tam; ++j)
                datos[i][j] = dis(gen);
    }

    int getTam() const { return tam; }
    int getValor(int i, int j) const { return datos[i][j]; }
};

// Suma secuencial de los elementos de la matriz
class SumadorSecuencial {
public:
    static int sumar(const Matriz& m) {
        int suma = 0;
        for (int i = 0; i < m.getTam(); ++i)
            for (int j = 0; j < m.getTam(); ++j)
                suma += m.getValor(i, j);
        return suma;
    }
};

// Suma paralela utilizando hilos
class SumadorThread {
public:
    static void sumarBloque(const Matriz& m, int inicio, int fin, int& resultado) {
        resultado = 0;
        int tam = m.getTam();
        for (int i = inicio; i < fin; ++i)
            for (int j = 0; j < tam; ++j)
                resultado += m.getValor(i, j);
    }

    static int sumar(const Matriz& m, int numThreads) {
        int tam = m.getTam();
        vector<thread> hilos;
        vector<int> resultados(numThreads, 0);
        int bloque = tam / numThreads;
        int inicio = 0;

        for (int i = 0; i < numThreads; ++i) {
            int fin = (i == numThreads - 1) ? tam : inicio + bloque;
            hilos.emplace_back(sumarBloque, ref(m), inicio, fin, ref(resultados[i]));
            inicio = fin;
        }

        for (auto& h : hilos)
            h.join();

        int total = 0;
        for (int res : resultados)
            total += res;

        return total;
    }
};

int main() {
    int N;
    cout << "Ingrese el tamaño máximo de la matriz NxN: ";
    cin >> N;

    // Limpiar archivos anteriores
    ofstream("resultados_secuencial.txt").close();
    ofstream("resultados_threads.txt").close();

    // Archivos de salida
    ofstream archivoSecuencial("resultados_secuencial.txt");
    if (!archivoSecuencial) {
        cerr << "No se pudo crear el archivo de salida secuencial.\n";
        return 1;
    }

    unsigned int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;

    cout << "\n--- SUMA SECUENCIAL ---\n";
    for (int tam = 1; tam <= N; ++tam) {
        Matriz matriz(tam);
        auto inicio = high_resolution_clock::now();
        int suma = SumadorSecuencial::sumar(matriz);
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<nanoseconds>(fin - inicio).count();

        cout << "Tamaño: " << tam << "x" << tam << ", Suma: " << suma << ", Tiempo: " << duracion << " ns\n";
        archivoSecuencial << tam << " " << duracion << endl;
    }
    archivoSecuencial.close();

    ofstream archivoThreads("resultados_threads.txt");
    if (!archivoThreads) {
        cerr << "No se pudo crear el archivo de salida threads.\n";
        return 1;
    }

    cout << "\n--- SUMA CON THREADS (" << numThreads << " hilos) ---\n";
    for (int tam = 1; tam <= N; ++tam) {
        Matriz matriz(tam);
        auto inicio = high_resolution_clock::now();
        int suma = SumadorThread::sumar(matriz, numThreads);
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<nanoseconds>(fin - inicio).count();

        cout << "Tamaño: " << tam << "x" << tam << ", Suma: " << suma << ", Tiempo: " << duracion << " ns\n";
        archivoThreads << tam << " " << duracion << endl;
    }
    archivoThreads.close();

    cout << "\nResultados guardados en 'resultados_secuencial.txt' y 'resultados_threads.txt'\n";
    return 0;
}

