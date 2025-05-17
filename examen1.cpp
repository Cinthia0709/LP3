#include <iostream>
using namespace std;

int main() {
    int i, tope = 65, aux = 1;
    for (i = 55; i < tope; i = i + 2) {
        cout << "El valor de i es : " << i << endl;
        if (i % 2 == 0) {
            aux++;
        }
    }
    cout << "El valor de i es : " << i << endl;
    cout << "El valor de aux es : " << aux << endl;

    return 0;
}
