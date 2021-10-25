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
#include<string>  

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
#pragma warning(disable:4996);

// Estructuras principales y variables para el juego

// Velocidad de Refrescamiento del Juego (Necesario para el main)
#define FPS 60.0

// Variable para determinar el intervalo entre disparos
#define FireCooldown 0.5

// Variables para llevar estadísticas del juego
int score = 0;

int enemigos_destruidos = 0;

int minas_destruidas = 0;

int total_enemigos_generados = 0;

int total_minas_generadas = 0;

// Global para el movimiento
//Direccion { ARRIBA, ABAJO, DERECHA, IZQUIERDA }; //!
// Cada bool coincide con lasdirecciones de arriba
bool teclasDireccion[4] = { false, false, false, false };



// Función principal del juego
// Recibe un bitmap que representa la imágen que se usará para el jugador dependiendo de la selección en el menú principal

int Cosmic_Defender(ALLEGRO_BITMAP* IJugador) {

	// Se desinstala el mouse ya que no se usará más
	al_uninstall_mouse();
	
	// Se definen los datos de la ventana
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
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

	//Imágenes
	// Balas
	ALLEGRO_BITMAP* IBala = al_load_bitmap("Images/Bala.png");

	// Se crea un arreglo de Enemigos y se cargan las imágenes para cada elemento
	ALLEGRO_BITMAP* enemigos[2];
	enemigos[0] = al_load_bitmap("Images/Cacodemon.png");
	enemigos[1] = al_load_bitmap("Images/Pain_elemental.png");

	// Se crea un arreglo de imágenes para los meteoros
	ALLEGRO_BITMAP* Meteoros[3];
	Meteoros[0] = al_load_bitmap("Images/Meteor1.png");
	Meteoros[1] = al_load_bitmap("Images/Meteor2.png");
	Meteoros[2] = al_load_bitmap("Images/Meteor3.png");

	// Fondo para el juego
	ALLEGRO_BITMAP* FondoJuego = al_load_bitmap("Images/FondoJuego.jpg");

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

	// Fuente para estadisticas
	ALLEGRO_FONT* fuenteE = al_load_font("Font/Overseer.otf",45,NULL);


	// Se registra el timer como fuente de evento
	al_register_event_source(colaJuego, al_get_timer_event_source(timerRefresh));
	al_register_event_source(colaJuego, al_get_keyboard_event_source());

	// Se van a crear las listas de enemigos y Minas

	// Enemigos
	enemigo* Aliens = NULL;
	inicializarlistaEnemigos(Aliens, false, enemigos, Meteoros);

	// Minas/ Meteoros
	enemigo* LMeteoros = NULL;
	inicializarlistaEnemigos(LMeteoros, true, enemigos, Meteoros);

	// Jugador
	jugador Player1;
	Player1.ejeX = 1920 / 2;
	Player1.ejeY = 960;
	Player1.vidas = 3;
	Player1.listaBalas = NULL;

	//Flag
	bool terminar = false;

	// Comienzan los timers, musica y ciclo
	al_start_timer(timerRefresh);
	al_play_sample(musicaFondoJuego, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	// Variables necesarias en el ciclo
	// Para manejar el progreso del fondo
	int progresoF = 1080;

	// Variable para determinar el tiempo cuando sucedio el último disparo
	// Sirve para tener un "cooldown" entre disparos, esto para evitar spam de balas
	float ultimoDisparo = clock();

	// Variable para saber el tiempo cuando sucedió el último impacto con el jugador
	// Permite tener inmunidad al inicio de juego y un cooldown entre impactos recibidos
	float ultimoimpacto = clock();


	// Para

	while (!terminar) {

		ALLEGRO_EVENT evento;

		al_wait_for_event(colaJuego, &evento);

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {//Para las teclas presionadas:
			switch (evento.keyboard.keycode) {

			//Si se preciona ESCAPE se sale del juego
			case ALLEGRO_KEY_ESCAPE:
				terminar = true;
				break;

			//Si se preciona la flecha izquierda 
			case ALLEGRO_KEY_LEFT:
				teclasDireccion[3] = true;
				break;

			//Si se preciona la flecha derecha 
			case ALLEGRO_KEY_RIGHT:
				teclasDireccion[2] = true;
				break;

			//Si se preciona la flecha de arriba 
			case ALLEGRO_KEY_UP:
				teclasDireccion[0] = true;
				break;

				//Si se preciona la flecha de abajo
			case ALLEGRO_KEY_DOWN:
				teclasDireccion[1] = true;
				break;

			//Al presionar espacio se crea un disparo y se registra en la lista del jugador
			case ALLEGRO_KEY_SPACE:

				// Se revisa si ya paso el timer de cooldown
				if (((clock() - ultimoDisparo) / CLOCKS_PER_SEC) > FireCooldown){

					AgregarListaBalas(Player1.listaBalas, Player1.ejeX+37, Player1.ejeY);
					ultimoDisparo = clock();
				}	
			}
		}

		// Si se tenia presionada una tecla y se deja de presionar
		if (evento.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (evento.keyboard.keycode) {
			case ALLEGRO_KEY_LEFT:
				teclasDireccion[3] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				teclasDireccion[2] = false;
				break;
			case ALLEGRO_KEY_UP:
				teclasDireccion[0] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				teclasDireccion[1] = false;
				break;
			}
		}

		// Si el evento viene de un timer
		if (evento.type == ALLEGRO_EVENT_TIMER){
			
			// Se revisa de que timer proviene el evento
			// Si proviene del timer de refresh se va a manejar el movimiento del fondo y la nave del jugador
			if (evento.timer.source == timerRefresh) {

				// Si esta presionada la tecla arriba
				if (teclasDireccion[0]) {

					// Se revisa sii no esta en los limites derechos
					if (Player1.ejeY >= 200) {

						// Si es posible se mueve
						Player1.ejeY = Player1.ejeY -= 5;

					}
				}

				// Si esta presionada la tecla abajp
				if (teclasDireccion[1]) {

					// Se revisa si no esta en los limites derechos
					if (Player1.ejeY <= 960) {

						// Si es posible se mueve
						Player1.ejeY = Player1.ejeY += 5;

					}
				}

				// Si esta presionada la tecla derecha
				if (teclasDireccion[2]) {

					// Se revisa si no esta en los limites derechos
					if (Player1.ejeX <= 1900) {

						// Si es posible se mueve
						Player1.ejeX = Player1.ejeX + 10;

					}
				}

				// Si esta presionada la tecla izquierda
				if (teclasDireccion[3]) {

					// Se revisa si la nave no esta en los limites izquierdos
					if (Player1.ejeX >= 340) {

						// Si es posible se mueve
						Player1.ejeX = Player1.ejeX - 10;

					}
				}

				// Se dibuja el fondo
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_scaled_bitmap(FondoJuego, 0, progresoF, 1920, 1080, 0, 0, 1920, 1080, NULL);
				al_draw_filled_rectangle(0, 0, 320, 1080, al_map_rgb(15, 15, 15));

				// Texto / Estadisticas
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71),15,150,NULL,"COSMIC DEFENDER");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 250, NULL, "Vidas: ");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 350, NULL, "Puntaje: ");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 450, NULL, "Enemigos");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 500, NULL, "Destruidos:");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 600, NULL, "Minas");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 650, NULL, "Destruidas: ");

				// Vidas del Jugador
				string vid = to_string(Player1.vidas);
				char const* vidasJ = vid.c_str();				
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 130, 250, NULL, vidasJ);

				// Puntaje del Jugador
				string sc = to_string(score);
				char const* ScoreJ = sc.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 150, 350, NULL, ScoreJ);

				// Enemigos destruidos
				string ed = to_string(enemigos_destruidos);
				char const* enemyD = ed.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 200, 500, NULL, enemyD);

				// Minas destruidas
				string md = to_string(enemigos_destruidos);
				char const* MinaD = md.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 200, 650, NULL, MinaD);

				// Se va a dibujar la nave
				int ancho = al_get_bitmap_width(IJugador);
				int largo = al_get_bitmap_height(IJugador);

				al_draw_scaled_bitmap(IJugador, 0, 0, ancho, largo, Player1.ejeX, Player1.ejeY, 130, 130, NULL);


				// Se dibujan los obstaculos
				pintarObstaculos(Aliens, LMeteoros);
				pintarBalas(Player1.listaBalas, IBala);
				desplazarEnemigos(Aliens, false, enemigos, Meteoros);
				desplazarEnemigos(LMeteoros, true, enemigos, Meteoros);
				desplazarBalas(Player1.listaBalas);
				revisarImpactoJugador(Player1, Aliens, ultimoimpacto,IJugador);
				revisarImpactoJugador(Player1, LMeteoros, ultimoimpacto, IJugador);
				eliminarBalasDestruidas(Player1.listaBalas);
				eliminarEnemigosDestruidos(Aliens);
				eliminarEnemigosDestruidos(LMeteoros);


				// Se revisa si ya se va a pasar del tamaÑo de la imagen para el fondo
				// Si sucede eso se reinicia el progreso
				if (progresoF - 1 == 0) {
					progresoF = 1080;
				}

				// Se avanza el fondo hacia adelante
				progresoF--;

			
			}

		}
		al_flip_display();
		
	}
	// Se destruyen todos los datos de allegro creados para el juego
	al_destroy_sample(sonidoDisparo);
	al_destroy_sample(sonidoChoque);
	al_destroy_timer(timerRefresh);
	al_destroy_event_queue(colaJuego);
	al_destroy_sample(musicaFondoJuego);
	al_destroy_bitmap(FondoJuego);
	al_destroy_bitmap(enemigos[0]);
	al_destroy_bitmap(enemigos[1]);
	al_destroy_bitmap(Meteoros[0]);
	al_destroy_bitmap(Meteoros[1]);
	al_destroy_bitmap(Meteoros[2]);
	al_destroy_bitmap(IBala);
	al_destroy_display(ventanaJuego);
	return 1;
}