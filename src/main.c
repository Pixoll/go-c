#include <stdio.h>
#include <string.h>

// Consola en español
#include <locale.h>
#include <wchar.h>

// Archivos locales
#include "datos.h"
#include "util.h"

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

    printf("Hola %s!", obtenerConfig().nombre);
    printf("\n");

    const int size = selectTablero();
    wprintf(L"Tamaño seleccionado: %dx%d\n", size, size);
    scanf("%d", &size);
    return 0;
}

void obtenerNombre() {
    if (strlen(obtenerConfig().nombre)) return;

    wprintf(L"¿Cuál es tu nombre de usuario? Máximo %d caracteres (alfanuméricos y guion bajo).\n", NOMBRE_MAX);
    char nombre[NOMBRE_MAX + 1];
    strget(nombre, NOMBRE_MAX + 1);

    while (!validarNombre(nombre)) {
        wprintf(L"Nombre inválido. Intenta de nuevo: ");
        strget(nombre, NOMBRE_MAX + 1);
    }

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
