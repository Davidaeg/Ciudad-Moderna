#include <gtk/gtk.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "lib/globales.h"
#include "lib/City.h"
#include "lib/car.h"
/////////////////////////Variables//////////////////////
int selectedSpeed =0;

int contadorParadas = 0;

int tempParadas[112];
int accionesTemp[112];

int carx=0;
int cary=0;

int carAxisx = 0;
int carAxisy = 0;

int salida = 0;

int accion_de_parada = 0;

//declara metodos
void hide_all();

void show_car_widgets();
void hide_car_widgets();

void hide_truck_widgets();
void show_truck_widgets();

void show_bus_widgets();
void hide_bus_widgets();

void show_ship_widgets();
void hide_ship_widgets();

void show_train_widgets();
void hide_train_widgets();

int getx(int id){
	return paradas[id]->x;
}
int getY(int id){
	return paradas[id]->y;
}
int detStop(int cantidadParadas){
	if(cantidadParadas>=1){
		return cantidadParadas-1;
	}
	return 1;
}
////////////////////////////////////////////////Thread que genera Reparaciones///////////////////////////////////////
void *repair_generator(void *arg){

	repair=malloc(sizeof(Repair));

	repair->x = 0;
	repair->y = 6;

	repair->axisX = repair->x*25;
	repair->axisY = repair->y*25;


	int tempx = 16;
	int tempy = 0;

	while(true){

		sleep(15);

		srand(time(NULL));
		do{
			tempx = rand()%(33-0+1)+0;
			tempy = rand()%(27-0+1)+0;

		}while(cityMatrix[tempy][tempx] != 0);
		repair->x = tempx;
		repair->y = tempy;

		repair->axisX = repair->x*25;
		repair->axisY = repair->y*25;
		sleep(3);
	}
} //reapirs generator
/////////////////////////////////////////////////////EVENTOS///////////////////////////////////////////////
gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data){
	return TRUE;
}// Evento de los toggle buttons

void toggled_cb (GtkToggleButton *toggle_button, gpointer user_data){
	GtkWindow *window = user_data;

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (speedCB1))){

		selectedSpeed = 1;
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB2), FALSE);
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB3), FALSE);

	}else  if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(speedCB2))){

		selectedSpeed = 2;
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB1), FALSE);
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB3), FALSE);

	}else  if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(speedCB3))){

		selectedSpeed = 3;
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB2), FALSE);
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB1), FALSE);

	}

}//accion al seleccionar toggle button

gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data){

	if (event->button == GDK_BUTTON_PRIMARY)
	{
	//	printf("%f,%f\n", event->x, event->y );

		carAxisx = event->x;
		carAxisy = event->y;

		carx = event->x/25;
		cary = event->y/25;

		for(int i = 0; i<112; i++){
				if(carx == paradas[i]->x && cary==paradas[i]->y){
						salida = paradas[i]->id;
						break;
				}
		}


	}
	else if (event->button == GDK_BUTTON_SECONDARY)
	{
		delete_car(event->x, event->y);
	}

	return TRUE;
}//Press Event

/////////////////////////////////////////////////////////ON DRAW////////////////////////////////////////////////
void on_draw (GtkWidget *widget, cairo_t *cr, gpointer data){

	drawingCity(cr);  //dibuja el mapa
	draw_repair(cr);
	draw_train(cr);
	draw_ships(cr);
	draw_dock_cargo(cr);

	for (int i = 0; i < count; i++)  //ciclo para dibujar cada hijo carro
	{
		drawing_car(cr, carros[i]);
	}


	gtk_widget_queue_draw(widget);
}//Fin on_draw

