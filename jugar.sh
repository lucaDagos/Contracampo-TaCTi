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
cd "$DIR/GUI"
gcc GUI.c -o gui-app `pkg-config --cflags --libs gtk+-3.0`
./gui-app



# Limpiar después de cerrar GUI
echo "🧹 Limpiando archivos temporales..."
rm -rf "$DIR/GUI/gui-app"
cd "$DIR"
make clean