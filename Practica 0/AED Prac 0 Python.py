import bisect
import math
import random
import time

# --- 1. ALGORITMOS DE BÚSQUEDA ---


def busqueda_lineal(a, x):
  for i in range(len(a)):
    if a[i] == x:
      return i
  return -1


def busqueda_binaria(a, x):
  idx = bisect.bisect_left(a, x)
  if idx < len(a) and a[idx] == x:
    return idx
  return -1


def busqueda_exponencial(a, x):
  if not a:
    return -1
  if a[0] == x:
    return 0

  i = 1
  n = len(a)
  while i < n and a[i] < x:
    i <<= 1

  # Búsqueda binaria en el rango [i // 2, min(i, n - 1)]
  sub_arr = a[i // 2 : min(i + 1, n)]
  idx = bisect.bisect_left(sub_arr, x)
  pos = (i // 2) + idx

  if pos < n and a[pos] == x:
    return pos
  return -1


def busqueda_interpolacion(a, x):
  low = 0
  high = len(a) - 1

  while low <= high and a[low] <= x <= a[high]:
    if a[low] == a[high]:
      return low if a[low] == x else -1

    # Estimación de posición
    pos = low + int(
        (float(high - low) / (a[high] - a[low])) * (x - a[low])
    )

    if pos < low or pos > high:
      break

    if a[pos] == x:
      return pos
    if a[pos] < x:
      low = pos + 1
    else:
      high = pos - 1

  return -1


# --- 2. MEDIDOR DE TIEMPO (BENCHMARK) ---


def medir(a, queries, buscador):
  t0 = time.perf_counter()

  checksum = 0
  for x in queries:
    checksum += buscador(a, x)

  t1 = time.perf_counter()
  ms_totales = (t1 - t0) * 1000.0
  return ms_totales / len(queries)  # Tiempo promedio en milisegundos


# --- 3. EJECUCIÓN PRINCIPAL ---


def main():
  ns = [10000, 100000, 500000]
  random.seed(123)  # Semilla para reproducibilidad

  print('=== BENCHMARK DE ALGORITMOS DE BÚSQUEDA (PYTHON) ===\n')

  for n in ns:
    # a) Uniforme ordenado
    uni = list(range(n))

    # b) Sesgado ordenado (80% en [0, 0.1n), 20% en [0.1n, 5n))
    ses = []
    for _ in range(n):
      if random.random() < 0.8:
        ses.append(random.randint(0, int(0.1 * n)))
      else:
        ses.append(random.randint(int(0.1 * n), 5 * n))
    ses.sort()

    # c) Desordenado
    des = list(uni)
    random.shuffle(des)

    # Generación de 1000 consultas (70% hits, 30% misses)
    def mkqueries(base):
      q = []
      # Hits
      for _ in range(700):
        q.append(random.choice(base))
      # Misses
      for i in range(300):
        q.append(int(10 * len(base)) + i)
      random.shuffle(q)
      return q

    q_uni = mkqueries(uni)
    q_ses = mkqueries(ses)
    q_des = mkqueries(des)

    def report(nombre, arr, q, es_ordenado):
      print('-' * 45)
      print(f'n = {n} | Dataset: {nombre}')
      print('-' * 45)
      t_lineal = medir(arr, q, busqueda_lineal)
      print(f' Lineal       : {t_lineal:.6f} ms/busq')

      if es_ordenado:
        t_binaria = medir(arr, q, busqueda_binaria)
        t_exp = medir(arr, q, busqueda_exponencial)
        t_interp = medir(arr, q, busqueda_interpolacion)

        print(f' Binaria      : {t_binaria:.6f} ms/busq')
        print(f' Exponencial  : {t_exp:.6f} ms/busq')
        print(f' Interpolacion: {t_interp:.6f} ms/busq')
      else:
        print(' (Datos no ordenados: omitidos algoritmos de rango)')
      print()

    report('Uniforme Ordenado', uni, q_uni, True)
    report('Sesgado Ordenado', ses, q_ses, True)
    report('Desordenado', des, q_des, False)


if __name__ == '__main__':
  main()