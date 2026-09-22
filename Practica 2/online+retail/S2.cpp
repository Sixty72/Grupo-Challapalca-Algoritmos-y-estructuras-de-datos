#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Algoritmos base
int busquedaLineal(const vector<long long>& a, long long x, long long& ops) {
    ops = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        ++ops;
        if (a[i] == x) return static_cast<int>(i);
    }
    return -1;
}

int busquedaBinaria(const vector<long long>& a, long long x, long long& ops) {
    int izq = 0, der = static_cast<int>(a.size()) - 1;
    ops = 0;
    while (izq <= der) {
        int medio = izq + (der - izq) / 2;
        ++ops;
        if (a[medio] == x) return medio;
        if (a[medio] < x) izq = medio + 1;
        else der = medio - 1;
    }
    return -1;
}

// Función para leer el CSV 
vector<long long> cargarDatos(int n) {
    vector<long long> datos;
    string filename = "dataset_n" + to_string(n) + ".csv";
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error al abrir " << filename << ". Verifica que estés ejecutando en la misma carpeta." << endl;
        return datos;
    }
    
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty()) {
            datos.push_back(stoll(line)); 
        }
    }
    return datos;
}

long long calcularMediana(vector<long long>& tiempos) {
    sort(tiempos.begin(), tiempos.end());
    return tiempos[tiempos.size() / 2];
}

int main() {
    vector<int> tamanos = {100, 1000, 10000, 100000, 500000};

    for (int n : tamanos) {
        vector<long long> datos = cargarDatos(n);
        if (datos.empty()) continue;

        vector<long long> datos_ordenados = datos;
        sort(datos_ordenados.begin(), datos_ordenados.end());

        long long objetivo_inicio = datos[0];
        long long objetivo_centro = datos[n / 2];
        long long objetivo_final = datos.back();
        long long objetivo_ausente = -99999;

        struct Escenario {
            string nombre;
            long long objetivo;
        };
        
        vector<Escenario> escenarios = {
            {"Inicio", objetivo_inicio},
            {"Centro", objetivo_centro},
            {"Final", objetivo_final},
            {"Ausente", objetivo_ausente}
        };

        cout << "\n--- Resultados para n = " << n << " ---" << endl;

        for (const auto& esc : escenarios) {
            vector<long long> tiempos_lin;
            vector<long long> tiempos_bin;
            long long ops_lin = 0, ops_bin = 0;

            for (int i = 0; i < 30; ++i) {
                auto start_lin = high_resolution_clock::now();
                busquedaLineal(datos, esc.objetivo, ops_lin);
                auto end_lin = high_resolution_clock::now();
                tiempos_lin.push_back(duration_cast<nanoseconds>(end_lin - start_lin).count());

                auto start_bin = high_resolution_clock::now();
                busquedaBinaria(datos_ordenados, esc.objetivo, ops_bin);
                auto end_bin = high_resolution_clock::now();
                tiempos_bin.push_back(duration_cast<nanoseconds>(end_bin - start_bin).count());
            }

            long long mediana_lin = calcularMediana(tiempos_lin);
            long long mediana_bin = calcularMediana(tiempos_bin);

            cout << "Escenario: " << esc.nombre << " (Objetivo: " << esc.objetivo << ")\n";
            cout << "  Lineal  -> Ops: " << ops_lin << " | Tiempo mediano: " << mediana_lin << " ns\n";
            cout << "  Binaria -> Ops: " << ops_bin << " | Tiempo mediano: " << mediana_bin << " ns\n";
        }
    }
    return 0;
}