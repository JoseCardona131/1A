// Estructuras de Datos
// Proyecto 1A
// Juego en Allegro
// 
// Integrantes:
// José Ricardo Cardona Quesada
// Carlos Leiva Medaglia


// Librerías requeridas
// Librerías C++
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// Librerías Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// Se incluye el archivo con las funciones y el juego en si
#include "CosmicDefender.h"

using namespace std;


// Código Ejecutable del Juego
int main() {

	// Se va a inicializar Allegro, en caso de que no se pueda se abrirá una ventana con un mensaje de error
	if (!al_init()) {
		al_show_native_message_box(NULL, "Error de Carga", "Error", "El programa no pudo accesar a la librería Allegro", NULL, NULL);
		return -666;
	}

	// En caso de que si se pudiera inicializar ALLEGRO
	//Se instalarán todos los complementos de ALLEGRO requeridos para el funcionamiento correcto del programa
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(7);
	al_install_mouse();

	// Para que el display sea en pantalla completa y se le pueda cambiar el tamaño
	al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	// Se crea un display actual para el menú principal con los tamaños máximos de la pantalla
	ALLEGRO_DISPLAY* ventanaMenu = al_create_display(1920, 1080);

	// Se le asinga un título a la ventana
	al_set_window_title(ventanaMenu, "Cosmic Defender");
	if (!ventanaMenu)
	{
		//Si el display no se ejecuta se lanza el mensaje de error
		al_show_native_message_box(NULL, "Error de Allegro", "Error", "No se puede crear el Display", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	// Se coloca la la ventana
	al_set_window_position(ventanaMenu, 100, 150);

	// Se van a cargar las variables y datos para ALLEGRO
	// Variables de Allegro
	// 
	// Imágen para el jugador
	ALLEGRO_BITMAP* Jugador = al_load_bitmap("Images/SpaceShip.jpg");

	// Fondo del Menú
	ALLEGRO_BITMAP* FondoMenu = al_load_bitmap("Images/FondoMenu.jpg");

	// Música del menú
	ALLEGRO_SAMPLE* musicaFondoMenu = al_load_sample("Sound/MusicaMenu.wav");

	// Fuentes del Menú
	ALLEGRO_FONT* fuenteTitulo = al_load_font("Font/Quake___.ttf",75,ALLEGRO_ALIGN_CENTRE);

	// Se creará una cola de eventos para el menú
	ALLEGRO_EVENT_QUEUE* cola_menu = al_create_event_queue();

	// Se registrarán los timers, el mouse y cambios en la pantalla como eventos

	// Se registra el teclado como una fuente de eventos de la cola
	al_register_event_source(cola_menu, al_get_keyboard_event_source());

	// Se registra el mouse como una fuente de eventos de la cola
	al_register_event_source(cola_menu, al_get_mouse_event_source());

	// Se reproduce la música para el menú en bucle hasta que se cierre la ventana
	al_play_sample(musicaFondoMenu, 0.3, 0,1, ALLEGRO_PLAYMODE_LOOP, NULL);

	//Se obtienen la magnitud del eje X en la ventana
	// Sirve para centrar elementos según su mitad
	int X = al_get_display_width(ventanaMenu);

	double mitadX = X / 2;

	// Ciclo para desplegar el menú del juego
	// Flag para el final del menú
	bool terminar = false;

	// Estas variables determinarán la posición del ratón en un determinado momento
	int XCursor = 0;
	int YCursor = 0;

	al_destroy_display(ventanaMenu);

	ALLEGRO_DISPLAY* nuevo = al_create_display(1920, 1080);


	// Mientras no se deba terminar
	while (!terminar) {

		al_destroy_sample(musicaFondoMenu);
		//Cosmic_Defender(Jugador);
		break;

		/*
		ALLEGRO_EVENT evento;

		al_wait_for_event(cola_menu, &evento);

		// Cada vez que se repite el ciclo
		// Se refresca el display
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

		// Se dibuja la imágen del menu y el texto
		al_draw_bitmap(FondoMenu, 0, 0, NULL);

		// Se usa X / 2 ya que significa que se dibujará en la mitad del eje X de la ventana
		al_draw_text(fuenteTitulo, al_map_rgb(250, 75, 75), mitadX, 100, ALLEGRO_ALIGN_CENTRE, "Q  GALACTIC DEFENDER  Q");
		al_draw_text(fuenteTitulo, al_map_rgb(200, 200, 200), mitadX, 300, ALLEGRO_ALIGN_CENTRE, "JUGAR");
		al_draw_text(fuenteTitulo, al_map_rgb(225, 225, 225), mitadX, 500, ALLEGRO_ALIGN_CENTRE, "RANKING");
		al_draw_text(fuenteTitulo, al_map_rgb(200, 200, 200), mitadX, 700, ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
		al_draw_text(fuenteTitulo, al_map_rgb(225, 225, 225), mitadX, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");


		// Para eventos generados por el Mouse
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			XCursor = evento.mouse.x;
			YCursor = evento.mouse.y;
		}

		// Se va a revisar si el mouse esta dentro de las coordenadas de alguna de las opciones del menú
		// Para revisar si esta en la  opción JUGAR
		// Si revisa si esta en el rango de X y Y correcto
		if ((XCursor >= mitadX - 125 && XCursor <= mitadX + 125) && (YCursor >= 280 && YCursor <= 375)) {

			// Se va a dibujar el texto en el mismo lugar con un color diferente para que se note que el cursor esta sobre esta opción
			al_draw_text(fuenteTitulo, al_map_rgb(250, 75, 75), mitadX, 300, ALLEGRO_ALIGN_CENTRE, "JUGAR");

			// Se agrega el 1 al if para que solo cuente si se presiona el botón izquierdo
			if (evento.mouse.button & 1) {

				al_destroy_sample(musicaFondoMenu);
				al_destroy_display(ventanaMenu);

				bool finalizar = true;
				while (finalizar) {
					finalizar = Cosmic_Defender(Jugador);
				}
				main();
				
				
			}
		}

		// Para revisar si esta en la opción RANKING
		// Si revisa si esta en el rango de X y Y correcto
		if ((XCursor >= mitadX - 200 && XCursor <= mitadX + 200) && (YCursor >= 480 && YCursor <= 570)) {

			// Se va a dibujar el texto en el mismo lugar con un color diferente para que se note que el cursor esta sobre esta opción
			al_draw_text(fuenteTitulo, al_map_rgb(250, 75, 75), mitadX, 500, ALLEGRO_ALIGN_CENTRE, "RANKING");

			// Se agrega el 1 al if para que solo cuente si se presiona el botón izquierdo
			if (evento.mouse.button & 1) {

				cout << "Ranking" << endl;
			}
		}

		// Para revisar si esta en la opción INSTRUCCIONES
		// Si revisa si esta en el rango de X y Y correcto
		if ((XCursor >= mitadX - 345 && XCursor <= mitadX + 345) && (YCursor >= 680 && YCursor <= 770)) {

			// Se va a dibujar el texto en el mismo lugar con un color diferente para que se note que el cursor esta sobre esta opción
			al_draw_text(fuenteTitulo, al_map_rgb(250, 75, 75), mitadX, 700, ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");

			// Se va a revisar si se presiono el click izquierdo del mouse dentro de estas coordenadas en X,Y 
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				// Se agrega el 1 al if para que solo cuente si se presiona el botón izquierdo
				if (evento.mouse.button & 1) {

					cout << "instrucciones" << endl;
				}
			}

		}

		// Para revisar si esta en la opción INSTRUCCIONES
		// Si revisa si esta en el rango de X y Y correcto
		if ((XCursor >= mitadX - 125 && XCursor <= mitadX + 125) && (YCursor >= 880 && YCursor <= 970)) {

			// Se va a dibujar el texto en el mismo lugar con un color diferente para que se note que el cursor esta sobre esta opción
			al_draw_text(fuenteTitulo, al_map_rgb(250, 75, 75), mitadX, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");

			// Se va a revisar si se presiono el click izquierdo del mouse dentro de esas coordenadas en X,Y 
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				// Se agrega el 1 Al IF para que solo cuente si se presiona el botón izquierdo
				if (evento.mouse.button & 1) {

					// Si se presiona Salir se cerrará el menú
					terminar = true;
				}
			}
		}

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
	*/
	}

	return 666;

}
