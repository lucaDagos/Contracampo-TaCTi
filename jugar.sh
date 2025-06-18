#!/bin/bash

# Salir si hay errores
set -e

# Directorio del script
DIR="$(cd "$(dirname "$0")" && pwd)"

# 1. Compilar el proyecto
echo "🔧 Compilando con make..."
make

# 2. Ejecutar GUI en Python
echo "🚀 Ejecutando GUI..."
python3 "GUI.py"


# Limpiar después de cerrar GUI
echo "🧹 Limpiando archivos temporales..."
cd "$DIR"
make clean