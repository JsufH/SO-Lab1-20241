
//INTEGRANTE:
//Jhon Sebastian Usuga Ferraro Cc: 1038337309
// Laboratorio 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrar_uso_y_salir() {
    fprintf(stderr, "uso: reverse <entrada> <salida>\n");
    exit(1);
}

// Invertir las líneas del archivo de entrada y escribirlas en la salida
void invertir_lineas(FILE *entrada, FILE *salida) {
    char **lineas = NULL;  
    size_t num_lineas = 0;  
    char *linea = NULL;
    size_t longitud = 0;

    // Leer cada línea y almacenarla en el array
    while (getline(&linea, &longitud, entrada) != -1) {
        lineas = realloc(lineas, sizeof(char*) * (num_lineas + 1));
        if (!lineas) {
            fprintf(stderr, "fallo en malloc\n");
            exit(1);
        }
        lineas[num_lineas] = strdup(linea);
        if (!lineas[num_lineas]) {
            fprintf(stderr, "fallo en malloc\n");
            exit(1);
        }
        num_lineas++;
    }
    free(linea);

    // Imprimir las líneas en orden inverso
    for (ssize_t i = num_lineas - 1; i >= 0; i--) {
        fprintf(salida, "%s", lineas[i]);
        free(lineas[i]);
    }

    free(lineas);
}

int main(int argc, char *argv[]) {
    FILE *entrada = stdin;
    FILE *salida = stdout;

    // Validar el número de argumentos
    if (argc > 3) {
        mostrar_uso_y_salir();
    }

    // Si se pasan 2 archivos 
    if (argc == 3) {
        // Verificar si el archivo de entrada y salida son el mismo
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "El archivo de entrada y salida deben diferir\n");
            exit(1);
        }

        // Abrir los archivos de entrada y salida
        entrada = fopen(argv[1], "r");
        if (!entrada) {
            fprintf(stderr, "error: no se puede abrir el archivo '%s'\n", argv[1]);
            exit(1);
        }
        salida = fopen(argv[2], "w");
        if (!salida) {
            fprintf(stderr, "error: no se puede abrir el archivo '%s'\n", argv[2]);
            fclose(entrada);
            exit(1);
        }
    }
    // Si se pasa solo un archivo
    else if (argc == 2) {
        entrada = fopen(argv[1], "r");
        if (!entrada) {
            fprintf(stderr, "error: no se puede abrir el archivo '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Invertir las líneas del archivo de entrada
    invertir_lineas(entrada, salida);

    // Cerrar los archivos
    if (entrada != stdin) fclose(entrada);
    if (salida != stdout) fclose(salida);

    return 0;
}

