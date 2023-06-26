#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"


GoPartida partida;

#define CELDA_J 'X'
#define CELDA_M 'O'

bool ocupadas[19][19];

void puntajePorCantidad();
void puntajePorArea();
void expandir();
void verificarArea(int total);

void crearTablero(int size) {
    partida.size = size;
    partida.puntajeJugador = 0;
    partida.puntajeOponente = 0;
    const int max = size;

    partida.tablero[1][1] = '+';
    partida.tablero[1][max] = '+';
    partida.tablero[max][max] = '+';
    partida.tablero[max][1] = '+';

    for (int i = 2; i < max; i++) {
        partida.tablero[max][i] = '+';
        partida.tablero[1][i] = '+';
        partida.tablero[i][1] = L'¦';
        partida.tablero[i][max] = L'¦';
        for (int j = 2; j < max; j++)
            partida.tablero[i][j] = L'¦';
    }
}

void printTablero() {
    const int size = partida.size;
    for (int i = 1; i < size + 1; i++) {
        for (int j = 1; j < size + 1; j++)
            printf(j + 1 < size + 1 ? "%c-" : "%c", partida.tablero[i][j]);
        printf("\n");
    }
    printf("\n");
}

void jugarTablero() {
    printTablero();
    const int size = partida.size;
    int turno = 0;
    partida.turnoNegras = true;
    for (int i = 1; i < size + 1; i++)
        for (int j = 1; j < size + 1; j++)
            ocupadas[i][j] = false;

    while (turno < 100) {  // condición de victoria
        int x, y;
        printf("\tTurno de %s\n\n", partida.turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        while (ocupadas[x][y] == true) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
        }
        if(x==9 && y==9){ // sirve para terminar la partida (no sabia si habia otra forma)
            break;
        }

        
        ocupadas[x][y] = true;
        partida.tablero[x][y] = partida.turnoNegras ? CELDA_J : CELDA_M;


        //Checkeo de capturas
        capturas(CELDA_J, CELDA_M);
        capturas(CELDA_M, CELDA_J);
        regreso_normal();

        
        limpiarConsola();
        printTitulo();
        printTablero();



        partida.turnoNegras = !partida.turnoNegras;
        turno++;
    }
    partida.terminada = true;
    partida.fecha = now();

    puntajePorCantidad();
    guardarPartida(partida);
}

void puntajePorCantidad() {
    for (int i = 1; i < partida.size + 1; i++) {
        for (int j = 1; j < partida.size + 1; j++) {
            const wchar_t celda = partida.tablero[i][j];
            if (celda == CELDA_J) partida.puntajeJugador++;
            if (celda == CELDA_M) partida.puntajeOponente++;
        }
    }
    puntajePorArea();
}

void puntajePorArea(){   // funcion que pasara por todos los puntos y buscara areas vacias
    int c=0;
    for(int i=1; i<partida.size+1; i++){
        for(int j=1; j<partida.size+1; j++){
            if ((partida.tablero[i][j]=='+') || (partida.tablero[i][j]==L'¦')){
                partida.tablero[i][j]='#';  //marca la primera zona vacia caracterizandole con el caracter #
                expandir();
            }
        }
    }
}

void expandir(){   // en base al area vacia inicial busca todas las areas vacias contiguas y las caracteriza con # 
    int c=1;       // contador que nos servira para saber cuando se dejan de encontrar areas vacias contiguas
    int total=1;   // cantidad de espacios vacios encerrados dentro de un area 
    while (c>0){  // ciclo que expande el area inicial cambiando las areas vacias contiguas por #, 
                  //se repite siempre que se haya expandido una ultima vez (c>0)
        c=0;
        for(int i=1; i<partida.size+1; i++){   
            for(int j=1; j<partida.size+1; j++){
                if (partida.tablero[i][j]=='#'){
                    if (partida.tablero[i][j-1]=='+' || partida.tablero[i][j-1]==L'¦'){
                        partida.tablero[i][j-1]='#';
                        c++;
                        total++;
                    }
                    if (partida.tablero[i-1][j]=='+' || partida.tablero[i-1][j]==L'¦'){
                        partida.tablero[i-1][j]='#';
                        c++;
                        total++;
                    }
                    if (partida.tablero[i+1][j]=='+' || partida.tablero[i+1][j]==L'¦'){
                        partida.tablero[i+1][j]='#';
                        c++;
                        total++;
                    }
                    if (partida.tablero[i][j+1]=='+' || partida.tablero[i][j+1]==L'¦'){
                        partida.tablero[i][j+1]='#';
                        c++;
                        total++;
                    }
                }
            }
        }
    }
    verificarArea(total);
    for(int i=1; i<partida.size+1; i++){ //cambia el area de # por @ para que esta no se vuelva a analizar nuevamente
        for(int j=1; j<partida.size+1; j++){
            if (partida.tablero[i][j]=='#'){
                partida.tablero[i][j]='@';
            }
        }
    }
}

