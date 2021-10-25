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
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

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

	// Se van a cargar las variables y datos para ALLEGRO
	// Variables de Allegro
	// 
	// Imágen para el jugador
	ALLEGRO_BITMAP* SkinsJugador[3];
	SkinsJugador[0] = al_load_bitmap("Images/SpaceShip.png");
	SkinsJugador[1] = al_load_bitmap("Images/SpaceShip2.png");
	SkinsJugador[2] = al_load_bitmap("Images/SpaceShip3.png");


	// Fondo del Menú
	ALLEGRO_BITMAP* FondoMenu = al_load_bitmap("Images/FondoMenu.jpg");

	// Fondo selección de Jugador
	ALLEGRO_BITMAP* FondoSelec = al_load_bitmap("Images/Fondo_selec_naves.jpg");

	// Música del menú
	ALLEGRO_SAMPLE* musicaFondoMenu = al_load_sample("Sound/MusicaMenu.wav");

	// Fuentes del Menú
	ALLEGRO_FONT* fuenteTitulo = al_load_font("Font/Quake___.ttf",75,ALLEGRO_ALIGN_CENTRE);

	ALLEGRO_FONT* TitleSubVentana = al_load_font("Font/Quake___.ttf", 45, ALLEGRO_ALIGN_CENTRE);
	ALLEGRO_FONT* fuenteSubVentana = al_load_font("Font/Quake___.ttf", 30, ALLEGRO_ALIGN_CENTRE);

	// Se creará una cola de eventos para el menú
	ALLEGRO_EVENT_QUEUE* cola_menu = al_create_event_queue();

	// Timer de Refrescamiento
	ALLEGRO_TIMER* timerRefresh = al_create_timer(1.0 / FPS);

	// Se registrarán los timers, el mouse y cambios en la pantalla como eventos

	// Se registra el teclado como una fuente de eventos de la cola
	al_register_event_source(cola_menu, al_get_keyboard_event_source());

	// Se registra el mouse como una fuente de eventos de la cola
	al_register_event_source(cola_menu, al_get_mouse_event_source());

	// Se registra el timer como una fuente de eventos en la cola
	al_register_event_source(cola_menu, al_get_timer_event_source(timerRefresh));

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

	al_start_timer(timerRefresh);

	// Mientras no se deba terminar
	while (!terminar) {

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

				// Se le va a preguntar al usuario que skin prefiere para su nave
				// Esto se hará a través de una una ventana nueva que funciona a través de un ciclo que no se detendrá hasta que se marque salir

				bool finalizar = false;
				// Se limpia el display
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				int XCursor2 = 0;
				int	YCursor2 = 0;
				// Se crea un ciclo para la ventana
				while (!finalizar) {
					al_wait_for_event(cola_menu, &evento);

					// Se limpia el display
					al_clear_to_color(al_map_rgb(0, 0, 0));
					
					al_draw_tinted_bitmap(FondoSelec, al_map_rgb(50, 50, 50), 0, 0, NULL);

					// Se va dibujar el menú para seleccionar una skin de jugador
					al_draw_text(TitleSubVentana, al_map_rgb(250, 75, 75), mitadX, 100, ALLEGRO_ALIGN_CENTRE, "SELECCIONE UN JUGADOR");

					// Manticore
					al_draw_text(fuenteSubVentana, al_map_rgb(250, 250, 250), 160, 500, ALLEGRO_ALIGN_CENTRE, "MANTICORE");

					int anchoImagen = al_get_bitmap_width(SkinsJugador[0]);
					int altoImagen = al_get_bitmap_height(SkinsJugador[0]);

					al_draw_scaled_bitmap(SkinsJugador[0],0,0, anchoImagen, altoImagen, 50, 250, 220, 220, NULL);

					// Siege
					al_draw_text(fuenteSubVentana, al_map_rgb(250, 250, 250), 530, 500, ALLEGRO_ALIGN_CENTRE, "SIEGE");

					anchoImagen = al_get_bitmap_width(SkinsJugador[1]);
					altoImagen = al_get_bitmap_height(SkinsJugador[1]);

					al_draw_scaled_bitmap(SkinsJugador[1], 0, 0, anchoImagen, altoImagen, 430, 250, 200, 200, NULL);

					// Specter
					al_draw_text(fuenteSubVentana, al_map_rgb(250, 250, 250), 900, 500, ALLEGRO_ALIGN_CENTRE, "SPECTER");

					anchoImagen = al_get_bitmap_width(SkinsJugador[2]);
					altoImagen = al_get_bitmap_height(SkinsJugador[2]);

					al_draw_scaled_bitmap(SkinsJugador[2], 0, 0, anchoImagen, altoImagen, 800, 270, 180, 180, NULL);

					// Para regresar al menú principal
					al_draw_text(TitleSubVentana, al_map_rgb(250, 250, 250), mitadX, 800, ALLEGRO_ALIGN_CENTRE, "REGRESAR AL MENU PRINCIPAL");

					// Para eventos generados por el Mouse
					if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
					{
						XCursor = evento.mouse.x;
						YCursor = evento.mouse.y;
					}

					// Se va a verificar si se clickeo sobre alguno de los nombres de las naves para seleccionarla
					// Dependiendo de la nave se va a lanzar la función cosmic defender con una skin diferente
					
					// Para Seleccionar Manticore
					if ((XCursor >= 50 && XCursor <= 260) && (YCursor >= 480 && YCursor <= 520)) {
						al_draw_text(fuenteSubVentana, al_map_rgb(250, 75, 250), 160, 500, ALLEGRO_ALIGN_CENTRE, "MANTICORE");

						// Se revisa si se selecciona esta nave
						if (evento.mouse.button & 1) {

							al_destroy_sample(musicaFondoMenu);
							al_destroy_display(ventanaMenu);
							finalizar = Cosmic_Defender(SkinsJugador[0]);
						}
					}

					// Para seleccionar Siege
					if ((XCursor >= 460 && XCursor <= 580) && (YCursor >= 480 && YCursor <= 520)) {
						al_draw_text(fuenteSubVentana, al_map_rgb(250, 75, 75), 530, 500, ALLEGRO_ALIGN_CENTRE, "SIEGE");

						// Se revisa si se selecciona esta nave
						if (evento.mouse.button & 1) {

							al_destroy_sample(musicaFondoMenu);
							al_destroy_display(ventanaMenu);
							finalizar = Cosmic_Defender(SkinsJugador[1]);
						}
					}

					// Para seleccionar Specter
					if ((XCursor >= 820 && XCursor <= 970) && (YCursor >= 480 && YCursor <= 520)) {
						al_draw_text(fuenteSubVentana, al_map_rgb(250, 250, 75), 900, 500, ALLEGRO_ALIGN_CENTRE, "SPECTER");

						// Se revisa si se selecciona esta nave
						if (evento.mouse.button & 1) {

							al_destroy_sample(musicaFondoMenu);
							al_destroy_display(ventanaMenu);
							finalizar = Cosmic_Defender(SkinsJugador[2]);
						}

					}

					// Si se quiere volver al menú principal
					if ((XCursor >= mitadX - 150 && XCursor <= mitadX + 150) && (YCursor >= 780 && YCursor <= 875)) {
						al_draw_text(TitleSubVentana, al_map_rgb(75, 250, 250), mitadX, 800, ALLEGRO_ALIGN_CENTRE, "REGRESAR AL MENU PRINCIPAL");

						// Se revisa si se selecciona esta opcion
						if (evento.mouse.button & 1) {

							al_destroy_sample(musicaFondoMenu);
							al_destroy_display(ventanaMenu);
							finalizar = true;
						}
					}

					// Se refresca la pantalla
					al_flip_display();

				}
				al_install_mouse();
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
	}

	return 666;

}
