#pragma once

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "CosmicDefender.h";
using namespace std;

// Variable que determina que tan frecuentemente se puede tomar daño nuevamente tras un impacto
#define cooldownImpacto 1.3


// Se van a definir las estructuras requeridas para el juego
// Reprenta los disparos que da el jugador
typedef struct bala {
	int ejeX;
	int ejeY;
	bool impacto;
	bala* siguiente = NULL;

};
// Jugador, representará la nave que se mueve con el teclado
typedef struct jugador {
	int ejeX;
	int ejeY;
	int vidas;
	bala* listaBalas = NULL;
};
// Sirve para representar meteoros y aliens 
// Tendrán una variable de bitmap para poder identificar como se deben de dibujar dentro del juego
typedef struct enemigo {
	int ejeX;
	int ejeY;
	bool impacto;
	ALLEGRO_BITMAP* skin;
	enemigo* siguienteE = NULL;

};

// Estructura Tubo
// Sirve para hacer un obstaculo a través de la pantalla
typedef struct tubo {

	int ejeY;

	int largoTubo;

	// Determina en que dirección se extenderá el tubo
	bool derecha;

};

// Funciones del Juego
// 
// Funciones para el manejo de Listas enlazadas
// Para agregar un elemento a una lista

// A una lista de enemigos
void AgregarListaEnemigos(enemigo* & listaE, int X, int Y, ALLEGRO_BITMAP* skinE) {

	// Si la lista esta vacía
	if (listaE == NULL) {

		// Se le asigna una estructura enemigo
		listaE = new enemigo();

		// Se definen sus datos según los recibidos en la función
		listaE->ejeX = X;
		listaE->ejeY = Y;
		listaE->impacto = false;
		listaE->skin = skinE;

		// Se pone el siguiente elemento como nulo
		listaE->siguienteE = NULL;
	}

	// Si la lista no esta vacía
	else {

		// Se crea un puntero que apunte al primer elemento en la lista
		// Sirve para no perder la dirección en memoria
		enemigo* aux = listaE;

		while (aux->siguienteE != NULL) {
			aux = aux->siguienteE;
		}
		aux->siguienteE = new enemigo();

		// Se definen sus datos según los recibidos en la función
		aux->siguienteE->ejeX = X;
		aux->siguienteE->ejeY = Y;
		aux->siguienteE->impacto = false;
		aux->siguienteE->skin = skinE;

		// Se pone el siguiente elemento como nulo
		aux->siguienteE->siguienteE = NULL;
	}

}

// A una lista de balas
void AgregarListaBalas(bala*& listaB, int X, int Y) {

	// Se revisa si la lista de balas esta vacía
	if (listaB == NULL) {

		// Se asigna simplemente un elemento nuevo al puntero
		listaB = new bala();
		listaB->siguiente = NULL;

	}
	else {

		// Se crea un puntero para guardar la dirección a la que apunta la lista
		bala* aux = listaB;

		// Se le asigna una nueva bala a la lista
		listaB = new bala();

		// Se le asigna la dirección anteriormente guardada al siguiente de la bala que se acaba de crear
		listaB->siguiente = aux;

	}

	// Se definen sus atributos según los parámetros ingresados
	listaB->ejeX = X;
	listaB->ejeY = Y;
	listaB->impacto = false;

}


// Funciones para borrar elementos de una lista enlazada