void check_selection(GtkWidget *widget, gpointer data){
	int activado;
	activado = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
	hide_all();
	if (activado == 1) {
		show_car_widgets();
	}else if(activado == 2){
		show_bus_widgets();
	}else if(activado == 3){
		show_ship_widgets();
	}else if(activado == 4){
		show_truck_widgets();
	}else if(activado == 5){
		show_train_widgets();
	}

}//check_selection
/////////////////////////////////////////////////////CAR////////////////////////////////////////
//Metodo que crea un carro
void add_Car(GtkWidget *widget,gpointer data){
	//selecciona la velocidad
	if(selectedSpeed == 1){
		selectedSpeed = 50000;
	}else if(selectedSpeed == 2){
		selectedSpeed = 15000;
	}else if(selectedSpeed == 3){
		selectedSpeed = 9000;
	}

	//Se crea instancia del carro
	Car *car = malloc(sizeof(Car));

	car->type = 0;
	car->alive = 1;
	car->speed =  selectedSpeed;
	car->axisX = carx*25;
	car->axisY = cary*25;
	car->matrizx = carx;
	car->matrizy = cary;
	car->d = 1;
	car->run = 1;
	car->paso = 1;
	car->id = gtk_entry_get_text(GTK_ENTRY (idEntry));
	car->cantidadParadas = contadorParadas;
	for(int i = 0; i<contadorParadas; i++){
		car->vparadas[i] = tempParadas[i];
		car->accionparadas[i] = accionesTemp[i];
	}
	car->salida = salida;
	car->ide = count;

	//Se inician los hilos
	carros[count] = car;
	pthread_create(&threads_carros[count], NULL, move_car, (void *)carros[count]);
	count++;

	contadorParadas = 0;
	accion_de_parada = 0;

	gtk_widget_set_sensitive (add_car_button ,false);
}//Fin add_Car_Action

//seleciona paradas carro
void seleccionar_Paradas(GtkWidget *widget, gpointer data){

	tempParadas[contadorParadas] = atoi(gtk_entry_get_text(GTK_ENTRY(paradasEntry)));
	accionesTemp[contadorParadas] = accion_de_parada;

	contadorParadas++;
	gtk_entry_set_text(GTK_ENTRY(paradasEntry), "");
	gtk_widget_set_sensitive (add_car_button ,true);
}//SELECCIONAR PARADAS

void accion_combustible(){
	accion_de_parada =  1;
}
void accion_llantas(){
	accion_de_parada =  2;
}
void accion_frenos(){
	accion_de_parada =  3;
}
void accion_temp(){
	accion_de_parada =  4;
}

///////////////////////////////////////////////////////////////Truck/////////////////////////////////
void add_truck1(GtkWidget *widget,gpointer data){
	int temp_truck_stops[2]= {16, 113};

	Car *truck = malloc(sizeof(Car));

	truck->speed=15000;
	truck->type = 0;
	truck->alive = 1;
	truck->speed = 15000;
	truck->matrizx=getx(temp_truck_stops[0]);
	truck->matrizy=getY(temp_truck_stops[0]);
	truck->axisX = truck->matrizx*25;
	truck->axisY = truck->matrizy*25;
	truck->d = 2;
	truck->run = 1;
	truck->paso = 1;

	truck->cargo=4;
	truck->dock=1;
	truck->priority = 1;
	truck->salida = 16;

	truck->id="C";
	truck->type=2;
	truck->ide = count;

	for (int i = 0; i < 2; ++i){
		truck->vparadas[i]=temp_truck_stops[i];
		truck->accionparadas[i]=7;
	}


	//Se inician los hilos
	carros[count] = truck;
	pthread_create(&threads_carros[count], NULL, move_truck, (void *)carros[count]);
	count++;

	contadorParadas = 0;
}//add Truck

void add_truck2(GtkWidget *widget,gpointer data){
	int temp_truck_stops[2]= {23, 112};

	//Se crea instancia del carro
	Car *truck = malloc(sizeof(Car));

	truck->speed=15000;
	truck->type = 0;
	truck->alive = 1;
	truck->speed = 15000;
	truck->matrizx=getx(temp_truck_stops[0]);
	truck->matrizy=getY(temp_truck_stops[0]);
	truck->axisX = truck->matrizx*25;
	truck->axisY = truck->matrizy*25;
	truck->d = 2;
	truck->run = 1;
	truck->paso = 1;

	truck->cargo=4;
	truck->dock=2;
	truck->priority = 1;
	truck->salida = 23;

	truck->id="C";
	truck->type=2;

	for (int i = 0; i < 2; ++i){
		truck->vparadas[i]=temp_truck_stops[i];
		truck->accionparadas[i]=7;
	}
	truck->ide = count;

	//Se inician los hilos
	carros[count] = truck;
	pthread_create(&threads_carros[count], NULL, move_truck, (void *)carros[count]);
	count++;

	contadorParadas = 0;
}//add Truck

