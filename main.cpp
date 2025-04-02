#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

void worstcase(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i - 1; 
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

double Times(int arr[], int n) {
    auto start = high_resolution_clock::now();
    
    insertionSort(arr, n);

    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

void FileWriter(int n, double time_taken) {
    ofstream file("resultados.txt", ios::app); 
    if (file.is_open()) {
        file << n << ", " << time_taken << endl;
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo." << endl;
    }
}

int main() {
  
    ofstream file("resultados.txt", ios::trunc);
    if (file.is_open()) {
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo para limpiar." << endl;
    }

    for (int n = 1; n <= 10000; ++n) {
        int* arr = new int[n];

        worstcase(arr, n);

        double time_taken = Times(arr, n);

        FileWriter(n, time_taken);

        cout << n << ", " << time_taken<< endl;

        delete[] arr;
    }

    cout << "Resultados guardados en resultados.txt" << endl;
    return 0;
}
