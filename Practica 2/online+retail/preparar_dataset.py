import pandas as pd
import os

def preparar_datos(excel_path='Online Retail.xlsx'):
    print(f"Cargando el archivo: {excel_path}...")
    print("Esto puede tomar un par de minutos dependiendo de tu computadora...")
    
    try:
        # Cargar el excel
        df = pd.read_excel(excel_path)
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo '{excel_path}'.")
        print("Asegúrate de que el archivo Excel esté en la misma carpeta que este script y tenga ese nombre.")
        return

    print("Limpiando los datos...")
    df_limpio = df.dropna(subset=['CustomerID']).copy()
    df_limpio['CustomerID'] = df_limpio['CustomerID'].astype('int64')
    
    claves = df_limpio['CustomerID'].tolist()
    total_disponible = len(claves)
    print(f"Total de registros válidos encontrados: {total_disponible}")

    tamanos = [100, 1000, 10000, 100000, 500000]
    
    for n in tamanos:
        if n <= total_disponible:
            muestra = claves[:n]
        else:
            print(f"Advertencia: No hay suficientes datos para n={n}. Se completará repitiendo datos.")
            muestra = (claves * (n // total_disponible + 1))[:n]
            
        # Guardar en archivo CSV
        nombre_archivo = f"dataset_n{n}.csv"
        df_export = pd.DataFrame({'CustomerID': muestra})
        df_export.to_csv(nombre_archivo, index=False)
        print(f" -> Generado: {nombre_archivo} ({n} registros)")
        
    print("¡Listo! Ya tienes tus archivos CSV para probar los algoritmos.")

if __name__ == '__main__':
    preparar_datos()
