#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <string>

using namespace std;

// --- 1. ALGORITMOS DE BÚSQUEDA ---

int busquedaLineal(const vector<int>& a, int x) {
    for (size_t i = 0; i < a.size(); ++i) 
        if (a[i] == x) return (int)i;
    return -1;
}

int busquedaBinaria(const vector<int>& a, int x) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        else if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int busquedaExponencial(const vector<int>& a, int x) {
    if (a.empty()) return -1;
    if (a[0] == x) return 0;
    int i = 1;
    while (i < (int)a.size() && a[i] < x) i <<= 1;
    int l = i / 2, r = min(i, (int)a.size() - 1);
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        else if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int busquedaInterpolacion(const vector<int>& a, int x) {
    int low = 0, high = (int)a.size() - 1;
    while (low <= high && x >= a[low] && x <= a[high]) {
        if (a[low] == a[high]) return (a[low] == x ? low : -1);
        long long pos = low + (long long)((double)(high - low) * ((double)(x - a[low]) / (double)(a[high] - a[low])));
        if (pos < low || pos > high) break;
        if (a[pos] == x) return (int)pos;
        if (a[pos] < x) low = (int)pos + 1; 
        else high = (int)pos - 1;
    }
    return -1;
}

// --- 2. MEDIDOR DE TIEMPOS (BENCHMARK) ---

template <class F>
double medir(const vector<int>& a, const vector<int>& queries, F buscador) {
    using clk = chrono::high_resolution_clock;
    auto t0 = clk::now();
    
    volatile long long checksum = 0; // Evita que el compilador elimine el bucle por optimización
    for (int x : queries) checksum += buscador(a, x);
    
    auto t1 = clk::now();
    chrono::duration<double, milli> ms = t1 - t0;
    return ms.count() / (double)queries.size(); // Retorna milisegundos promedio por búsqueda
}

// --- 3. FUNCIÓN PRINCIPAL ---

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> ns = {10000, 100000, 500000};
    mt19937 rng(123); // Semilla fija para reproducibilidad

    cout << "=== BENCHMARK DE ALGORITMOS DE BUSQUEDA ===\n\n";

    for (int n : ns) {
        // a) Uniforme ordenado
        vector<int> uni(n);
        iota(uni.begin(), uni.end(), 0);

        // b) Sesgado ordenado
        vector<int> ses(n);
        for (int i = 0; i < n; i++) {
            double u = uniform_real_distribution<double>(0, 1)(rng);
            if (u < 0.8) ses[i] = uniform_int_distribution<int>(0, (int)(0.1 * n))(rng);
            else ses[i] = uniform_int_distribution<int>((int)(0.1 * n), 5 * n)(rng);
        }
        sort(ses.begin(), ses.end());

        // c) Desordenado
        vector<int> des = uni;
        shuffle(des.begin(), des.end(), rng);

        // Generación de 1000 consultas (70% hits, 30% misses)
        auto mkqueries = [&](const vector<int>& base) {
            vector<int> q; q.reserve(1000);
            uniform_int_distribution<int> idx(0, (int)base.size() - 1);
            for (int i = 0; i < 700; i++) q.push_back(base[idx(rng)]);
            for (int i = 0; i < 300; i++) q.push_back((int)(10 * base.size()) + i);
            shuffle(q.begin(), q.end(), rng);
            return q;
        };

        auto q_uni = mkqueries(uni);
        auto q_ses = mkqueries(ses);
        auto q_des = mkqueries(des);

        auto report = [&](const string& nombre, const vector<int>& arr, const vector<int>& q) {
            cout << "----------------------------------------\n";
            cout << "n = " << n << " | Dataset: " << nombre << "\n";
            cout << "----------------------------------------\n";
            cout << " Lineal       : " << medir(arr, q, busquedaLineal) << " ms/busq\n";
            
            if (is_sorted(arr.begin(), arr.end())) {
                cout << " Binaria      : " << medir(arr, q, busquedaBinaria) << " ms/busq\n";
                cout << " Exponencial  : " << medir(arr, q, busquedaExponencial) << " ms/busq\n";
                cout << " Interpolacion: " << medir(arr, q, busquedaInterpolacion) << " ms/busq\n";
            } else {
                cout << " (Datos no ordenados: omitidos algoritmos de rango)\n";
            }
            cout << "\n";
        };

        report("Uniforme Ordenado", uni, q_uni);
        report("Sesgado Ordenado", ses, q_ses);
        report("Desordenado", des, q_des);
    }

    return 0;
}