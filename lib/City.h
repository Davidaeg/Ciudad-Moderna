#include "structures.h"


//Tamanno de la ventana
#define winWeigth 1050
#define winHeigth 700

//filas y columnas de la matriz
#define rows 28
#define columns 34

#define INFINITY 9999

void setRelaciones();

struct parada* paradas[114];

int graph[114][114];

void crearGrafo(){
	for (int i = 0; i < 112; ++i){
		for (int j = 0; j < 112; ++j){
			graph[i][j]=0;
		}
	}

	for (int id = 0; id < 112; ++id){
		for (int i = 0; i < 2; ++i){
			if (paradas[id]->siguientes[i]!=-1){
				graph[id][paradas[id]->siguientes[i]]=1;
			}
		}

	}
}//crearGrafo

void crearGrafo2(){
	for (int i = 0; i < 114; ++i){
		for (int j = 0; j < 114; ++j){
			graph[i][j]=0;
		}
	}

	for (int id = 0; id < 114; ++id){
		for (int i = 0; i < 2; ++i){
			if (paradas[id]->siguientes[i]!=-1){
				graph[id][paradas[id]->siguientes[i]]=1;
			}
		}

	}
}//crearGrafo

void truck_stops(){
	paradas[112] = malloc(sizeof(struct parada));
	paradas[112]->id=112;
	paradas[112]->x=23;
	paradas[112]->y=13;

	paradas[112]->axisX=paradas[112]->x*25;
	paradas[112]->axisY=paradas[112]->y*25;

	paradas[113] = malloc(sizeof(struct parada));
	paradas[113]->id=113;
	paradas[113]->x=11;
	paradas[113]->y=14;

	paradas[113]->axisX=paradas[113]->x*25;
	paradas[113]->axisY=paradas[113]->y*25;

}

/*
	Calle ----> 0
   	agua -----> 2
   	Cuadra ---> 1
  	Puente ---> 3
   	P.Elev ---> 5
   	Almacen1 -> 4
   	Almacen2 -> 6
   	block ---> 8

*/

