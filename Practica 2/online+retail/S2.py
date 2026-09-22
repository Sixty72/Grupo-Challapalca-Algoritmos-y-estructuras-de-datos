import csv
import statistics
from time import perf_counter_ns

def busqueda_lineal(a, x):
    ops = 0
    for i, valor in enumerate(a):
        ops += 1
        if valor == x:
            return i, ops
    return -1, ops

def busqueda_binaria(a, x):
    izq, der, ops = 0, len(a) - 1, 0
    while izq <= der:
        medio = izq + (der - izq) // 2
        ops += 1
        if a[medio] == x:
            return medio, ops
        if a[medio] < x:
            izq = medio + 1
        else:
            der = medio - 1
    return -1, ops

def cargar_datos(n):
    datos = []
    with open(f'dataset_n{n}.csv', 'r') as f:
        reader = csv.reader(f)
        next(reader) 
        for row in reader:
            datos.append(int(row[0]))
    return datos

tamanos = [100, 1000, 10000, 100000, 500000]

for n in tamanos:
    datos = cargar_datos(n)
    datos_ordenados = sorted(datos)
    
    objetivos = {
        "Inicio": datos[0],
        "Centro": datos[n // 2],
        "Final": datos[-1],
        "Ausente": -99999 
    }

    print(f"\n--- Resultados para n = {n} ---")
    
    for escenario, objetivo in objetivos.items():
        tiempos_lin = []
        tiempos_bin = []
        ops_lin = 0
        ops_bin = 0
        
        for _ in range(30):
            inicio = perf_counter_ns()
            _, ops_lin = busqueda_lineal(datos, objetivo)
            fin = perf_counter_ns()
            tiempos_lin.append(fin - inicio)
            

            inicio = perf_counter_ns()
            _, ops_bin = busqueda_binaria(datos_ordenados, objetivo)
            fin = perf_counter_ns()
            tiempos_bin.append(fin - inicio)
        
        mediana_lin = statistics.median(tiempos_lin)
        mediana_bin = statistics.median(tiempos_bin)
        
        print(f"Escenario: {escenario} (Objetivo: {objetivo})")
        print(f"  Lineal  -> Ops: {ops_lin} | Tiempo mediano: {mediana_lin:.2f} ns")
        print(f"  Binaria -> Ops: {ops_bin} | Tiempo mediano: {mediana_bin:.2f} ns")