void verificarArea(int total){  // analizamos el area de # verificando si solo la rodea un tipo de ficha o no
    int negras=0;
    int blancas=0;
    int otro=0;
    for(int i=1; i<partida.size+1; i++){ // cicloq ue contabiliza los tipos de zonas que rodean el area de #
        for(int j=1; j<partida.size+1; j++){ 
            if (partida.tablero[i][j]=='#'){
                if (partida.tablero[i+1][j]==CELDA_NEGRA || partida.tablero[i-1][j]==CELDA_NEGRA || partida.tablero[i][j+1]==CELDA_NEGRA ||partida.tablero[i][j-1]==CELDA_NEGRA){
                    negras++;
                }
                if (partida.tablero[i+1][j]==CELDA_BLANCA || partida.tablero[i-1][j]==CELDA_BLANCA || partida.tablero[i][j+1]==CELDA_BLANCA ||partida.tablero[i][j-1]==CELDA_BLANCA){
                    blancas++;
                }
                if (partida.tablero[i+1][j]!=CELDA_BLANCA && partida.tablero[i-1][j]!=CELDA_BLANCA && partida.tablero[i][j+1]!=CELDA_BLANCA && partida.tablero[i][j-1]!=CELDA_BLANCA 
                && partida.tablero[i+1][j]!=CELDA_NEGRA && partida.tablero[i-1][j]!=CELDA_NEGRA && partida.tablero[i][j+1]!=CELDA_NEGRA && partida.tablero[i+j][j-1]!=CELDA_NEGRA){
                    otro++;
                }
            }
        }
    }
    if (negras!=0 && blancas==0 && otro<negras*2 ) { // si el area es rodeada solo por fichas negras o otro le asigna puntaje a jugador blanco
                                    
        partida.puntajeJugador=partida.puntajeJugador+total;
    }
    else if (blancas!=0 && negras==0 && otro<blancas*2 ) { // lo mismo con fichas blancas y con puntos al jugador blanco
                                    
        partida.puntajeOponente=partida.puntajeOponente+total;
    }
    // de no cumplirse ninguna condicion anterior se asume que el area es neutra y no se le otorgara puntaje a ningun jugador
    // mas a delante se pueden implementar mas condiciones de area neutra haciendo las condiciones del juego mas realista
}

// si quieren probar la contabilizacion de puntajes en una condicion un tanto extrema seleccione jugar y el tablero de 9x9 (1) y coloque
// 1 2 3 1 2 2 4 2 2 3 5 2 2 4 6 2 3 5 7 3 4 4 7 4 4 3 7 5 5 3 7 6 6 3 8 6 6 4 9 6 6 5 4 8 6 6 5 7 5 6 5 9 4 7 6 7 3 7 6 9 2 7 7 8 1 7 





//cva

//busca y recorre los grupos de fichas, los marca como que tienen o no libertades
void DFS(int posX, int posY, char celdaJugador, char celdaOponente, bool visitado[19][19], bool *tieneLibertades, int grupo[19][19]) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    visitado[posX][posY] = true;
    grupo[posX][posY] = 1;
    
    for (int i = 0; i < 4; i++) {
        int nx = posX + dx[i];
        int ny = posY + dy[i];

        // verifica si las fichas vecinas son validas
        if (nx >= 1 && nx <= partida.size && ny >= 1 && ny <= partida.size) {
            // verifica si hay espacios en blanco en las fichas vecinas
            if (partida.tablero[nx][ny] == '+' || partida.tablero[nx][ny] == L'¦') {
                *tieneLibertades = true;
            }
            else if (partida.tablero[nx][ny] == celdaJugador && !visitado[nx][ny]) {
                DFS(nx, ny, celdaJugador, celdaOponente, visitado, tieneLibertades, grupo);
            }
        }
    }
}




//analiza el tablero y los grupos de fichas que se forman, usa dfs para recorrer los grupos y ve si estan o no capturados
void capturas(char celdaJugador, char celdaOponente) {
    bool visitado[19][19];
    int grupo[19][19];
    
    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
            visitado[i][j] = false;
            grupo[i][j] = 0;
        }
    }
    
    for (int posX = 1; posX <= partida.size; posX++) {
        for (int posY = 1; posY <= partida.size; posY++) {
            if (partida.tablero[posX][posY] == celdaJugador && !visitado[posX][posY]) {
                bool tieneLibertades = false;
                DFS(posX, posY, celdaJugador, celdaOponente, visitado, &tieneLibertades, grupo);
                
                // si un grupo de fichas del jugador no tiene libertades
                if (!tieneLibertades) {
                    for (int i = 1; i <= partida.size; i++) {
                        for (int j = 1; j <= partida.size; j++) {
                            if (grupo[i][j] == 1) {
                                partida.tablero[i][j] = celdaOponente == CELDA_J ? '2' : '1';  // captura X=1, O=2
                                ocupadas[i][j] = false;  // actualiza el estado de 'ocupado' 
                                grupo[i][j] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}


//saca las fichas capturadas y regresa el estado del tablero a X, O y espacios en blanco
void regreso_normal(){
    for (int posX = 1; posX <= partida.size; posX++){
        for (int posY = 1; posY <= partida.size; posY++){
            if (partida.tablero[posX][posY] == '2'){
                partida.tablero[posX][posY] = '+';
            }
            if (partida.tablero[posX][posY] == '1'){
                partida.tablero[posX][posY] = '+';
                ocupadas[posX][posY] = false;
            }
        }
    }
}