// Borrar enemmigos
// Esta función busca por enemigos con variables de impacto marcadas true y los elimina de la lista
void eliminarEnemigosDestruidos(enemigo*& listaE) {

	// Punteros para poder recorrer la lista
	enemigo* actual = listaE;
	enemigo* elementoAnterior = NULL;

	// Mientras el siguiente del enemigo actual no sea NULL (No se haya llegado al último elemento)
	while (actual!= NULL) {

		// Si se esta en el primer elemento de la lista
		if (elementoAnterior == NULL) {

			// Si el primer elemento tuvo un impacto
			if (actual->impacto) {

				// Se mueve el puntero de la lista hacia adelante
				listaE = listaE->siguienteE;

				// Se borra el elemento al que esta apuntando el actual
				delete(actual);

				// Se pone al puntero actual a apuntar al nuevo primer elemento en la lista
				actual = listaE;
			}
			// Si el primer elemento no tuvo impacto
			else {
				// El elemento anterior se vuelve el actual
				elementoAnterior = actual;

				// El actual pasa al siguiente elemento
				actual = actual->siguienteE;

			}
		}
		// Si ya se paso del primer elemento en la lista
		else {

			// Si el elemento en que se esta tuvo un impacto
			if (actual->impacto) {

				// El elemento pasado en la lista apuntará al siguiente del actual para no perder la dirección en memoria del resto de elementos
				elementoAnterior->siguienteE = actual->siguienteE;

				// Ahora que se guardo la dirección del siguiente y se aislo el elemento a borrar se elimina
				delete(actual);

				// El puntero actual apuntará al siguiente del elemento anterior
				actual = elementoAnterior->siguienteE;

			}
			// Si el elemento actual no tuvo impacto
			else {
				// El elemento anterior se vuelve el actual
				elementoAnterior = actual;

				// El elemento actual se vuelve el siguiente
				actual = actual->siguienteE;
			}

		}
	}
}

// Borrar Balas // Funciona igual que la función anterior, solo que con estructuras bala
// Esta función busca por balas con variables de impacto marcadas true y las elimina de la lista
void eliminarBalasDestruidas(bala*& listaB) {

	// Punteros para poder recorrer la lista
	bala* actual = listaB;
	bala* elementoAnterior = NULL;

	// Mientras el siguiente del enemigo actual no sea NULL (No se haya llegado al último elemento)
	while (actual!= NULL) {

		// Si se esta en el primer elemento de la lista
		if (elementoAnterior == NULL) {

			// Si el primer elemento tuvo un impacto
			if (actual->impacto) {

				// Se mueve el puntero de la lista hacia adelante
				listaB = listaB->siguiente;

				// Se borra el elemento al que esta apuntando el actual
				delete(actual);

				// Se pone al puntero actual a apuntar al nuevo primer elemento en la lista
				actual = listaB;
			}
			// Si el primer elemento no tuvo impacto
			else {
				// El elemento anterior se vuelve el actual
				elementoAnterior = actual;

				// El actual pasa al siguiente elemento
				actual = actual->siguiente;

			}
		}
		// Si ya se paso del primer elemento en la lista
		else {

			// Si el elemento en que se esta tuvo un impacto
			if (actual->impacto) {

				// El elemento pasado en la lista apuntará al siguiente del actual para no perder la dirección en memoria del resto de elementos
				elementoAnterior->siguiente = actual->siguiente;

				// Ahora que se guardo la dirección del siguiente y se aislo el elemento a borrar se elimina
				delete(actual);

				// El puntero actual apuntará al siguiente del elemento anterior
				actual = elementoAnterior->siguiente;

			}
			// Si el elemento actual no tuvo impacto
			else {
				// El elemento anterior se vuelve el actual
				elementoAnterior = actual;

				// El elemento actual se vuelve el siguiente
				actual = actual->siguiente;
			}

		}
	}
}

// 
// Función para inicializar una lista enlazada de enemigos
// 
// Se va a recibir una variable booleana, en caso de que sea true se va a inicializar una lista de meteoros (minas) , de lo contrario es una lista de enemigos Alien
// También se reciben los arreglos con los diferentes tipos de Bitmap para cada enemigo para poder asignarles una skin
//
void inicializarlistaEnemigos(enemigo*& lista, bool Mina, ALLEGRO_BITMAP* enemigos[2], ALLEGRO_BITMAP* Meteoros[3]) {

	// Se revisa el parámetro Mina
	if (Mina) {

		// Se van a colocar minas alrededor de toda la pantalla inicialmente
		// Para esto se va a ir creando meteoros sobre el eje X hasta llegar al límite de la pantalla

		int ejeX = 320;
		while (ejeX <= 1890) {

			// Se agrega un enemigo con ese eje X y en un eje en Y aleatorio
			// También tendrá una imágen aleatoria del arreglo de meteoros
			AgregarListaEnemigos(lista, ejeX, rand() % 750, Meteoros[rand() % 3]);
			ejeX += 100;

		}

	}

	// Si no se esta trabajando con minas
	else {
		// Se va a determinar la cantidad de enemigos a desplegar de forma aleatoria (Cantidad máxima inicial = 5)
		int cantidad_enemigos = rand() % 8;

		// Se va  a definir 2 como la cantidad mínima de enemigos con los que comenzar la lista
		if (cantidad_enemigos < 4) {
			cantidad_enemigos = 4;
		}

		// Se va a hacer un ciclo que agregue minas a la lista hasta llegar a la cantidad deseada
		for (int i = 0; i < cantidad_enemigos; i++) {


			// Se genera en un X aleatorio dentro de los límites del juego
			// Siempre empiezan desde la sección superior por lo que comienzan en Y = 0;

			// Para definir el X
			int ejeX = rand() % 1890;
			if (ejeX < 320) {
				ejeX = 320;
			}

			// Para iniciar el eje Y de los enemigos
			int ejeY = -1 * (rand() % 500);

			AgregarListaEnemigos(lista, ejeX, ejeY, enemigos[rand() % 2]);

		}
	}
}