/////////////////////////////////////////////////////////////////BARCO/////////////////////////////////
void add_Ship(GtkWidget *widget,gpointer data){

	create_ship(2, atoi(gtk_entry_get_text(GTK_ENTRY(ship_cargo_entry))));


}//add ship

void add_Ship2(GtkWidget *widget,gpointer data){

	create_ship(1, atoi(gtk_entry_get_text(GTK_ENTRY(ship_cargo_entry))));

}//add ship

/////////////////////////////////////////BUS///////////////////////////////////
void add_bus(GtkWidget *widget,gpointer data){
	//selecciona la velocidad
	if(selectedSpeed == 1){
		selectedSpeed = 50000;
	}else if(selectedSpeed == 2){
		selectedSpeed = 15000;
	}else if(selectedSpeed == 3){
		selectedSpeed = 9000;
	}

	//Se crea instancia del carro
	Car *bus = malloc(sizeof(Car));
	bus->type = 1;
	bus->alive = 1;
	bus->speed =  selectedSpeed;
	bus->axisX = carx*25;
	bus->axisY = cary*25;
	bus->matrizx = carx;
	bus->matrizy = cary;
	bus->d = 1;
	bus->run = 1;
	bus->paso = 1;
	bus->personasActual = 0;
	bus->id = "B";
	bus->cantidadParadas = contadorParadas+1;
	bus->capacidadPersonas = 30;
	for(int i = 0; i<=contadorParadas; i++){
		if(i == 0){
			bus->vparadas[i] = salida;
		}else{
			bus->vparadas[i] = tempParadas[i-1];

		}
	}
	bus->salida = salida;
	bus->ide = count;
	//Se inician los hilos
	carros[count] = bus;
	pthread_create(&threads_carros[count], NULL, move_car, (void *)carros[count]);
	count++;

	contadorParadas = 0;
}//add ship

/////////////////////////////////////////botones de tren//////////////////////
void add_wagon(GtkWidget *widget,gpointer data){
	train->cantidadVagones++;
}//add WAGON

void delete_wagon(GtkWidget *widget,gpointer data){
	if(train->cantidadVagones == 0){

	}else{
		train->cantidadVagones--;
	}
}//delete_wagon

