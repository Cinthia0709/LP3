- Análisis de Complejidad del Algoritmo de Ordenamiento por Inserción

Este proyecto analiza el tiempo de ejecución del algoritmo de ordenamiento por inserción en su peor caso. Se generan listas en orden inverso, se ordenan con el algoritmo y se mide el tiempo de ejecución. Finalmente, los resultados se guardan en un archivo y se grafican con GNU Plot.

- Integrantes
Karlo Eduardo Ayala Salazar
Dylan Edward Davila Grau
Cinthia America Blanco Rodrigo

- Porciones de Código fuente

Worst Case: Se generan arreglos en orden inverso.
-----------------------------------------
void worstcase(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i - 1; 
    }
}

Sort Algorithm: Se aplica el ordenamiento por inserción.
---------------------------------------------
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

Times: Se mide el tiempo de ejecución del algoritmo.
----------------------------------------------
double Times(int arr[], int n) {
    auto start = high_resolution_clock::now();
    insertionSort(arr, n);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

File Writer: Se escriben los tiempos en un archivo "resultados.txt".
----------------------------------------------------
void FileWriter(int n, double time_taken) {
    ofstream file("resultados.txt", ios::app); 
    if (file.is_open()) {
        file << n << ", " << time_taken << endl;
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo." << endl;
    }
}

GNU Plot: Se grafican los resultados hasta "n = 10,000".
---------------------------------------------
gnuplot grafica.gnu
