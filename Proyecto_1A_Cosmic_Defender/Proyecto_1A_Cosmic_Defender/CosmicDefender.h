#pragma once

// Librerías de C++
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

// Librerías de Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "FuncionesCD.h"

using namespace std;


// Estructuras principales y variables para el juego

// Velocidad de Refrescamiento del Juego (Necesario para el main)
#define FPS 60.0

// Variables para llevar estadísticas del juego
int score = 0;

int enemigos_destruidos = 0;

int minas_destruidas = 0;

int total_enemigos_generados = 0;

int total_minas_generadas = 0;

using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0


// Función principal del juego
// Recibe un bitmap que representa la imágen que se usará para el jugador dependiendo de la selección en el menú principal

int Cosmic_Defender(ALLEGRO_BITMAP* JIugador) {

	// Se desinstala el mouse ya que no se usará más
	al_uninstall_mouse();
	
	// Se definen los datos de la ventana
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FULLSCREEN);
	ALLEGRO_DISPLAY* ventanaJuego = al_create_display(1920, 1080);
	//al_set_window_position(pantalla, 0, 0);
	al_set_window_title(ventanaJuego, "Space Invaders");

	if (!ventanaJuego)
	{
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	// Si se pudo crear el display se pasa a crear las variables necesarias para el funcionamiento del programa
	// Variables de ALLEGRO
	ALLEGRO_KEYBOARD_STATE teclado;

	//Imágenes
	// Balas
	ALLEGRO_BITMAP* IBala = al_load_bitmap("Images/Bala.png");

	// Se crea un arreglo de Enemigos y se cargan las imágenes para cada elemento
	ALLEGRO_BITMAP* enemigos[2];
	enemigos[0] = al_load_bitmap("Images/Cacodemon.jpg");
	enemigos[1] = al_load_bitmap("Images/Pain_elemental.png");

	// Se crea un arreglo de imágenes para los meteoros
	ALLEGRO_BITMAP* Meteoros[3];
	Meteoros[0] = al_load_bitmap("Images/Meteor1.png");
	Meteoros[1] = al_load_bitmap("Images/Meteor2.png");
	Meteoros[2] = al_load_bitmap("Images/Meteor3.png");

	// Música de Fondo para el juego
	ALLEGRO_SAMPLE* musicaFondoJuego = al_load_sample("Sound/MusicaJuego.wav");;

	// Sonido de un disparo necesario para el juego
	ALLEGRO_SAMPLE* sonidoDisparo = al_load_sample("Sound/Laser Gun Sound Effect.wav");

	// Sonido de un impacto / explosión
	ALLEGRO_SAMPLE* sonidoChoque = al_load_sample("Sound/8 bit impact sound effect (volume up).wav");

	// Se crea un timer que servirá para refrescar la pantalla
	ALLEGRO_TIMER* timerRefresh = al_create_timer(1 / FPS);

	// Se crea una cola de eventos 
	ALLEGRO_EVENT_QUEUE* colaJuego = al_create_event_queue();

	// Se registra el timer como fuente de evento
	al_register_event_source(colaJuego, al_get_timer_event_source(timerRefresh));
	al_register_event_source(colaJuego, al_get_keyboard_event_source());

	// Se van a crear las listas de enemigos y Minas

	// Enemigos
	enemigo* Aliens;
	inicializarlistaEnemigos(Aliens, false, enemigos, Meteoros);

	// Minas/ Meteoros
	enemigo* LMeteoros;
	inicializarlistaEnemigos(LMeteoros, true, enemigos, Meteoros);

	bool terminar = false;


	al_start_timer(timerRefresh);
	al_play_sample(musicaFondoJuego, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);


	while (!terminar) {

		ALLEGRO_EVENT evento;

		al_wait_for_event(colaJuego, &evento);

		al_draw_bitmap(enemigos[1],200,200,NULL);

		// Si se presiono una tecla
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Se revisa si se presiono la tecla ESC
			switch (evento.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:

				// Si es ESC  se cierra el menú
				terminar = true;
			}
		}
		// Si se presiona la X en la ventana del juego se va a cerrar el menú
		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			terminar = true;
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_flip_display();
		
	}
	return 1;
}