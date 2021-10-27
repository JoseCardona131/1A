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
#define FPS 120.0

// Variable para determinar el intervalo entre disparos
#define FireCooldown 0.3

// Variables para llevar estadísticas del juego
int score = 0;
int enemigos_destruidos = 0;
int minas_destruidas = 0;

// Global para el movimiento
//Direccion { ARRIBA, ABAJO, DERECHA, IZQUIERDA }; //!
// Cada bool coincide con las direcciones mencionadas en el comentario anterior
bool teclasDireccion[4] = { false, false, false, false };

// Función principal del juego
// Recibe un bitmap que representa la imágen que se usará para el jugador dependiendo de la selección en el menú principal
// Regresa un valor booleano que servirá para cerrar la ventana de selección de nave cuando termina

bool Cosmic_Defender(ALLEGRO_BITMAP* IJugador) {

	// Se desinstala el mouse ya que no hace falta dentro del juego
	al_uninstall_mouse();
	
	// Se definen los datos de la ventana para el juego
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY* ventanaJuego = al_create_display(1920, 1080);
	al_set_window_title(ventanaJuego, "Cosmic Defender");

	// Se verifica si se pudo crear correctamente la ventana
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

	// Explosiones
	ALLEGRO_BITMAP* IEXPLOSION = al_load_bitmap("Images/explo.png");

	// Se crea un arreglo de Enemigos y se cargan las imágenes para cada elemento
	ALLEGRO_BITMAP* enemigos[2];
	enemigos[0] = al_load_bitmap("Images/Cacodemon.png");
	enemigos[1] = al_load_bitmap("Images/Pain_elemental.png");

	// Se crea un arreglo de imágenes para los meteoros
	ALLEGRO_BITMAP* Meteoros[3];
	Meteoros[0] = al_load_bitmap("Images/Meteor1.png");
	Meteoros[1] = al_load_bitmap("Images/Meteor2.png");
	Meteoros[2] = al_load_bitmap("Images/Meteor3.png");

	// Se cargan las imágenes para formar el tubo
	ALLEGRO_BITMAP* partesTubo[4];
	partesTubo[0] = al_load_bitmap("Images/mitad.png");
	partesTubo[1] = al_load_bitmap("Images/Der.png");
	partesTubo[2] = al_load_bitmap("Images/Izq.png");
	partesTubo[3] = al_load_bitmap("Images/mitad reverso.png");

	// Fondo para el juego
	ALLEGRO_BITMAP* FondoJuego = al_load_bitmap("Images/FondoJuego.jpg");

	// Sonidos
	// Música de Fondo para el juego
	ALLEGRO_SAMPLE* musicaFondoJuego = al_load_sample("Sound/MusicaJuego.wav");;

	// Sonido de un disparo necesario para el juego
	ALLEGRO_SAMPLE* sonidoDisparo = al_load_sample("Sound/Laser Gun Sound.wav");

	// Sonido de un impacto / explosión
	ALLEGRO_SAMPLE* sonidoChoque = al_load_sample("Sound/8 bit impact sound effect (volume up).wav");

	// Eventos
	// Se crea un timer que servirá para refrescar la pantalla
	ALLEGRO_TIMER* timerRefresh = al_create_timer(1 / FPS);

	// Se crea una cola de eventos 
	ALLEGRO_EVENT_QUEUE* colaJuego = al_create_event_queue();

	// Fuente para estadisticas
	ALLEGRO_FONT* fuenteE = al_load_font("Font/Pixeboy.ttf",45,NULL);


	// Se registra el timer y el teclado como fuentes de eventos para el juego
	al_register_event_source(colaJuego, al_get_timer_event_source(timerRefresh));
	al_register_event_source(colaJuego, al_get_keyboard_event_source());


	// Se van a crear las listas de enemigos y Minas
	// Enemigos
	enemigo* Aliens = NULL;
	inicializarlistaEnemigos(Aliens, false, enemigos, Meteoros);

	// Minas/ Meteoros
	enemigo* LMeteoros = NULL;
	inicializarlistaEnemigos(LMeteoros, true, enemigos, Meteoros);

	// Se crea un Jugador y se inicializa
	jugador Player1;
	Player1.ejeX = 1920 / 2;
	Player1.ejeY = 960;
	Player1.vidas = 3;
	Player1.listaBalas = NULL;

	//Se crea el tubo de obstáculo
	tubo ObstaculoVertical;

	// Comienza extendido a la derecha
	ObstaculoVertical.derecha = true;

	// Comienza fuera de la pantalla
	ObstaculoVertical.ejeY = -500;

	// Se va a determinar el largo inicial del tubo
	ObstaculoVertical.largoTubo = rand() % 1200;
	if (ObstaculoVertical.largoTubo < 800) { ObstaculoVertical.largoTubo = 800; }

	//Flag para poder finalizar el ciclo del juego
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

	// Ciclo del juego
	while (!terminar) {

		// Se crea una variable de evento y se espera hasta que se le asigna algun evento de la cola
		ALLEGRO_EVENT evento;
		al_wait_for_event(colaJuego, &evento);

		// 
		// EVENTOS RECIBIDOS DEL TECLADO
		//

		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {//Para las teclas presionadas:
			switch (evento.keyboard.keycode) {

			//Si se preciona ESCAPE se sale del juego
			case ALLEGRO_KEY_ESCAPE:
				terminar = true;
				break;

			// CAMBIOS DE DIRECCIÓN
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

				// Se revisa si ya paso el timer de cooldown desde el último disparo
				if (((clock() - ultimoDisparo) / CLOCKS_PER_SEC) > FireCooldown){

					// Se agrega una bala nueva a la lista
					al_play_sample(sonidoDisparo, 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					AgregarListaBalas(Player1.listaBalas, Player1.ejeX+37, Player1.ejeY);
					ultimoDisparo = clock();

				}	
			}
		}

		// Si se tenia presionada una tecla y se deja de presionar
		// Se apaga la dirección
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

		// 
		// EVENTOS DE TIMER
		//

		if (evento.type == ALLEGRO_EVENT_TIMER){
			
			// Se revisa de que timer proviene el evento
			// Si proviene del timer de refresh se va a manejar el movimiento de todos los elementos en  pantalla
			if (evento.timer.source == timerRefresh) {

				// Si esta presionada la tecla arriba
				if (teclasDireccion[0]) {

					// Se revisa si no esta fuera de los limites derechos
					if (Player1.ejeY >= 200) {

						// Si es posible se mueve
						Player1.ejeY = Player1.ejeY -= 5;

					}
				}

				// Si esta presionada la tecla abajo
				if (teclasDireccion[1]) {

					// Se revisa si no esta fuera de los limites derechos
					if (Player1.ejeY <= 960) {

						// Si es posible se mueve
						Player1.ejeY = Player1.ejeY += 5;

					}
				}

				// Si esta presionada la tecla derecha
				if (teclasDireccion[2]) {

					// Se revisa si no se esta fuera de los limites derechos
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

				// Se dibuja el fondo y el rectángulo con estadísticas al lado izquierdo de la pantalla
				// Antes de dibujar se limpia la pantalla
				al_clear_to_color(al_map_rgb(0, 0, 0));

				// Se dibuja el fondo
				al_draw_scaled_bitmap(FondoJuego, 0, progresoF, 1920, 1080, 0, 0, 1920, 1080, NULL);
				
				// Texto / Estadisticas
				al_draw_filled_rectangle(0, 0, 320, 1080, al_map_rgb(15, 15, 15));
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71),15,150,NULL,"COSMIC DEFENDER");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 250, NULL, "Vidas: ");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 350, NULL, "Puntaje: ");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 450, NULL, "Enemigos");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 500, NULL, "Destruidos:");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 600, NULL, "Minas");
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 15, 650, NULL, "Destruidas: ");

				// Los datos de estadisticas son convertidos de ints a texto y son desplegados
				// Vidas del Jugador
				string vid = to_string(Player1.vidas);
				char const* vidasJ = vid.c_str();				
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 140, 250, NULL, vidasJ);

				// Puntaje del Jugador
				string sc = to_string(score);
				char const* ScoreJ = sc.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 190, 350, NULL, ScoreJ);

				// Enemigos destruidos
				string ed = to_string(enemigos_destruidos);
				char const* enemyD = ed.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 250, 500, NULL, enemyD);

				// Minas destruidas
				string md = to_string(minas_destruidas);
				char const* MinaD = md.c_str();
				al_draw_text(fuenteE, al_map_rgb(250, 179, 71), 250, 650, NULL, MinaD);

				// Se va a dibujar la nave
				int ancho = al_get_bitmap_width(IJugador);
				int largo = al_get_bitmap_height(IJugador);
				al_draw_scaled_bitmap(IJugador, 0, 0, ancho, largo, Player1.ejeX, Player1.ejeY, 130, 130, NULL);

				// Se dibuja el tubo obstáculo
				dibujarTubo(ObstaculoVertical, partesTubo);
				
				// Se dibujan los enemigos y meteoros
				pintarObstaculos(Aliens, LMeteoros);

				// Se dibujan las balas existentes
				pintarBalas(Player1.listaBalas, IBala);

				// Se desplazan todos los elementos en pantalla
				desplazarEnemigos(Aliens, false, enemigos, Meteoros);
				desplazarTubo(ObstaculoVertical);
				desplazarEnemigos(LMeteoros, true, enemigos, Meteoros);
				desplazarBalas(Player1.listaBalas);
				
				// Se van a verificar si hubieron alguna clase de choques durante esta vuelta del ciclo

				// Choques sobre el jugador
				// Estas funciones devolverán true si durante la revisión de impactos las vidas del jugador llego a 0 
				
				// Impactos con Aliens
				terminar = revisarImpactoJugador(Player1, Aliens,false, ultimoimpacto, IJugador,enemigos ,Meteoros, sonidoChoque);
				if (terminar) {
					ventanaGuardarArchivo(score, enemigos_destruidos, minas_destruidas, colaJuego,fuenteE);
					break;
				}

				// Impactos con Meteoros
				terminar = revisarImpactoJugador(Player1, LMeteoros,true, ultimoimpacto, IJugador, enemigos, Meteoros, sonidoChoque);
				if (terminar) {
					ventanaGuardarArchivo(score, enemigos_destruidos, minas_destruidas, colaJuego, fuenteE);
					break;
				}

				// Impactos con el tubo
				terminar = revisarImpactoTubo(Player1, ObstaculoVertical, ultimoimpacto, sonidoChoque);
				if (terminar) {
					ventanaGuardarArchivo(score, enemigos_destruidos, minas_destruidas, colaJuego, fuenteE);
					break;
				}

				// Para revisar impactos de balas con enemigos
				revisarImpactoEnemigo(Player1.listaBalas, Aliens, LMeteoros, enemigos_destruidos, minas_destruidas, score, enemigos, Meteoros, sonidoChoque);

				// Se eliminan las balas, enemigos y meteoros que tuvieron un impacto
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

	// Se reinician las teclas de dirección
	teclasDireccion[0] = false;
	teclasDireccion[1] = false;
	teclasDireccion[2] = false;
	teclasDireccion[3] = false;

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
	return true;
}