int cityMatrix[rows][columns]={
//     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
/*0*/ {0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 },
/*1*/ {0 ,4 ,4 ,4 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,6 ,6 ,6 ,0 },
/*2*/ {8 ,4 ,4 ,4 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,6 ,6 ,6 ,8 },
/*3*/ {0 ,4 ,4 ,4 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,6 ,6 ,6 ,0 },
/*4*/ {0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,30,31,0 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 },
/*5*/ {0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,0 ,32,30,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 },
/*6*/ {0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*7*/ {8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 },
/*8*/ {0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*9*/ {0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,7 ,7 ,7 ,5 ,5 ,7 ,7 ,7 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 },
/*10*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,7 ,7 ,7 ,7 ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,7 ,7 ,7 ,7 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*11*/{8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 },
/*12*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,7 ,2 ,2 ,6 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,4 ,2 ,2 ,7 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*13*/{0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,7 ,6 ,6 ,6, 2, 2, 2, 2, 2, 2, 2 ,2 ,4 ,4 ,4 ,7 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 },
/*14*/{0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,7 ,6 ,6 ,6, 2, 2, 2, 2, 2, 2, 2 ,2 ,4 ,4 ,4 ,7 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 },
/*15*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,7 ,2 ,2 ,6 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,4 ,2 ,2 ,7 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*16*/{8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 },
/*17*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,7 ,7 ,7 ,7 ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,7 ,7 ,7 ,7 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*18*/{0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,7 ,7 ,7 ,5 ,5 ,7 ,7 ,7 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 },
/*19*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*20*/{8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 },
/*21*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*22*/{0 ,0 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,33,33,0 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 ,9 ,8 ,0 ,0 },
/*23*/{0 ,9 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,0 ,33,33,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 ,0 ,8 ,9 ,0 },
/*24*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*25*/{8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 ,1 ,1 ,1 ,8 },
/*26*/{0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,2 ,2 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 },
/*27*/{0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,8 ,0 ,0 }

} ;

static void drawingCity (cairo_t * cr){
	char* buf[3];
	int count = 0;
  	for (int i = 0; i < rows; i++)
  	{
  		for (int j = 0; j < columns; j++)
  		{
  			if(cambiarEstado == 0){
  				if(cityMatrix[i][j] == 31){

  					cairo_set_source_rgb (cr, 0.48, 0.98, 0);
  					cairo_rectangle (cr, j*25, i*25, 25, 25);
  					cairo_fill (cr);
  				}

  				if (cityMatrix[i][j] == 32)
  				{
  					cairo_set_source_rgb (cr, 1, 0, 0);
  					cairo_rectangle (cr, j*25, i*25, 25, 25);
  					cairo_fill (cr);
  				}
  			}else{
  				if(cityMatrix[i][j] == 31){

  					cairo_set_source_rgb (cr, 1, 0, 0);
  					cairo_rectangle (cr, j*25, i*25, 25, 25);
  					cairo_fill (cr);
  				}

  				if (cityMatrix[i][j] == 32)
  				{
  					cairo_set_source_rgb (cr, 0.48, 0.98, 0);
  					cairo_rectangle (cr, j*25, i*25, 25, 25);
  					cairo_fill (cr);
  				}
  			}



  			if(cityMatrix[i][j] == 0 || cityMatrix[i][j] == 8 || cityMatrix[i][j] == 9 ){


  				cairo_set_source_rgb (cr, 0.3, 0.3, 0.3);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  				if(cityMatrix[i][j] == 8 ){
  					cairo_set_source_rgb (cr, 1,1,1);
  					cairo_set_font_size(cr,9);
    				cairo_move_to(cr, (j*25)+8, (i*25)+16);

    				sprintf(buf, "%d", count);

    				cairo_show_text(cr, buf);
    				count ++;
  				}

  			}else if(cityMatrix[i][j] == 2){

  				cairo_set_source_rgb (cr, 0, 0, 1);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  			}else if(cityMatrix[i][j] == 1){
  				cairo_set_source_rgb (cr, 0.102,0.205,0.170);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  			}else if(cityMatrix[i][j] == 4){
  				cairo_set_source_rgb (cr, 0.188,0.238,0.104);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  			}else if(cityMatrix[i][j] == 6){

  				cairo_set_source_rgb (cr, 0.210,0.105,0.30);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  				if(cityMatrix[i][j] == 18 ){
  					cairo_set_source_rgb (cr, 1,1,1);
  					cairo_set_font_size(cr,9);
    				cairo_move_to(cr, (j*25)+8, (i*25)+16);

    				sprintf(buf, "%d", count);

    				cairo_show_text(cr, buf);
    				count ++;
  				}

  			}else if(cityMatrix[i][j] == 7){

  				cairo_set_source_rgb (cr, 0.10,0.10,0.10);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  			}else if(cityMatrix[i][j] == 5){

  				cairo_set_source_rgb (cr, 0.10,0.30,0.10);
  				cairo_rectangle (cr, j*25, i*25, 25, 25);
  				cairo_fill (cr);

  			}

  			if(i == 2 && j == 2){
  				cairo_set_source_rgb (cr, 1,1,1);
  				cairo_set_font_size(cr,16);
    			cairo_move_to(cr, (j*25)+8, (i*25)+16);

    			sprintf(buf, "A%d", 1);

    			cairo_show_text(cr, buf);
  			}

  			if (i == 2 && j == 31){
  				cairo_set_source_rgb (cr, 1,1,1);
  				cairo_set_font_size(cr,16);
    			cairo_move_to(cr, (j*25), (i*25)+16);

    			sprintf(buf, "A%d", 2);

    			cairo_show_text(cr, buf);
  			}

  		}//for j
  	}//for i
}//Fin drawingCity

void crearParadas(){
	int count = 0;
	for (int i = 0; i < rows; i++)
  	{
  		for (int j = 0; j < columns; j++)
  		{
  			if(cityMatrix[i][j] == 8){
  				paradas[count] = malloc(sizeof(struct parada));

  				paradas[count]->x = j;
  				paradas[count]->y = i;
  				paradas[count]->id = count;
  				paradas[count]->axisX = j*25;
  				paradas[count]->axisY = i*25;

  				count++;
  			}
  		}
  	}
		truck_stops();
  	setRelaciones();
}//CrearParadas

void dijkstra(int n,int startnode, int finalnode, Car* car)
{

 	//printf("%d, %d, %d\n", n, startnode, finalnode);
    int cost[114][114],distance[114],pred[114];
    int visited[114],count,mindistance,nextnode,i,j;

		if(car->type==2) {
			crearGrafo2();
		} else{
			crearGrafo();
		}
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(graph[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=graph[i][j];

    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }

    distance[startnode]=0;
    visited[startnode]=1;
    count=1;

    while(count<n-1)
    {
        mindistance=INFINITY;

        for(i=0;i<n;i++)
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }

            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    }

    int contador = 0;

    for(i=0;i<n;i++){
    	if(i==finalnode)
        {
        	int temproute[distance[i]+1];
        	temproute[contador] = finalnode;
        	contador++;
            j=i;
            do
            {
                j=pred[j];
                temproute[contador] = j;
                contador++;
            }while(j!=startnode);

            int reversa = contador-1;

    		for (int i = 0; i < contador; ++i)
    		{
    			car->direccion[i] = temproute[reversa];
    			reversa--;
    			//cp++;
    		}

    	}
    }
}//dijkstra


///Semaforo en el puente de arriba
void *semaforo (void *arg){
	while(1){
		sleep(10);

		if(cambiarEstado == 0){
			cambiarEstado = 1;
		}else{
			cambiarEstado = 0;
		}

	}//while

}//semaforo

void able(){
	puente_ocupado = 0;
}//habilida el puente

void disable(){
	puente_ocupado = 1;
}//deshabilita el puente

//pinta las reparaciones
void draw_repair(cairo_t *cr){

	cairo_set_source_rgb (cr, 0.533, 0.592, 0.301);
	cairo_rectangle (cr,repair->axisX ,repair->axisY, 25, 25);
	cairo_fill (cr);
	cairo_move_to(cr, repair->axisX+8, repair->axisY+16);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_show_text(cr, "R");

}//draw_Repair

void draw_ships(cairo_t *cr){
	for (int i = 0; i < ships_number; ++i){
		if(ships[i]->run){
			if(ships[i]->desapair == 1){

			}else{
				if(ships[i]->dock==1){
					cairo_set_source_rgb (cr, 0.210,0.105,0.30);
				}else{

					cairo_set_source_rgb (cr, 0.188,0.238,0.104);
				}
				cairo_rectangle (cr,ships[i]->axisX ,ships[i]->axisY, 25, 25);
				cairo_fill (cr);
			}
		}
	}
}//draw_ships

void draw_dock_cargo(cairo_t *cr){
		char* buffer[3];
		cairo_set_source_rgb (cr, 1, 1, 1);

		cairo_move_to (cr,(12*25)+10,(12*25)+(25/2));
		sprintf(buffer, "%d", a1cargo);
		cairo_show_text (cr,buffer);
		sprintf(buffer, "%d", a2cargo);
		cairo_move_to (cr,(21*25)+10,(12*25)+(25/2));
		cairo_show_text (cr,buffer);
}//draw

void setRelaciones(){
	paradas[0]->siguientes[0]=1;
	paradas[0]->siguientes[1]=9;
	paradas[0]->siguientes[2]=-1;

	paradas[1]->siguientes[0]=2;
	paradas[1]->siguientes[1]=10;
	paradas[1]->siguientes[2]=-1;

	paradas[2]->siguientes[0]=3;
	paradas[2]->siguientes[1]=11;
	paradas[2]->siguientes[2]=-1;

	paradas[3]->siguientes[0]=4;
	paradas[3]->siguientes[1]=-1;
	paradas[3]->siguientes[2]=-1;

	paradas[4]->siguientes[0]=5;
	paradas[4]->siguientes[1]=12;
	paradas[4]->siguientes[2]=-1;

	paradas[5]->siguientes[0]=6;
	paradas[5]->siguientes[1]=13;
	paradas[5]->siguientes[2]=-1;

	paradas[6]->siguientes[0]=7;
	paradas[6]->siguientes[1]=14;
	paradas[6]->siguientes[2]=-1;

	paradas[7]->siguientes[0]=15;
	paradas[7]->siguientes[1]=-1;
	paradas[7]->siguientes[2]=-1;

	paradas[8]->siguientes[0]=0;
	paradas[8]->siguientes[1]=-1;
	paradas[8]->siguientes[2]=-1;

	paradas[9]->siguientes[0]=16;
	paradas[9]->siguientes[1]=33;
	paradas[9]->siguientes[2]=25;

	paradas[10]->siguientes[0]=17;
	paradas[10]->siguientes[1]=34;
	paradas[10]->siguientes[2]=-1;

	paradas[11]->siguientes[0]=18;
	paradas[11]->siguientes[1]=35;
	paradas[11]->siguientes[2]=27;

	paradas[12]->siguientes[0]=20;
	paradas[12]->siguientes[1]=36;
	paradas[12]->siguientes[2]=29;

	paradas[13]->siguientes[0]=21;
	paradas[13]->siguientes[1]=37;
	paradas[13]->siguientes[2]=30;

	paradas[14]->siguientes[0]=22;
	paradas[14]->siguientes[1]=38;
	paradas[14]->siguientes[2]=31;

	paradas[15]->siguientes[0]=23;
	paradas[15]->siguientes[1]=39;
	paradas[15]->siguientes[2]=-1;

	paradas[16]->siguientes[0]=8;
	paradas[16]->siguientes[1]=-1;
	paradas[16]->siguientes[2]=-1;

	paradas[17]->siguientes[0]=16;
	paradas[17]->siguientes[1]=33;
	paradas[17]->siguientes[2]=-1;

	paradas[18]->siguientes[0]=17;
	paradas[18]->siguientes[1]=34;
	paradas[18]->siguientes[2]=-1;

	paradas[19]->siguientes[0]=18;
	paradas[19]->siguientes[1]=35;
	paradas[19]->siguientes[2]=-1;

	paradas[20]->siguientes[0]=19;
	paradas[20]->siguientes[1]=-1;
	paradas[20]->siguientes[2]=-1;

	paradas[21]->siguientes[0]=20;
	paradas[21]->siguientes[1]=36;
	paradas[21]->siguientes[2]=-1;

	paradas[22]->siguientes[0]=21;
	paradas[22]->siguientes[1]=37;
	paradas[22]->siguientes[2]=-1;

	paradas[23]->siguientes[0]=22;
	paradas[23]->siguientes[1]=38;
	paradas[23]->siguientes[2]=-1;

	paradas[24]->siguientes[0]=25;
	paradas[24]->siguientes[1]=33;
	paradas[24]->siguientes[2]=-1;

	paradas[25]->siguientes[0]=26;
	paradas[25]->siguientes[1]=34;
	paradas[25]->siguientes[2]=-1;

	paradas[26]->siguientes[0]=27;
	paradas[26]->siguientes[1]=35;
	paradas[26]->siguientes[2]=-1;

	paradas[27]->siguientes[0]=28;
	paradas[27]->siguientes[1]=-1;
	paradas[27]->siguientes[2]=-1;

	paradas[28]->siguientes[0]=29;
	paradas[28]->siguientes[1]=36;
	paradas[28]->siguientes[2]=-1;

	paradas[29]->siguientes[0]=30;
	paradas[29]->siguientes[1]=37;
	paradas[29]->siguientes[2]=-1;

	paradas[30]->siguientes[0]=31;
	paradas[30]->siguientes[1]=38;
	paradas[30]->siguientes[2]=-1;

	paradas[31]->siguientes[0]=39;
	paradas[31]->siguientes[1]=-1;
	paradas[31]->siguientes[2]=-1;

	paradas[32]->siguientes[0]=8;
	paradas[32]->siguientes[1]=24;
	paradas[32]->siguientes[2]=-1;

	paradas[33]->siguientes[0]=40;
	paradas[33]->siguientes[1]=47;
	paradas[33]->siguientes[2]=-1;

	paradas[34]->siguientes[0]=41;
	paradas[34]->siguientes[1]=48;
	paradas[34]->siguientes[2]=-1;

	paradas[35]->siguientes[0]=42;
	paradas[35]->siguientes[1]=-1;
	paradas[35]->siguientes[2]=-1;

	paradas[36]->siguientes[0]=43;
	paradas[36]->siguientes[1]=-1;
	paradas[36]->siguientes[2]=-1;

	paradas[37]->siguientes[0]=44;
	paradas[37]->siguientes[1]=49;
	paradas[37]->siguientes[2]=-1;

	paradas[38]->siguientes[0]=45;
	paradas[38]->siguientes[1]=50;
	paradas[38]->siguientes[2]=-1;

	paradas[39]->siguientes[0]=51;
	paradas[39]->siguientes[1]=-1;
	paradas[39]->siguientes[2]=-1;

	paradas[40]->siguientes[0]=32;
	paradas[40]->siguientes[1]=-1;
	paradas[40]->siguientes[2]=-1;

	paradas[41]->siguientes[0]=40;
	paradas[41]->siguientes[1]=47;
	paradas[41]->siguientes[2]=-1;

	paradas[42]->siguientes[0]=41;
	paradas[42]->siguientes[1]=48;
	paradas[42]->siguientes[2]=-1;

	paradas[43]->siguientes[0]=44;
	paradas[43]->siguientes[1]=-1;
	paradas[43]->siguientes[2]=-1;

	paradas[44]->siguientes[0]=45;
	paradas[44]->siguientes[1]=50;
	paradas[44]->siguientes[2]=-1;

	paradas[45]->siguientes[0]=51;
	paradas[45]->siguientes[1]=-1;
	paradas[45]->siguientes[2]=-1;

	paradas[46]->siguientes[0]=32;
	paradas[46]->siguientes[1]=-1;
	paradas[46]->siguientes[2]=-1;

	paradas[47]->siguientes[0]=52;
	paradas[47]->siguientes[1]=-1;
	paradas[47]->siguientes[2]=-1;

	paradas[48]->siguientes[0]=53;
	paradas[48]->siguientes[1]=62;
	paradas[48]->siguientes[2]=-1;

	paradas[49]->siguientes[0]=44;
	paradas[49]->siguientes[1]=-1;
	paradas[49]->siguientes[2]=-1;

	paradas[50]->siguientes[0]=54;
	paradas[50]->siguientes[1]=59;
	paradas[50]->siguientes[2]=-1;

	paradas[51]->siguientes[0]=55;
	paradas[51]->siguientes[1]=65;
	paradas[51]->siguientes[2]=-1;

	paradas[52]->siguientes[0]=46;
	paradas[52]->siguientes[1]=-1;
	paradas[52]->siguientes[2]=-1;

	paradas[53]->siguientes[0]=52;
	paradas[53]->siguientes[1]=-1;
	paradas[53]->siguientes[2]=-1;

	paradas[54]->siguientes[0]=49;
	paradas[54]->siguientes[1]=-1;
	paradas[54]->siguientes[2]=-1;

	paradas[55]->siguientes[0]=54;
	paradas[55]->siguientes[1]=-1;
	paradas[55]->siguientes[2]=-1;

	paradas[56]->siguientes[0]=57;
	paradas[56]->siguientes[1]=-1;
	paradas[56]->siguientes[2]=-1;

	paradas[57]->siguientes[0]=62;
	paradas[57]->siguientes[1]=-1;
	paradas[57]->siguientes[2]=-1;

	paradas[58]->siguientes[0]=59;
	paradas[58]->siguientes[1]=-1;
	paradas[58]->siguientes[2]=-1;

	paradas[59]->siguientes[0]=65;
	paradas[59]->siguientes[1]=-1;
	paradas[59]->siguientes[2]=-1;

	paradas[60]->siguientes[0]=46;
	paradas[60]->siguientes[1]=56;
	paradas[60]->siguientes[2]=-1;

	paradas[61]->siguientes[0]=57;
	paradas[61]->siguientes[1]=52;
	paradas[61]->siguientes[2]=-1;

	paradas[62]->siguientes[0]=67;
	paradas[62]->siguientes[1]=-1;
	paradas[62]->siguientes[2]=-1;

	paradas[63]->siguientes[0]=58;
	paradas[63]->siguientes[1]=49;
	paradas[63]->siguientes[2]=-1;

	paradas[64]->siguientes[0]=59;
	paradas[64]->siguientes[1]=-1;
	paradas[64]->siguientes[2]=-1;

	paradas[65]->siguientes[0]=79;
	paradas[65]->siguientes[1]=-1;
	paradas[65]->siguientes[2]=-1;

	paradas[66]->siguientes[0]=60;
	paradas[66]->siguientes[1]=-1;
	paradas[66]->siguientes[2]=-1;

	paradas[67]->siguientes[0]=61;
	paradas[67]->siguientes[1]=66;
	paradas[67]->siguientes[2]=-1;

	paradas[68]->siguientes[0]=67;
	paradas[68]->siguientes[1]=-1;
	paradas[68]->siguientes[2]=-1;

	paradas[69]->siguientes[0]=63;
	paradas[69]->siguientes[1]=70;
	paradas[69]->siguientes[2]=-1;

	paradas[70]->siguientes[0]=64;
	paradas[70]->siguientes[1]=71;
	paradas[70]->siguientes[2]=-1;

	paradas[71]->siguientes[0]=79;
	paradas[71]->siguientes[1]=-1;
	paradas[71]->siguientes[2]=-1;

	paradas[72]->siguientes[0]=60;
	paradas[72]->siguientes[1]=-1;
	paradas[72]->siguientes[2]=-1;

	paradas[73]->siguientes[0]=61;
	paradas[73]->siguientes[1]=66;
	paradas[73]->siguientes[2]=-1;

	paradas[74]->siguientes[0]=67;
	paradas[74]->siguientes[1]=-1;
	paradas[74]->siguientes[2]=-1;

	paradas[75]->siguientes[0]=68;
	paradas[75]->siguientes[1]=-1;
	paradas[75]->siguientes[2]=-1;

	paradas[76]->siguientes[0]=69;
	paradas[76]->siguientes[1]=-1;
	paradas[76]->siguientes[2]=-1;

	paradas[77]->siguientes[0]=63;
	paradas[77]->siguientes[1]=70;
	paradas[77]->siguientes[2]=-1;

	paradas[78]->siguientes[0]=64;
	paradas[78]->siguientes[1]=71;
	paradas[78]->siguientes[2]=-1;

	paradas[79]->siguientes[0]=87;
	paradas[79]->siguientes[1]=103;
	paradas[79]->siguientes[2]=-1;

	paradas[80]->siguientes[0]=72;
	paradas[80]->siguientes[1]=-1;
	paradas[80]->siguientes[2]=-1;

	paradas[81]->siguientes[0]=73;
	paradas[81]->siguientes[1]=80;
	paradas[81]->siguientes[2]=-1;

	paradas[82]->siguientes[0]=74;
	paradas[82]->siguientes[1]=81;
	paradas[82]->siguientes[2]=-1;

	paradas[83]->siguientes[0]=75;
	paradas[83]->siguientes[1]=82;
	paradas[83]->siguientes[2]=-1;

	paradas[84]->siguientes[0]=83;
	paradas[84]->siguientes[1]=-1;
	paradas[84]->siguientes[2]=-1;

	paradas[85]->siguientes[0]=76;
	paradas[85]->siguientes[1]=84;
	paradas[85]->siguientes[2]=-1;

	paradas[86]->siguientes[0]=77;
	paradas[86]->siguientes[1]=85;
	paradas[86]->siguientes[2]=-1;

	paradas[87]->siguientes[0]=78;
	paradas[87]->siguientes[1]=86;
	paradas[87]->siguientes[2]=-1;

	paradas[88]->siguientes[0]=73;
	paradas[88]->siguientes[1]=89;
	paradas[88]->siguientes[2]=-1;

	paradas[89]->siguientes[0]=74;
	paradas[89]->siguientes[1]=90;
	paradas[89]->siguientes[2]=-1;

	paradas[90]->siguientes[0]=75;
	paradas[90]->siguientes[1]=91;
	paradas[90]->siguientes[2]=-1;

	paradas[91]->siguientes[0]=92;
	paradas[91]->siguientes[1]=-1;
	paradas[91]->siguientes[2]=-1;

	paradas[92]->siguientes[0]=76;
	paradas[92]->siguientes[1]=93;
	paradas[92]->siguientes[2]=-1;

	paradas[93]->siguientes[0]=77;
	paradas[93]->siguientes[1]=94;
	paradas[93]->siguientes[2]=-1;

	paradas[94]->siguientes[0]=78;
	paradas[94]->siguientes[1]=95;
	paradas[94]->siguientes[2]=-1;

	paradas[95]->siguientes[0]=103;
	paradas[95]->siguientes[1]=-1;
	paradas[95]->siguientes[2]=-1;

	paradas[96]->siguientes[0]=72;
	paradas[96]->siguientes[1]=88;
	paradas[96]->siguientes[2]=-1;

	paradas[97]->siguientes[0]=73;
	paradas[97]->siguientes[1]=89;
	paradas[97]->siguientes[2]=-1;

	paradas[98]->siguientes[0]=74;
	paradas[98]->siguientes[1]=90;
	paradas[98]->siguientes[2]=-1;

	paradas[99]->siguientes[0]=75;
	paradas[99]->siguientes[1]=91;
	paradas[99]->siguientes[2]=-1;

	paradas[100]->siguientes[0]=76;
	paradas[100]->siguientes[1]=93;
	paradas[100]->siguientes[2]=-1;

	paradas[101]->siguientes[0]=77;
	paradas[101]->siguientes[1]=94;
	paradas[101]->siguientes[2]=-1;

	paradas[102]->siguientes[0]=78;
	paradas[102]->siguientes[1]=95;
	paradas[102]->siguientes[2]=-1;

	paradas[103]->siguientes[0]=111;
	paradas[103]->siguientes[1]=-1;
	paradas[103]->siguientes[2]=-1;

	paradas[104]->siguientes[0]=96;
	paradas[104]->siguientes[1]=-1;
	paradas[104]->siguientes[2]=-1;

	paradas[105]->siguientes[0]=97;
	paradas[105]->siguientes[1]=104;
	paradas[105]->siguientes[2]=-1;

	paradas[106]->siguientes[0]=98;
	paradas[106]->siguientes[1]=105;
	paradas[106]->siguientes[2]=-1;

	paradas[107]->siguientes[0]=99;
	paradas[107]->siguientes[1]=106;
	paradas[107]->siguientes[2]=-1;

	paradas[108]->siguientes[0]=107;
	paradas[108]->siguientes[1]=-1;
	paradas[108]->siguientes[2]=-1;

	paradas[109]->siguientes[0]=100;
	paradas[109]->siguientes[1]=108;
	paradas[109]->siguientes[2]=-1;

	paradas[110]->siguientes[0]=101;
	paradas[110]->siguientes[1]=109;
	paradas[110]->siguientes[2]=-1;

	paradas[111]->siguientes[0]=102;
	paradas[111]->siguientes[1]=110;
	paradas[111]->siguientes[2]=-1;

	//Especiales para los camiones
	paradas[112]->siguientes[0]=58;
	paradas[112]->siguientes[1]=-1;
	paradas[112]->siguientes[2]=-1;

	paradas[113]->siguientes[0]=53;
	paradas[113]->siguientes[1]=-1;
	paradas[113]->siguientes[2]=-1;

}
