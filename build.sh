#!/bin/bash

# Script de compilación para FreeRTOS Posix Demo
# Asignatura: Sistemas Operativos 2

set -e  # Finalizar si ocurre un error

echo "======================================"
echo " Compilando Demo FreeRTOS Posix"
echo "======================================"

# Verificar que existe el directorio FreeRTOS
if [ ! -d "FreeRTOS" ]; then
    echo "❌ ERROR: No se encuentra el directorio 'FreeRTOS'."
    echo "   Asegúrate de que el entorno se ha construido correctamente."
    echo "   Si estás en Codespaces, espera a que termine el postCreateCommand."
    exit 1
fi

# Archivos fuente del kernel (lista explícita para evitar problemas con comodines)
KERNEL_SRCS="FreeRTOS/tasks.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/timers.c"
PORT_SRCS="FreeRTOS/portable/port.c FreeRTOS/portable/port_common.c FreeRTOS/portable/heap_3.c"

# Verificar que existen los archivos críticos
for file in $KERNEL_SRCS $PORT_SRCS; do
    if [ ! -f "$file" ]; then
        echo "❌ ERROR: Falta el archivo fuente: $file"
        echo "   Verifica que la descarga de FreeRTOS fue exitosa."
        exit 1
    fi
done

echo "✔ Archivos fuente verificados."

# Compilar
echo "🔨 Compilando..."
gcc -o demo main.c \
    $KERNEL_SRCS \
    $PORT_SRCS \
    -I FreeRTOS/include \
    -I FreeRTOS/portable \
    -I . \
    -lpthread \
    -Wall -Wextra

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilación exitosa. Ejecuta ./demo para iniciar la simulación."
else
    echo "❌ Falló la compilación. Revisa los mensajes de error arriba."
    exit 1
fi
