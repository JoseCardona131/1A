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
void revisarImpactoJugador(jugador &Player, enemigo*&ListaEnemigos, float ultimoImpacto, ALLEGRO_BITMAP* IJugador){

	// Se van a revisar los enemigos
	enemigo* actual = ListaEnemigos;

	while (actual != NULL) {
		
		// Si el enemigo choca con el hitbox del jugador
		if (actual->ejeX >= Player.ejeX + 45 && actual->ejeX <= Player.ejeX - 45 && actual->ejeY >= Player.ejeY - 100 && actual->ejeY <= Player.ejeY + 100) {

			if (((clock() - ultimoImpacto) / CLOCKS_PER_SEC) > cooldownImpacto) {
				Player.vidas -= 1;
				cout << "impacto" << endl;
				actual->impacto = true;

				// Se obtienen los datos de la imágen
				int ancho = al_get_bitmap_width(IJugador);
				int largo = al_get_bitmap_height(IJugador);

				// Se dibuja la imagen parpadeando rojo para representar el impacto
				al_draw_tinted_scaled_bitmap(IJugador, al_map_rgb(250, 60, 60), 0, 0, ancho, largo, Player.ejeX, Player.ejeY, 130, 130,NULL);
				al_draw_tinted_scaled_bitmap(IJugador, al_map_rgb(170, 60, 60), 0, 0, ancho, largo, Player.ejeX, Player.ejeY, 130, 130, NULL);
				al_draw_tinted_scaled_bitmap(IJugador, al_map_rgb(250, 60, 60), 0, 0, ancho, largo, Player.ejeX, Player.ejeY, 130, 130, NULL);
			}
		}
		actual = actual->siguienteE;
	}
}


