
//INTEGRANTE:
//Jhon Sebastian Usuga Ferraro Cc: 1038337309
// Laboratorio 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Para usar la estructura stat


typedef struct LineNode {
    char *line;
    struct LineNode *next;
} LineNode;

// Inserta una línea al inicio de la lista
LineNode* prepend_line(LineNode *head, const char *line) {
    LineNode *node = (LineNode *)malloc(sizeof(LineNode));
    if (node == NULL) {
        perror("Error: malloc falló");
        exit(EXIT_FAILURE);
    }
    node->line = strdup(line);
    if (node->line == NULL) {
        perror("Error: strdup falló");
        exit(EXIT_FAILURE);
    }
    node->next = head;
    return node;
}

// Libera la memoria de la lista de líneas
void destroy_lines(LineNode *head) {
    while (head != NULL) {
        LineNode *temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}

// Lee una línea de longitud variable
ssize_t get_line(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Tamaño inicial del buffer
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            perror("Error: malloc falló");
            exit(EXIT_FAILURE);
        }
    }
    return getline(lineptr, n, stream);
}

// Verifica si dos archivos son el mismo (hardlinked)
int files_are_identical(const char *file1, const char *file2) {
    struct stat stat1, stat2;

    if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0) {
        perror("Error: No se pudo acceder a los archivos");
        exit(EXIT_FAILURE);
    }
    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino);
}

// Función principal
int main(int argc, char *argv[]) {
    FILE *input = stdin;  // Archivo de entrada predeterminado
    FILE *output = stdout; // Archivo de salida predeterminado
    LineNode *lines = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;

    // Manejo de argumentos
    if (argc > 3) {
        fprintf(stderr, "Uso: reverse <input> <output>\n");
        exit(EXIT_FAILURE);
    }

    // Abrir archivo de entrada si se proporciona
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "reverse: No se puede abrir el archivo '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    // Abrir archivo de salida si se proporciona
    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "reverse: No se puede abrir el archivo '%s'\n", argv[2]);
            if (input != stdin) fclose(input);
            exit(EXIT_FAILURE);
        }

        // Verificar si los archivos de entrada y salida son el mismo
        if (strcmp(argv[1], argv[2]) == 0 || files_are_identical(argv[1], argv[2])) {
            fprintf(stderr, "reverse: El archivo de entrada y salida deben ser diferentes\n");
            fclose(input);
            fclose(output);
            exit(EXIT_FAILURE);
        }
    }

    // Leer el archivo de entrada línea por línea y agregar a la lista
    while (get_line(&buffer, &buffer_size, input) != -1) {
        lines = prepend_line(lines, buffer);
    }

    // Imprimir las líneas en orden inverso
    for (LineNode *current = lines; current != NULL; current = current->next) {
        fprintf(output, "%s", current->line);
    }

    // Limpiar recursos
    destroy_lines(lines);
    free(buffer);
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
