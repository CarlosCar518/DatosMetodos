
import random
from pathlib import Path

# Carpeta de salida
output_dir = Path("datos_ordenamiento")
output_dir.mkdir(exist_ok=True)

# Generador aleatorio del sistema operativo
rng = random.SystemRandom()

# Tamaños de los conjuntos
sizes = [1_0000, 50_000, 200_000]

# Rango de valores
MIN_VALUE = 0
MAX_VALUE = 1_000_000

for size in sizes:
    # Generar números pseudoaleatorios
    values = [
        rng.randint(MIN_VALUE, MAX_VALUE)
        for _ in range(size)
    ]

    # Crear archivo
    file_path = output_dir / f"datos_{size}.txt"

    with file_path.open("w", encoding="utf-8") as file:
        for value in values:
            file.write(f"{value}\n")

    print(f"Generado: {file_path} ({size} elementos)")

print("Generación finalizada.")
