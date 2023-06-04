#include <stdio.h>
#include <string.h>

// Consola en español
#include <locale.h>
#include <wchar.h>

// Archivos locales
#include "datos.h"

const int sizes[] = {9, 13, 19};
const int numeroTableros = sizeof(sizes) / sizeof(sizes[0]);

int selectTablero();
void obtenerNombre();

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
}

int main() {
    setup();

    printf("##################################\n");
    printf("#               GO               #\n");
    printf("##################################\n");
    printf("\n");

    obtenerNombre();

    printf("Hola %s!", obtenerDatos().nombre);
    printf("\n");

    const int size = selectTablero();
    wprintf(L"Tamaño seleccionado: %dx%d\n", size, size);
    scanf("%d", &size);
    return 0;
}

void obtenerNombre() {
    if (strlen(obtenerDatos().nombre)) return;

    wprintf(L"¿Cuál es tu nombre de usuario? Máximo %d caracteres alfanuméricos.\n", NOMBRE_MAX);
    char nombre[NOMBRE_MAX + 1];
    fgets(nombre, NOMBRE_MAX, stdin);
    strtok(nombre, "\n");
    guardarNombre(nombre);
}

int selectTablero() {
    wprintf(L"Seleccione el tamaño del tablero.\n");
    for (int i = 0; i < numeroTableros; i++) {
        const int size = sizes[i];
        printf("%d. %dx%d\n", i + 1, size, size);
    }

    int size;
    wprintf(L"Ingrese el tamaño aquí: ");
    scanf("%d", &size);

    while (size < 1 || size > numeroTableros) {
        wprintf(L"Tamaño inválido. Intente de nuevo: ");
        scanf("%d", &size);
    }

    return sizes[size - 1];
}