void activate (GtkApplication* app,gpointer user_data){
	time_t t;
	srand((unsigned) time(&t));


	//se crea la ventana con sus caracteristicas
	window = gtk_application_window_new (app);
	//Codigo para setearle un titulo a la ventana
	gtk_window_set_title (GTK_WINDOW (window), "Ciudad Moderna");
	//Codigo para setearle un tamano a la ventana
	gtk_window_set_default_size (GTK_WINDOW (window), winWeigth, winHeigth);
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);

	//que se cierre con la X
	g_signal_connect (window, "destroy",
	G_CALLBACK (gtk_main_quit), NULL);

	//crea el fixed
	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER (window), fixed);

	//DRAWING AREA
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_area, 850, 700);
	g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);


	g_signal_connect (drawing_area,"configure-event",
	G_CALLBACK (configure_event_cb), NULL);
	g_signal_connect (drawing_area, "button-press-event",
	G_CALLBACK (button_press_event_cb), NULL);
	gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
	| GDK_BUTTON_PRESS_MASK);

	gtk_fixed_put(GTK_FIXED(fixed), drawing_area, 0,0);

	//////////////////////////////PANEL DERECHO//////////////////////////////////
	combo_label = gtk_label_new("Elija el objeto que quiera insertar: ");
	gtk_fixed_put(GTK_FIXED(fixed), combo_label, 860,10);

	//COMBO BOX
	combo_box = gtk_combo_box_text_new_with_entry();
	gtk_widget_set_size_request(GTK_COMBO_BOX_TEXT(combo_box), 50, 20);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"--");

	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"Carro");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"Bus");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"Barco");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"Camion");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box),"Tren");

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);
	g_signal_connect(combo_box, "changed", G_CALLBACK(check_selection), NULL);
	gtk_fixed_put(GTK_FIXED(fixed), combo_box, 860, 28);

	//////////////////////////////ADD CAR////////////////////////////////////////
	//ID LABEL
	idLabel = gtk_label_new("ID del carro: ");
	//ID ENTRY
	idEntry = gtk_entry_new ();
	gtk_entry_set_placeholder_text ( GTK_ENTRY (idEntry),"id");
	gtk_entry_set_max_length (GTK_ENTRY (idEntry), 3);
	gtk_entry_set_width_chars (GTK_ENTRY (idEntry), 3);

	//SPEED LABEL
	speedLabel = gtk_label_new("Escoja una velocidad: ");

	speedCB1 = gtk_check_button_new_with_label ("Lento");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB1), FALSE);
	g_signal_connect (GTK_TOGGLE_BUTTON (speedCB1), "toggled", G_CALLBACK (toggled_cb), window);

	speedCB2 = gtk_check_button_new_with_label ("Normal");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB2), FALSE);
	g_signal_connect (GTK_TOGGLE_BUTTON (speedCB2), "toggled", G_CALLBACK (toggled_cb), window);

	speedCB3 = gtk_check_button_new_with_label ("Rapido");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (speedCB3), FALSE);
	g_signal_connect (GTK_TOGGLE_BUTTON (speedCB3), "toggled", G_CALLBACK (toggled_cb), window);

	//SELECCIONAR PARADAS
	paradasCarLabel = gtk_label_new("ingrese el numero de parada: ");
	paradasEntry = gtk_entry_new ();
	gtk_entry_set_placeholder_text ( GTK_ENTRY (paradasEntry),"#");
	gtk_entry_set_max_length (GTK_ENTRY (paradasEntry), 3);
	gtk_entry_set_width_chars (GTK_ENTRY (paradasEntry), 3);

	button_add_stations = gtk_button_new_with_label("Agregar parada");
	g_signal_connect(button_add_stations, "clicked", G_CALLBACK(seleccionar_Paradas), NULL);

	add_car_button = gtk_button_new_with_label ("Crear Carro");
	g_signal_connect (add_car_button, "clicked", G_CALLBACK (add_Car),NULL);
	gtk_widget_set_sensitive (add_car_button ,false);

	revisar_combustible_btn = gtk_button_new_with_label ("Combustible");
	g_signal_connect (revisar_combustible_btn, "clicked", G_CALLBACK (accion_combustible),NULL);

	revisar_frenos_btn = gtk_button_new_with_label ("Frenos");
	g_signal_connect (revisar_frenos_btn, "clicked", G_CALLBACK (accion_frenos),NULL);

	revisar_llantas_btn = gtk_button_new_with_label ("llantas");
	g_signal_connect (revisar_llantas_btn, "clicked", G_CALLBACK (accion_llantas),NULL);

	revisar_temp_btn = gtk_button_new_with_label ("Temperatura");
	g_signal_connect (revisar_temp_btn, "clicked", G_CALLBACK (accion_temp),NULL);
	///////////////////////ADD CAMMION///////////////////////////////////////////
	prioridadLabel = gtk_label_new("ingrese prioridad del Camion: ");
	prioridadEntry = gtk_entry_new ();
	gtk_entry_set_placeholder_text ( GTK_ENTRY (prioridadEntry),"#");
	gtk_entry_set_max_length (GTK_ENTRY (prioridadEntry), 3);
	gtk_entry_set_width_chars (GTK_ENTRY (prioridadEntry), 3);

	add_truck_button1 = gtk_button_new_with_label("Crear para muelle 1");
	g_signal_connect(add_truck_button1, "clicked", G_CALLBACK(add_truck1),NULL);

	add_truck_button2 = gtk_button_new_with_label("Crear para muelle 2");
	g_signal_connect(add_truck_button2, "clicked", G_CALLBACK(add_truck2),NULL);
	///////////////////////////////////////BARCO//////////////////////////////////////

	ship_cargo_label = gtk_label_new("ingrese la carga del barco: ");
	ship_cargo_entry = gtk_entry_new ();
	gtk_entry_set_placeholder_text ( GTK_ENTRY (ship_cargo_entry),"#");
	gtk_entry_set_max_length (GTK_ENTRY (ship_cargo_entry), 3);
	gtk_entry_set_width_chars (GTK_ENTRY (ship_cargo_entry), 3);
	add_ship_button = gtk_button_new_with_label("Crear Barco muelle A1");
	g_signal_connect(add_ship_button, "clicked", G_CALLBACK(add_Ship), NULL);

	add_ship_button2 = gtk_button_new_with_label("Crear Barco muelle A2");
	g_signal_connect(add_ship_button2, "clicked", G_CALLBACK(add_Ship2), NULL);

	///////////////////////ADD BUS////////////////////////////////////////////////
	add_bus_button = gtk_button_new_with_label("Crear Bus");
	g_signal_connect(add_bus_button, "clicked", G_CALLBACK(add_bus), NULL);

	////////////////////////////Train//////////////////////////////////////////
	add_wagon_button = gtk_button_new_with_label("Agregar Vagon");
	g_signal_connect(add_wagon_button, "clicked", G_CALLBACK (add_wagon),NULL);

	delete_wagon_button = gtk_button_new_with_label("Quitar Vagon");
	g_signal_connect(delete_wagon_button, "clicked", G_CALLBACK (delete_wagon),NULL);

	//TREN
	gtk_fixed_put(GTK_FIXED(fixed), add_wagon_button, 860, 200);
	gtk_fixed_put(GTK_FIXED(fixed), delete_wagon_button, 860, 240);
	//Barco
	gtk_fixed_put (GTK_FIXED (fixed), add_ship_button,860, 300);
	gtk_fixed_put (GTK_FIXED (fixed), add_ship_button2,860, 270);
	gtk_fixed_put (GTK_FIXED (fixed), ship_cargo_entry, 860, 200);
	gtk_fixed_put (GTK_FIXED (fixed), ship_cargo_label, 860, 175);

	//Bus
	gtk_fixed_put (GTK_FIXED (fixed), add_bus_button,860, 400);

	//CAMION
	gtk_fixed_put(GTK_FIXED(fixed), prioridadLabel, 860, 120);
	gtk_fixed_put(GTK_FIXED(fixed), prioridadEntry, 940, 140);

	gtk_fixed_put (GTK_FIXED (fixed), add_truck_button1,900, 200);
	gtk_fixed_put (GTK_FIXED (fixed), add_truck_button2,900, 230);

	///////CARRO
	gtk_fixed_put(GTK_FIXED(fixed), idLabel, 860, 66);
	gtk_fixed_put (GTK_FIXED (fixed), idEntry, 945, 60);
	gtk_fixed_put(GTK_FIXED(fixed), speedLabel, 860, 90);
	gtk_fixed_put(GTK_FIXED(fixed), speedCB1, 860, 115);
	gtk_fixed_put(GTK_FIXED(fixed), speedCB2, 860, 135);
	gtk_fixed_put(GTK_FIXED(fixed), speedCB3, 860, 155);
	gtk_fixed_put(GTK_FIXED(fixed), paradasCarLabel, 860, 350);
	gtk_fixed_put(GTK_FIXED(fixed), paradasEntry, 860, 370);
	gtk_fixed_put(GTK_FIXED(fixed), button_add_stations, 900, 370);

	gtk_fixed_put(GTK_FIXED(fixed), revisar_combustible_btn,860, 200);
	gtk_fixed_put(GTK_FIXED(fixed), revisar_llantas_btn,860, 240);
	gtk_fixed_put(GTK_FIXED(fixed), revisar_frenos_btn,860, 280);
	gtk_fixed_put(GTK_FIXED(fixed), revisar_temp_btn,860, 320);

	gtk_fixed_put (GTK_FIXED (fixed), add_car_button,920, 405);


	crearParadas(); // CREA UN ARREGLO DE PARADAS

	pthread_t sem;
	pthread_create(&sem, NULL , semaforo , NULL);

	pthread_t repair_thread;
	pthread_create(&repair_thread, NULL,repair_generator, NULL );

	create_train();

	gtk_widget_show(window);
	gtk_widget_show(drawing_area);
	gtk_widget_show(combo_label);
	gtk_widget_show(combo_box);
	gtk_widget_show(fixed);

	gtk_main();

}//Activate