//Función para  desplazar los enemigos
void desplazarEnemigos(enemigo*& listaEnemigos, bool minas, ALLEGRO_BITMAP* ImagenesEnemigos[2], ALLEGRO_BITMAP* ImagenesMeteos[3]) {

	// Se crea un puntero para recorrer la lista
	enemigo* aux = listaEnemigos;

	// Mientras no se termine la lista de enemigos se seguirá desplazando elementos
	while (aux != NULL) {

		if (minas) {
			// Se le suma la distancia al enemigo
			aux->ejeY += 2;

			// Se verifica si el enemigo se sale del límite
			if (aux->ejeY >= 1030) {
				aux->impacto = true;

				// Como se eliminará ese enemigo se va a agregar 1 en ese mismo eje de X

				AgregarListaEnemigos(listaEnemigos, aux->ejeX,(-1*(rand()%500)), ImagenesMeteos[rand() % 3]);
	
			}

		}
		else {
			aux->ejeY += 5;

			// Se verifica si el enemigo se sale del límite
			if (aux->ejeY >= 1030) {
				aux->impacto = true;

				// Como se eliminará ese enemigo se va a agregar aleatoriamente 0, 1 o 2 enemigos nuevos

				int nuevosAgregar = rand() % 3;

				for (int i = 0; i < nuevosAgregar; i++) {

					// Para definir el X
					int ejeX = rand() % 1890;
					if (ejeX < 320) {
						ejeX = 320;
					}

					AgregarListaEnemigos(listaEnemigos, ejeX, (-1 * (rand() % 500)), ImagenesEnemigos[rand() % 2]);
				}
			}
		}
		aux = aux->siguienteE;
	}
}


// Función dibujar enemigos 
// Esta función va a tomar todos los enemigos y meteoros y los dibujará dentro de la ventana del juego

void pintarObstaculos(enemigo* listaEnemigos, enemigo* listaMeteoros) {

	// Se crea un puntero auxiliar para pintar todos los obstáculos
	// Va comenzar dibujando la lista de enemigos
	enemigo* aux = listaEnemigos;
	// Mientras no se termine la lista de enemigos se seguirá pintando
	while (aux != NULL) {

		// Se obtienen los datos de la imágen
		int ancho = al_get_bitmap_width(aux->skin);
		int largo = al_get_bitmap_height(aux->skin);

		al_draw_scaled_bitmap(aux->skin, 0, 0, ancho, largo, aux->ejeX, aux->ejeY, 120, 100,NULL);

		aux = aux->siguienteE;
	}

	// Luego se dibuja la lista de meteoros
	aux = listaMeteoros;

	//Mientras no se termine la lista de enemigos se seguirá pintando
	while (aux != NULL) {

		// Se obtienen los datos de la imágen
		int ancho = al_get_bitmap_width(aux->skin);
		int largo = al_get_bitmap_height(aux->skin);

		al_draw_scaled_bitmap(aux->skin, 0, 0, ancho, largo, aux->ejeX, aux->ejeY, 120, 100, NULL);
		aux = aux->siguienteE;
	}

}

