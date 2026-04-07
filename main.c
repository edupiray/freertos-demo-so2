/**
 * DEMOSTRACIÓN DE FreeRTOS - Simulador Posix (Linux)
 * Asignatura: Sistemas Operativos 2
 * 
 * Esta demo ejecuta FreeRTOS como un proceso normal de Linux.
 * No requiere hardware simulado. La salida es por consola.
 * 
 * CONCEPTOS DEMOSTRADOS:
 *   - Creación de tareas (xTaskCreate)
 *   - Prioridades (mayor número = mayor prioridad)
 *   - Planificación expulsiva (preemptive)
 *   - Inanición (starvation) por tarea de alta prioridad
 */

#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"

// Prototipos de las tareas
void tareaLED(void *parametros);
void tareaContador(void *parametros);
void tareaBloqueante(void *parametros);

int main(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║         DEMOSTRACIÓN DE FreeRTOS - SISTEMAS OPERATIVOS 2      ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Tarea LED       (Prioridad 1) : Parpadeo cada 1 segundo      ║\n");
    printf("║  Tarea Contador  (Prioridad 1) : Incrementa cada 2 segundos   ║\n");
    printf("║  Tarea Bloqueante(Prioridad 3) : Bloquea el sistema 5 seg     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");

    // Creación de tareas
    xTaskCreate(tareaLED,        "LED",      128, NULL, 1, NULL);
    xTaskCreate(tareaContador,   "Contador", 128, NULL, 1, NULL);
    xTaskCreate(tareaBloqueante, "Bloqueo",  128, NULL, 3, NULL);

    // Iniciar el planificador de FreeRTOS
    vTaskStartScheduler();

    // Si todo funciona correctamente, nunca se llegará aquí
    return 0;
}

/**
 * Tarea LED: Simula el parpadeo de un LED imprimiendo mensajes.
 */
void tareaLED(void *parametros) {
    while (1) {
        printf("[LED] ENCENDIDO\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
        
        printf("[LED] APAGADO\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
    }
}

/**
 * Tarea Contador: Incrementa y muestra un contador cada 2 segundos.
 */
void tareaContador(void *parametros) {
    int contador = 0;
    while (1) {
        contador++;
        printf("[CONTADOR] %d\n", contador);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Espera 2 segundos
    }
}

/**
 * Tarea Bloqueante: Cada 10 segundos toma el control de la CPU
 * y ejecuta un bucle de espera activa que impide la ejecución
 * de las tareas de menor prioridad.
 */
void tareaBloqueante(void *parametros) {
    while (1) {
        // Espera tranquila 10 segundos (cede el control)
        vTaskDelay(pdMS_TO_TICKS(10000));
        
        printf("\n┌─────────────────────────────────────────────────────────────┐\n");
        printf("│  ¡ALTA PRIORIDAD ACTIVADA!                                  │\n");
        printf("│  Las tareas LED y Contador quedarán CONGELADAS 5 segundos.  │\n");
        printf("└─────────────────────────────────────────────────────────────┘\n\n");
        
        // Bucle de bloqueo: NO llama a vTaskDelay, por lo que el planificador
        // no puede cambiar a tareas de menor prioridad (inanición).
        for (int i = 0; i < 5; i++) {
            printf("  >>> Tarea bloqueante ejecutando paso %d/5 <<<\n", i + 1);
            
            // Espera activa (mala práctica, pero excelente para enseñar)
            for (volatile unsigned long j = 0; j < 50000000; j++) {
                // Consumir ciclos de CPU
            }
        }
        
        printf("\n┌─────────────────────────────────────────────────────────────┐\n");
        printf("│  BLOQUEO FINALIZADO. Reanudando tareas normales.            │\n");
        printf("└─────────────────────────────────────────────────────────────┘\n\n");
    }
}

// Hooks requeridos por el port Posix (no se usan en esta demo)
void vApplicationIdleHook(void) {}
void vApplicationMallocFailedHook(void) {}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {}
