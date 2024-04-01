#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Función de comparación para qsort
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s archivo_entrada archivo_salida\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    // Abrir el archivo de entrada
    int fd_input = open(input_file, O_RDONLY);
    if (fd_input == -1) {
        perror("Error al abrir el archivo de entrada");
        return 1;
    }

    // Leer todas las palabras del archivo de entrada
    char buffer[BUFFER_SIZE];
    char *words[BUFFER_SIZE];
    int num_words = 0;
    ssize_t bytes_read;
    while ((bytes_read = read(fd_input, buffer, BUFFER_SIZE)) > 0) {
        char *token = strtok(buffer, " \n\t"); // Separar palabras por espacio, nueva línea o tabulación
        while (token != NULL) {
            words[num_words++] = strdup(token);
            token = strtok(NULL, " \n\t");
        }
    }
    close(fd_input);

    // Ordenar las palabras alfabéticamente
    qsort(words, num_words, sizeof(char *), compare);

    // Abrir el archivo de salida
    int fd_output = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_output == -1) {
        perror("Error al abrir el archivo de salida");
        return 1;
    }

    // Escribir las palabras ordenadas en el archivo de salida
    for (int i = 0; i < num_words; i++) {
        write(fd_output, words[i], strlen(words[i]));
        write(fd_output, "\n", 1); // Agregar un salto de línea después de cada palabra
        free(words[i]); // Liberar la memoria asignada a cada palabra
    }
    close(fd_output);

    return 0;
}