// Función para desplazar las balas disparadas por el jugador
void desplazarBalas(bala*& ListaBalas) {
	
	bala* aux = ListaBalas;

	// Mientras no se termine la lista
	while (aux != NULL) {

		// Se sube la bala
		aux->ejeY -= 9;

		// Si la bala ya se sale del limite visible de la pantalla
		if (aux->ejeY == 50) {
			aux->impacto = true;
		}

		aux = aux->siguiente;

	}
}

// Función para pintar las balas disparadas por el jugador
void pintarBalas(bala* listaBalas, ALLEGRO_BITMAP* imagenBala) {

	bala* aux = listaBalas;

	// Mientras no se termine la lista
	while (aux != NULL) {
	
		// Se obtienen los datos de la imágen de bala
		int ancho = al_get_bitmap_width(imagenBala);
		int largo = al_get_bitmap_height(imagenBala);

		// Se dibuja la imagen de forma más pequeÑa
		al_draw_scaled_bitmap(imagenBala, 0, 0, ancho, largo, aux->ejeX, aux->ejeY, 60, 60, NULL);

		aux = aux->siguiente;

	}


}

// Funciones para revisar si un enemigo o meteoro choca con el jugador
bool revisarImpactoJugador(jugador &Player, enemigo*&ListaEnemigos, bool minas, float &ultimoImpacto, ALLEGRO_BITMAP* IJugador, ALLEGRO_BITMAP* enemigos[2], ALLEGRO_BITMAP* Meteos[3], ALLEGRO_SAMPLE* sonidoExplosion){

	// Se van a revisar los enemigos
	enemigo* actual = ListaEnemigos;

	while (actual != NULL) {
		
		// Si el enemigo choca con el hitbox del jugador
		if (actual->ejeX <= Player.ejeX + 40 && actual->ejeX >= Player.ejeX - 40 && actual->ejeY >= Player.ejeY - 90 && actual->ejeY <= Player.ejeY + 90) {

			// Se revisa si ya paso el tiempo de cooldown tras un impacto
			if (((clock() - ultimoImpacto) / CLOCKS_PER_SEC) > cooldownImpacto) {

				al_play_sample(sonidoExplosion, 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				if (Player.vidas - 1 == 0) {
					return true;
				}

				ultimoImpacto = clock();

				// El jugador pierde una vida
				Player.vidas -= 1;
				
				// Ese objeto tiene un impacto
				actual->impacto = true;

				// Si no se esta trabajando con minas
				if (!minas){
					
					// Al chocar con el enemigo se van a generar aleatoriamente entre 0 y 2 enemigos nuevos
					for (int i = 0; i < (rand() % 3); i++) {
						int ejeX = rand() % 1890;
						if (ejeX < 320) {
							ejeX = 320;
						}
						AgregarListaEnemigos(ListaEnemigos, ejeX, (-1 * (rand() % 500)), enemigos[rand() % 2]);
					}
				}
				else {

					// Al chocar con el enemigo se van a generar aleatoriamente entre 0 y 2 enemigos nuevos
					for (int i = 0; i < (rand() % 3); i++) {

						int ejeX = rand() % 1890;
						if (ejeX < 320) {
							ejeX = 320;
						}
						AgregarListaEnemigos(ListaEnemigos, ejeX, (-1 * (rand() % 500)), Meteos[rand() % 3]);
					}
				}

				// Se obtienen los datos de la imágen
				int ancho2 = al_get_bitmap_width(IJugador);
				int largo2 = al_get_bitmap_height(IJugador);

				// Se dibuja la imagen parpadeando rojo para representar el impacto
				al_draw_tinted_scaled_bitmap(IJugador, al_map_rgb(250, 0, 0), 0, 0, ancho2, largo2, Player.ejeX, Player.ejeY, 130, 130,NULL);

			}
		}
		actual = actual->siguienteE;
	}
	return false;
}

// Para revisar los impactos de balas con enemigo

void revisarImpactoEnemigo(bala* &listaBalas, enemigo* &ListaEnemigos, enemigo* &ListaMeteoros, int &contadorEnemigoD, int& contadorMeteoroD, int &Puntaje, ALLEGRO_BITMAP* enemigos[2], ALLEGRO_BITMAP* Meteos[3], ALLEGRO_SAMPLE* sonidoExplosion) {

	// Se va a crear un puntero que va a servir para recorrer todas las balas en la lista
	bala* actualB = listaBalas;

	// Puntero para recorrer todos los enemigos y meteoros
	enemigo* actualE;

	// Hasta que terminen las balas en la lista
	while (actualB != NULL) {

		// Para revisar la lista de enemigos
		actualE = ListaEnemigos;

		while (actualE != NULL) {

			// Si la bala esta dentro de las coordenadas del enemigo
			if (actualB->ejeX <= actualE->ejeX + 70 && actualB->ejeX >= actualE->ejeX - 70 && actualB->ejeY >= actualE->ejeY - 90 && actualB->ejeY <= actualE->ejeY + 90) {

				// Se eliminarán ambos objetos y termina el ciclo para esa bala
				contadorEnemigoD++;
				Puntaje += 50;
				actualE->impacto = true;
				actualB->impacto = true;
				al_play_sample(sonidoExplosion, 0.1, 0, 1.3, ALLEGRO_PLAYMODE_ONCE, NULL);

				int ejeX = rand() % 1890;
				if (ejeX < 320) {
					ejeX = 320;
				}

				AgregarListaEnemigos(ListaEnemigos, ejeX, (-1 * (rand() % 500)), enemigos[rand() % 2]);
				break;

			}

			actualE = actualE->siguienteE;
		}

		// Para revisar la lista de meteoros
		actualE = ListaMeteoros;

		while (actualE != NULL) {

			// Si la bala esta dentro de las coordenadas del enemigo
			if (actualB->ejeX <= actualE->ejeX + 60 && actualB->ejeX >= actualE->ejeX - 60 && actualB->ejeY >= actualE->ejeY - 80 && actualB->ejeY <= actualE->ejeY + 80) {

				// Se eliminarán ambos objetos y termina el ciclo para esa bala
				contadorMeteoroD++;
				Puntaje += 25;
				actualE->impacto = true;
				actualB->impacto = true;
				al_play_sample(sonidoExplosion, 0.1, 0, 1.3, ALLEGRO_PLAYMODE_ONCE, NULL);

				int ejeX = rand() % 1890;
				if (ejeX < 320) {
					ejeX = 320;
				}

				AgregarListaEnemigos(ListaMeteoros, ejeX, (-1 * (rand() % 500)), Meteos[rand() % 3]);
				break;

			}

			actualE = actualE->siguienteE;
		}

	
		// Se pasa a la siguiente bala
		actualB = actualB->siguiente;
	}
}


// Funciones para trabajar con el tubo obstáculo

// Para dibujar al tubo
void dibujarTubo(tubo Obstaculo, ALLEGRO_BITMAP* partesDelTubo[4]) {

	// Se dibuja el tubo obstáculo dependiendo de la dirección de su extensión
	if (Obstaculo.derecha) {

		int ancho2 = al_get_bitmap_width(partesDelTubo[0]);
		int largo2 = al_get_bitmap_height(partesDelTubo[0]);
		al_draw_scaled_bitmap(partesDelTubo[0], 0, 0, ancho2, largo2, 320, Obstaculo.ejeY, Obstaculo.largoTubo, 200, NULL);

		int ancho3 = al_get_bitmap_width(partesDelTubo[1]);
		int largo3 = al_get_bitmap_height(partesDelTubo[1]);
		al_draw_scaled_bitmap(partesDelTubo[1], 0, 0, ancho3, largo3, Obstaculo.largoTubo + 300, Obstaculo.ejeY + 24, 160, 153, NULL);

		int ancho4 = al_get_bitmap_width(partesDelTubo[2]);
		int largo4 = al_get_bitmap_height(partesDelTubo[2]);
		al_draw_scaled_bitmap(partesDelTubo[2], 0, 0, ancho4, largo4, 1750, Obstaculo.ejeY + 24, 160, 153, NULL);

	}
	else {

		int ancho2 = al_get_bitmap_width(partesDelTubo[3]);
		int largo2 = al_get_bitmap_height(partesDelTubo[3]);
		al_draw_scaled_bitmap(partesDelTubo[3], 0, 0, ancho2, largo2, 1950 - Obstaculo.largoTubo, Obstaculo.ejeY, Obstaculo.largoTubo, 200, NULL);

		int ancho3 = al_get_bitmap_width(partesDelTubo[1]);
		int largo3 = al_get_bitmap_height(partesDelTubo[1]);
		al_draw_scaled_bitmap(partesDelTubo[1], 0, 0, ancho3, largo3, 320, Obstaculo.ejeY + 24, 160, 153, NULL);

		int ancho4 = al_get_bitmap_width(partesDelTubo[2]);
		int largo4 = al_get_bitmap_height(partesDelTubo[2]);
		al_draw_scaled_bitmap(partesDelTubo[2], 0, 0, ancho4, largo4, 1950 - Obstaculo.largoTubo, Obstaculo.ejeY + 24, 160, 153, NULL);
	}

}


// Desplazar Tubo
// Lo mueve hacia abajo y en caso de que se salga del límite lo re inicializa aleatoriamente
void desplazarTubo(tubo &Obstaculo) {

	Obstaculo.ejeY += 1;

	// Si se sale de pantalla se reinicializa
	if (Obstaculo.ejeY >= 1030) {

		// Se reinicia el eje Y del tubo
		Obstaculo.ejeY = -500;

		// Se va a determinar aleatoriamente hacia que lado se extenderá el tubo
		int extension = rand() % 3;

		if (extension < 2) { Obstaculo.derecha = true; }
		else { Obstaculo.derecha = false; }

		if (Obstaculo.derecha) {
			// Se determina el largo del tubo
			Obstaculo.largoTubo = rand() % 1200;
			if (Obstaculo.largoTubo < 800) { Obstaculo.largoTubo = 800; }
		}
		else {
			// Se determina el largo del tubo
			Obstaculo.largoTubo = rand() % 1400;
			if (Obstaculo.largoTubo < 1000) { Obstaculo.largoTubo = 1000; }
		}

	}

}

// Función para revisar si hubo impacto con los tubos
bool revisarImpactoTubo(jugador& Player, tubo Obstaculo, float& ultimoImpact, ALLEGRO_SAMPLE* sonidoIm) {

	if (Obstaculo.derecha) {

		if ((Player.ejeX >= 1750 && (Player.ejeY <= Obstaculo.ejeY + 100 && Player.ejeY >= Obstaculo.ejeY - 100)) || (Player.ejeX <= Obstaculo.largoTubo + 320) && (Player.ejeY <= Obstaculo.ejeY + 100 && Player.ejeY >= Obstaculo.ejeY - 100)) {

			// Se revisa si ya paso el tiempo de cooldown tras un impacto
			if (((clock() - ultimoImpact) / CLOCKS_PER_SEC) > cooldownImpacto) {

				al_play_sample(sonidoIm, 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				if (Player.vidas - 1 == 0) {
					return true;
				}
				Player.vidas--;
				ultimoImpact = clock();

			}
		}
	}
	else {
		if ((Player.ejeX <= 430 && (Player.ejeY <= Obstaculo.ejeY + 100 && Player.ejeY >= Obstaculo.ejeY - 100)) || (Player.ejeX >= 1900 - Obstaculo.largoTubo && (Player.ejeY <= Obstaculo.ejeY + 100 && Player.ejeY >= Obstaculo.ejeY - 100))) {

			// Se revisa si ya paso el tiempo de cooldown tras un impacto
			if (((clock() - ultimoImpact) / CLOCKS_PER_SEC) > cooldownImpacto) {

				if (Player.vidas - 1 == 0) {
					return true;
				}
				Player.vidas--;
				ultimoImpact = clock();
			}
		}
	}
	return false;

}

// Función para guardar datos en un archivo
void guardarDatos(char* nombre, int score, int enemigosD, int minasD) {

	// Se abre el archivo en modo append, por lo tanto no se borrán sus contenidos
	FILE* archivo;
	fopen_s(&archivo,"ResultadosCD.txt", "a");

	// Se revisa si se pudo acceder al archivo
	if (NULL == archivo) {
		fprintf(stderr, "No se pudo crear acceder al archivo ResultadosCD.txt");
		exit(-1);
	}
	// Se escriben los datos del jugador en el final del archivo
	else {
		fprintf(archivo, "Nombre del Jugador: %s\n", nombre);
		fprintf(archivo, "Puntaje Obtenido: %i\n", score);
		fprintf(archivo, "Enemigos Destruidos: %i\n", enemigosD);
		fprintf(archivo, "Minas Destruidas: %i\n", minasD);
		fprintf(archivo, "\n\n");
	}
	// Se cierra el archivo ResultadosCD.txt
	fclose(archivo);
}

// Función para cargar los datos guardados en el archivo ResultadosCD.txt
// Recibe un índice de lectura para saber desde que persona debería comenzar a cargar los datos

void cargarDatos(ALLEGRO_FONT* fuente, int indiceLectura){
	
	// Variables para guardar los datos del archivo
	char nombreJugador[30];
	char puntajeJ[25];
	char enemigoDes[32];
	char minaDes[32];

	// Eje Y para determinar la posición del texto leído
	int ejeY = 300;

	// Indice para recorrer los usuarios en el archivo
	int indiceRecorrido = 0;

	// Se determina el final del rango que se va a leer
	int finalLectura = indiceLectura + 3;

	// Se abre el documento en modo lectura
	FILE* archivo;
	fopen_s(&archivo,"ResultadosCD.txt", "r");

	// Se revisa si el archivo esta vacío
	if (archivo == NULL) {}

	// Si contiene algo
	else
	{
		// Mientras no se llegue al final del arcchivo
		while (!feof(archivo)) {

			// Se leen los datos
			fscanf_s(archivo, "Nombre del Jugador: %s\n", nombreJugador,30);
			fscanf_s(archivo, "Puntaje Obtenido: %s\n", puntajeJ,25);
			fscanf_s(archivo, "Enemigos Destruidos: %s\n", enemigoDes,32);
			fscanf_s(archivo, "Minas Destruidas: %s\n", minaDes,32);

			// Si se esta dentró del rango de los que se deben mostrar
			if (indiceRecorrido >= indiceLectura && indiceRecorrido <= finalLectura) {

				// Para el nombre del jugador
				al_draw_text(fuente, al_map_rgb(250, 250, 250), (1920/2) - 400, ejeY, NULL, "Nombre del Jugador: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), 1920 / 2, ejeY,NULL,nombreJugador);
				ejeY += 150;

				// Para el puntaje
				al_draw_text(fuente, al_map_rgb(250, 250, 250), (1920 / 2) - 400, ejeY, NULL,"Puntaje Obtenido: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), 1920 / 2, ejeY, NULL, puntajeJ);
				ejeY += 150;

				al_draw_text(fuente, al_map_rgb(250, 250, 250), (1920 / 2) - 400, ejeY, NULL, "Enemigos Destruidos: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), 1920 / 2, ejeY, NULL, enemigoDes);
				ejeY += 150;

				al_draw_text(fuente, al_map_rgb(250, 250, 250), (1920 / 2) - 400, ejeY, NULL, "Minas Destruidas: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), 1920 / 2, ejeY, NULL, minaDes);
				ejeY += 250;

			}
			indiceRecorrido++;
		}

	}
	fclose(archivo);
}


// Función para guardar las estadísticas de un jugador en el archivo de Ranking
// Recibe las variables de estadistica y las coloca dentro del archivo dependiendo de si son mayores o menores que el resto
// También reinicia los valores de las variables para el siguiente juego

void ventanaGuardarArchivo(int &score, int& cEnemigoD, int& cMeteoD, ALLEGRO_EVENT_QUEUE* cola, ALLEGRO_FONT* fuente) {
	// Mientras no se cambie esta variable el ciclo de guardado no termina
	int terminar2 = false;

	// Para el puntaje
	string sc = to_string(score);
	char const* ScoreJ = sc.c_str();

	// Se juntan en un solo String el número y el texto
	char puntaje[25] = "Puntaje: ";
	strcat_s(puntaje, 25, ScoreJ);

	// Para los enemigos destruidos
	string ed = to_string(cEnemigoD);
	char const* enemyD = ed.c_str();

	// Se juntan en un solo String el número y el texto
	char enmyD[32] = "Enemigos Destruidos: ";
	strcat_s(enmyD, 32, enemyD);

	// Para la cantidad de minas destruidas
	string md = to_string(cMeteoD);
	char const* MinaD = md.c_str();

	// Se juntan en un solo String el número y el texto
	char MineD[32] = "Minas Destruidas: ";
	strcat_s(MineD, 32, MinaD);

	// Para poder recibir un nombre de parte del usuario
	// Arreglo para guardar el string
	char nJugador[30];

	// El primer caracter del arreglo será " _ " para que parezca que se esta esperando input del usuario
	nJugador[0] = char(95);

	// Para saber cual es el número de letra del nombre en que se esta
	// Si se borra una letra la posición baja y si se escribe una nueva se aumenta
	int numLetras = 0;

	// Se crea un ciclo para esperar eventos
	while (!terminar2) {

		// Se espera un evento
		ALLEGRO_EVENT eventoos;	
		al_wait_for_event(cola, &eventoos);

		// Se limpia la pantalla
		al_clear_to_color(al_map_rgb(0, 0, 0));

		// Si se recibe una letra como input del usuario
		if (eventoos.type == ALLEGRO_EVENT_KEY_CHAR) {

			// Se revisa si la cantidad de letras se excede del límite
			if (numLetras < 30) {

				// Al presionar BACKSPACE se va a borrar la última letra escrita y se reduce la cantidad de letras
				if (eventoos.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {

					// Si hay al menos una letra
					if (numLetras != 0) {

						// Se define la letra en esa posición como NULL
						nJugador[numLetras] = NULL;
						numLetras = numLetras - 1;

					}
				}
				// Se dicta que al presionar las teclas tab y escape no se van a recibir como input ni tampoco servirán de nada
				else if (eventoos.keyboard.keycode == ALLEGRO_KEY_TAB) {
				}
				else if (eventoos.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				}

				//Si se preciona espacio se va a poner una barra baja 
				else if (eventoos.keyboard.keycode == ALLEGRO_KEY_SPACE) {
					nJugador[numLetras] = char(95);
					numLetras = numLetras + 1;
				}

				// Al presionar ENTER termina el ciclo y se guardan los datos
				else if (eventoos.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					terminar2 = true;
				}

				// Si la tecla presionada es diferente de las anteriores se va a detectar como un caracter válido para el nombre
				else {
					// Se define la letra en ese índice como la recibida por el teclado
					nJugador[numLetras] = eventoos.keyboard.unichar;
					numLetras = numLetras + 1;
				}
			}
		}
		// Se dibuja en el display el nombre, puntaje y enemigos destruidos
		al_draw_text(fuente, al_map_rgb(250, 75, 75), 1920 / 2, 1080 / 2 - 450, ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2 - 350, ALLEGRO_ALIGN_CENTRE, puntaje);
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2 - 250, ALLEGRO_ALIGN_CENTRE, enmyD);
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2 - 150, ALLEGRO_ALIGN_CENTRE, MineD);

		al_draw_text(fuente, al_map_rgb(255, 75, 75), 1920 / 2, 1080 / 2, ALLEGRO_ALIGN_CENTRE, nJugador);
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Presione Enter para finalizar");
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2 - 50, ALLEGRO_ALIGN_CENTRE, "Ingrese su nombre");

		// Se dibuja el nombre que va  ingresando el usuario
		if (nJugador[0] == char(95)) {
			al_draw_text(fuente, al_map_rgb(255, 255, 255), 1920 / 2, 1080 / 2, ALLEGRO_ALIGN_CENTRE, "_");
		}
		al_flip_display();
	}
	//Si el nombre es vacio se escribe Anon para el nombre
	if (numLetras == 0) {
		nJugador[0] = 'A';
		nJugador[1] = 'N';
		nJugador[2] = 'O';
		nJugador[3] = 'N';
		numLetras = 4;
	}
	// Se  definen todos los caracteres después de los escritos por el usuario como vacíos
	nJugador[numLetras] = NULL;

	// Se va a guardar la información en el archivo de Registro
	guardarDatos(nJugador, score, cEnemigoD, cMeteoD);
	
	// Se reinician las variables
	score = 0;
	cEnemigoD = 0;
	cMeteoD = 0;

}