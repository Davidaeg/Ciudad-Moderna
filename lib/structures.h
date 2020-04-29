//Structuras
typedef struct
{
		int type;
		//bus
		int capacidadPersonas;
		int personasActual;
		//Truck
		int priority;
		int dock;
		int cargo;

    int alive;
    int speed;
    int axisX;
    int axisY;
    int matrizx;
    int matrizy;
    char* id;
    int cantidadParadas;
    int direccion[112];
    int vparadas[112];
    int accionparadas[112];
    int salida;
    int d;
		int run;
    int paso;
		int ide;
}Car;

struct parada
{
	int x;
	int y;
	int id;
	int siguientes[3];
	int axisX;
	int axisY;
	int action;
};

typedef struct
{
	int id;
	int run;
	int matrizx;
	int matrizy;
	int axisX;
	int axisY;
	int cargo;
	int dock;
	int desapair;
}Ship;

typedef struct Repair{
	int axisX;
	int axisY;
	int x;
	int y;
}Repair;


typedef struct
{
    int cargo;
    int axisX;
    int axisY;
    int matrizx;
    int matrizy;
    int cantidadVagones;
    int run;
}Train;

Repair* repair;

int cambiarEstado = 0;
int puente_ocupado = 0;
//vector de carros
Car* carros[100];
//vector de threads
pthread_t threads_carros[100];
//hilo Tren
pthread_t tTrain;
//para barcos
int ships_number=0;
Ship* ships[25];
pthread_t ship_thread[25];

int stopa1 = 0;
int stopa2 = 0;

int a1cargo = 0;
int a2cargo = 0;

int block=0;
int lock = 1;

bool interrupt=true;
