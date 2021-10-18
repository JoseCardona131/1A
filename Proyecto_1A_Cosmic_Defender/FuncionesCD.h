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

// Se van a definir las estructuras requeridas para el juego
// Reprenta los disparos que da el jugador
typedef struct bala {
	int ejeX;
	int ejeY;
	bool impacto;
	bala* siguiente;

};
// Jugador, representará la nave que se mueve con el teclado
typedef struct jugador {
	int ejeX;
	int ejeY;
	int vidas;
	bala* listaBalas;
};
// Sirve para representar meteoros y aliens 
// Tendrán una variable de bitmap para poder identificar como se deben de dibujar dentro del juego
typedef struct enemigo {
	int ejeX;
	int ejeY;
	bool impacto;
	ALLEGRO_BITMAP* skin;
	enemigo* siguiente;

};

// Funciones del Juego
// 
// Funciones para el manejo de Listas enlazadas
// Para agregar un elemento a una lista

// A una lista de enemigos
void AgregarListaEnemigos(enemigo* & listaE, int X, int Y, ALLEGRO_BITMAP* skin) {

	// Si la lista esta vacía
	if (listaE == NULL) {

		// Se le asigna una estructura enemigo
		listaE = new enemigo();

		// Se pone el siguiente elemento como nulo
		listaE->siguiente = NULL;
	}

	// Si la lista no esta vacía
	else {

		// Se crea un puntero que apunte al primer elemento en la lista
		// Sirve para no perder la dirección en memoria
		enemigo* aux = listaE;

		// Se le asigna un nuevo elemento a la lista
		listaE = new enemigo();

		// Se le asigna la dirección anteriormente guardada al siguiente del enemigo que se acaba de crear
		listaE->siguiente = aux;
	}

	// Se definen sus datos según los recibidos en la función
	listaE->ejeX = X;
	listaE->ejeY = Y;
	listaE->impacto = false;
	listaE->skin = skin;
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
	while (actual->siguiente != NULL) {

		// Si se esta en el primer elemento de la lista
		if (elementoAnterior == NULL) {

			// Si el primer elemento tuvo un impacto
			if (actual->impacto) {

				// Se mueve el puntero de la lista hacia adelante
				listaE = listaE->siguiente;

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
	// Si ya se llego al último elemento se revisa si es necesario destruirlo
	// Si el último elemento tuvo un impacto
	if (actual->impacto) {

		// Se aisla del resto de la lista y se borra
		elementoAnterior->siguiente = NULL;
		delete(actual);
	}

}

// Borrar Balas // Funciona igual que la función anterior, solo que con estructuras bala
// Esta función busca por balas con variables de impacto marcadas true y las elimina de la lista
void eliminarBalasDestruidas(bala*& listaB) {

	// Punteros para poder recorrer la lista
	bala* actual = listaB;
	bala* elementoAnterior = NULL;

	// Mientras el siguiente del enemigo actual no sea NULL (No se haya llegado al último elemento)
	while (actual->siguiente != NULL) {

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
	// Si ya se llego al último elemento se revisa si es necesario destruirlo
	// Si el último elemento tuvo un impacto
	if (actual->impacto) {

		// Se aisla del resto de la lista y se borra
		elementoAnterior->siguiente = NULL;
		delete(actual);
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

		// Se va a determinar la cantidad de minas a desplegar de forma aleatoria (Cantidad máxima inicial = 8)
		int cantidad_minas = rand() % 8;

		// Se va  a definir 4 como la cantidad mínima de minas con las que comenzar la lista
		if (cantidad_minas < 4) {
			cantidad_minas = 4;
		}
		// Se va a hacer un ciclo que agregue minas a la lista hasta llegar a la cantidad deseada
		for (int i = 0; i < cantidad_minas; i++) {

			// Por cada mina se va a sacar aleatoriamente una imágen del arreglo
			int indice_imagen = rand() % 3;

			// Ya que los arreglos empiezan en 0 se le resta 1 al índice
			indice_imagen -= 1;

			// Se agrega una mina a la lista con la imagen en ese índice del arreglo
			// Se genera en un X aleatorio dentro de los límites de la pantalla
			// Siempre empiezan desde la sección superior por lo que comienzan en Y = 0;
			AgregarListaEnemigos(lista, rand() % 1980, 0, Meteoros[indice_imagen]);

		}
	}
	// Si no se esta trabajando con minas
	else {
		// Se va a determinar la cantidad de enemigos a desplegar de forma aleatoria (Cantidad máxima inicial = 5)
		int cantidad_enemigos = rand() % 5;

		// Se va  a definir 2 como la cantidad mínima de enemigos con los que comenzar la lista
		if (cantidad_enemigos < 2) {
			cantidad_enemigos = 2;
		}

		// Se va a hacer un ciclo que agregue minas a la lista hasta llegar a la cantidad deseada
		for (int i = 0; i < cantidad_enemigos; i++) {

			// Por cada mina se va a sacar aleatoriamente una imágen del arreglo
			int indice_imagen = rand() % 2;

			// Ya que los arreglos empiezan en 0 se le resta 1 al índice
			indice_imagen -= 1;

			// Se agrega un enemigo a la lista con la imagen en ese índice del arreglo
			// Se genera en un X aleatorio dentro de los límites de la pantalla
			// Siempre empiezan desde la sección superior por lo que comienzan en Y = 0;
			AgregarListaEnemigos(lista, rand() % 1980, 0, enemigos[indice_imagen]);

		}
	}
}