int main (int argc, char **argv){

	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}//main

void hide_all(){
	hide_car_widgets();
	hide_bus_widgets();
	hide_ship_widgets();
	hide_train_widgets();
	hide_truck_widgets();
}

void show_car_widgets(){
	gtk_widget_show(add_car_button);
	gtk_widget_show(idLabel);
	gtk_widget_show(idEntry);
	gtk_widget_show(speedLabel);
	gtk_widget_show(speedCB1);
	gtk_widget_show(speedCB2);
	gtk_widget_show(speedCB3);
	gtk_widget_show(button_add_stations);
	gtk_widget_show(paradasCarLabel);
	gtk_widget_show(paradasEntry);
	gtk_widget_show(add_car_button);
	gtk_widget_show(revisar_combustible_btn);
	gtk_widget_show(revisar_frenos_btn);
	gtk_widget_show(revisar_llantas_btn);
	gtk_widget_show(revisar_temp_btn);
}//show_car_widgets

void hide_car_widgets(){
	gtk_widget_hide(add_car_button);
	gtk_widget_hide(idLabel);
	gtk_widget_hide(idEntry);
	gtk_widget_hide(speedLabel);
	gtk_widget_hide(speedCB1);
	gtk_widget_hide(speedCB2);
	gtk_widget_hide(speedCB3);
	gtk_widget_hide(button_add_stations);
	gtk_widget_hide(paradasCarLabel);
	gtk_widget_hide(paradasEntry);
	gtk_widget_hide(add_car_button);
	gtk_widget_hide(revisar_combustible_btn);
	gtk_widget_hide(revisar_frenos_btn);
	gtk_widget_hide(revisar_llantas_btn);
	gtk_widget_hide(revisar_temp_btn);
}//hide_car_widgets

