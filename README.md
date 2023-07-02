# Go en C

Adaptación del juego de mesa "Go" en C.

## Compilación

Para compilar debes tener instalado `gcc` en tu sistema (>= 10.0.0 recomendado). Puedes verificar si lo tienes instalado al ejecutar `gcc -v`. En caso que no lo tengas instalado, dirígete a [Installing GCC: Binaries - GNU Project](https://gcc.gnu.org/install/binaries.html) e instala la versión de GCC correspondiente a tu sistema antes de continuar.

Una vez hayas instalado GCC, debes instalar SDL2 y los paquetes de imágenes, audio y fuente: `libsdl2-2.0-0 libsdl2-doc libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-ttf-2.0-0`.

Si lo tienes instalado, simplemente ejecuta el archivo `build.sh` (probado en Windows y Linux), o `build.bat` (en Windows, es más rápido que `build.sh` por alguna razón) y compilará el código automáticamente. El ejecutable del juego tiene el nombre de `go.exe` por defecto.