void show_truck_widgets(){
	gtk_widget_show(add_truck_button1);
	gtk_widget_show(add_truck_button2);
	gtk_widget_show(prioridadLabel);
	gtk_widget_show(prioridadEntry);
}//show_truck_widgets

void hide_truck_widgets(){
	gtk_widget_hide(add_truck_button1);
	gtk_widget_hide(add_truck_button2);
	gtk_widget_hide(prioridadLabel);
	gtk_widget_hide(prioridadEntry);
}//hide_truck_widgets

void show_bus_widgets(){
	gtk_widget_show(speedLabel);
	gtk_widget_show(speedCB1);
	gtk_widget_show(speedCB2);
	gtk_widget_show(speedCB3);
	gtk_widget_show(add_bus_button);
	gtk_widget_show(button_add_stations);
	gtk_widget_show(paradasCarLabel);
	gtk_widget_show(paradasEntry);
}//show_bus_widgets

void hide_bus_widgets(){
	gtk_widget_hide(add_bus_button);
	gtk_widget_hide(speedLabel);
	gtk_widget_hide(speedCB1);
	gtk_widget_hide(speedCB2);
	gtk_widget_hide(speedCB3);
}//hide_bus_widgets

void show_ship_widgets(){
	gtk_widget_show(add_ship_button);
	gtk_widget_show(add_ship_button2);
	gtk_widget_show(add_ship_button);
	gtk_widget_show(ship_cargo_entry);
	gtk_widget_show(ship_cargo_label);
}//show_ship_widget

void hide_ship_widgets(){
	gtk_widget_hide(add_ship_button);
	gtk_widget_hide(add_ship_button2);
	gtk_widget_hide(add_ship_button);
	gtk_widget_hide(ship_cargo_entry);
	gtk_widget_hide(ship_cargo_label);
}//hide_ship_widgets

void show_train_widgets(){
	gtk_widget_show(add_wagon_button);
	gtk_widget_show(delete_wagon_button);
}//show_train_widgets

void hide_train_widgets(){
	gtk_widget_hide(add_wagon_button);
	gtk_widget_hide(delete_wagon_button);
}//hide_train_widgets
