Train* train;

void drawing_car(cairo_t * cr, Car* car){
	if(car->run){

				if(car->type == 1){
					cairo_set_source_rgb (cr, 0.890, 0.352, 0.352);
	        cairo_rectangle (cr, car->axisX, car->axisY, 25, 25);
	        cairo_fill (cr);
					cairo_move_to(cr, car->axisX+7, car->axisY+16);
					cairo_set_source_rgb (cr, 1, 1, 1);
					char* buf[3];
					sprintf(buf, "B%d", car->personasActual);

			    cairo_show_text(cr, buf);

				}else if(car->type == 2){
					if(car->dock == 2){
						cairo_set_source_rgb (cr, 0.898,0.584,0.803);
					}else{
						cairo_set_source_rgb (cr, 0.811,0.937,0.454);

					}
					cairo_rectangle (cr, car->axisX, car->axisY, 25, 25);
					cairo_fill (cr);
				}else{
					cairo_set_source_rgb (cr, 0, 0, 0);
	        cairo_rectangle (cr, car->axisX, car->axisY, 25, 25);
	        cairo_fill (cr);
					cairo_move_to(cr, car->axisX+8, car->axisY+16);
					cairo_set_source_rgb (cr, 1, 1, 1);
					cairo_show_text(cr, car->id);
				}

	}
}//Fin drawing_car

void check_semaforo(Car* car){

    if(car->d == 2){
     if(car->axisX == (17*25)+25 && car->axisY == 4*25){
         if(cambiarEstado == 1){
           car->paso = 0;
         }else {
           car->paso = 1;
         }
      }
    } else if(car->d == 1){
      if(car->axisX == (16*25)-25 && car->axisY == 5*25){
         if(cambiarEstado == 0){
           car->paso = 0;
         }else {
           car->paso = 1;
         }
      }
    }

}//check_semaforo

void check_bridge(Car* car){
    switch(car->d){
        case 1:
            if(car->axisX == 15*25 &&  puente_ocupado == 1){
                while(puente_ocupado == 1){
                    car->paso = 0;
                }
                car->paso = 1;
            }else  if(cityMatrix[car->matrizx][(car->matrizy)] == 33 && puente_ocupado == 0){
                disable();
            } else if(cityMatrix[car->matrizx+1][(car->matrizy)] == 33 && puente_ocupado == 1){
                able();
            }
        break;
        case 2:
            if(car->axisX == 18*25 &&  puente_ocupado == 1){
                while(puente_ocupado == 1){
                    car->paso = 0;
                }
                car->paso = 1;
            }else  if(cityMatrix[car->matrizx][(car->matrizy)] == 33 && puente_ocupado == 0){
                disable();
            } else if(cityMatrix[car->matrizx-1][(car->matrizy)] == 33 && puente_ocupado == 1){
                able();
            }
        break;
    }
}//check_bridge

void check_intersection(Car* car){

   if(cityMatrix[car->matrizy][(car->matrizx)] == 9){
        usleep(500000);
    }
}//check_bridge


int check_repair(Car* car){

	int temprepair=0;
    switch(car->d){
        case 1:
            if(car->axisX+25 == repair->axisX && car->axisY == repair->axisY ){
                temprepair=1;
            }
        break;
        case 2:
            if(car->axisX-25 == repair->axisX && car->axisY == repair->axisY ){
                temprepair=1;
            }
        break;
        case 3:
            if(car->axisX == repair->axisX && car->axisY-25 == repair->axisY ){
                temprepair=1;
            }

        break;
        case 4:
            if(car->axisX == repair->axisX && car->axisY+25 == repair->axisY ){
                temprepair=1;
            }

        break;
    }

	return temprepair;
}//Check Repair

int check_colision(Car* car){
    int succes=0;

    for (int i = 0; i < count; ++i){
        if(i!=car->ide){
            switch(car->d){
                case 1:
                    if((car->axisX+25 > carros[i]->axisX && car->axisX < carros[i]->axisX+25 && car->axisX+25 < carros[i]->axisX+25)
                        &&(car->axisY+25 > carros[i]->axisY && car->axisY < carros[i]->axisY+25)){
                        if(carros[i]->run){
                            succes=1;
                        }
                    }
                    break;
                case 2:
                    if((car->axisX+25 > carros[i]->axisX && car->axisX < carros[i]->axisX+25 && car->axisX+25 > carros[i]->axisX+25)
                        &&(car->axisY+25 >carros[i]->axisY && car->axisY < carros[i]->axisY+25)){
                        if(carros[i]->run){
                            succes=1;
                        }
                    }
                    break;
                case 3:
                    if((car->axisY+25 > carros[i]->axisY && car->axisY < carros[i]->axisY+25 && car->axisY+25 > carros[i]->axisY+25)
                        &&(car->axisX+25 >carros[i]->axisX && car->axisX < carros[i]->axisX+25)){
                        if(carros[i]->run){
                            succes=1;
                        }
                    }
                    break;

                case 4:
                    if((car->axisY+25 > carros[i]->axisY && car->axisY < carros[i]->axisY+25 && car->axisY+25 < carros[i]->axisY+25)
                        &&(car->axisX+25 > carros[i]->axisX && car->axisX < carros[i]->axisX+25)){
                        if(carros[i]->run){
                            succes=1;
                        }
                    }
                    break;

            }
        }
    }
		if((car->axisX%25) != 0 && (car->axisY%25)!= 0){
			succes = 0;
		}
    return succes;
}//check colision


void *move_car (void * car) {
	Car* threadCar = car;

    int positionX, positionY;

    int contadorParadas = 0;
    int contadorParadas1 = 0;
		int busC = threadCar->cantidadParadas;
    int prevType;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &prevType);

    for(int i = 0; i<threadCar->cantidadParadas; i++){

				dijkstra(112, threadCar->salida, threadCar->vparadas[i], threadCar);
        contadorParadas = 0;
				threadCar->run = 1;

        while (threadCar->run) {
					  //check_intersection(threadCar);
            check_semaforo(threadCar);
            while(threadCar->paso == 0){
                check_semaforo(threadCar);
            }
            check_bridge(threadCar);
            while(threadCar->paso == 0){
                check_bridge(threadCar);
            }
            if(check_repair(threadCar) != 1 && check_colision(threadCar) != 1){
                usleep(threadCar->speed);

                threadCar->matrizx = threadCar->axisX/25;
                threadCar->matrizy = threadCar->axisY/25;

                if(threadCar->axisX == paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY > paradas[threadCar->direccion[contadorParadas]]->axisY){  //1 derecha, 2 izqui, 3 arriba, 4 abajo

                    threadCar->d = 3;
                    threadCar->axisY--;

                }else
                if(threadCar->axisX < paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY > paradas[threadCar->direccion[contadorParadas]]->axisY){

                    if(threadCar->d == 3){
                        threadCar->axisY--;
                    }else{
                        threadCar->axisX++;
                    }


                }else
                if(threadCar->axisX < paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY == paradas[threadCar->direccion[contadorParadas]]->axisY){

                    threadCar->d = 1;
                    threadCar->axisX++;

                }else
                if(threadCar->axisX < paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY < paradas[threadCar->direccion[contadorParadas]]->axisY){

                    if(threadCar->d == 1){
                       threadCar->axisX++;
                   }else{
                       threadCar->axisY++;
                   }


                }else
                if(threadCar->axisX == paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY < paradas[threadCar->direccion[contadorParadas]]->axisY){

                    threadCar->d = 4;
                    threadCar->axisY++;

                }else
                if(threadCar->axisX > paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY < paradas[threadCar->direccion[contadorParadas]]->axisY){

                    if(threadCar->d == 4){
                        threadCar->axisY++;
                    }else{
                        threadCar->axisX--;
                    }


                }else
                if(threadCar->axisX > paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY == paradas[threadCar->direccion[contadorParadas]]->axisY){

                    threadCar->d = 2;
                    threadCar->axisX--;

                }else
                if(threadCar->axisX > paradas[threadCar->direccion[contadorParadas]]->axisX
                    && threadCar->axisY > paradas[threadCar->direccion[contadorParadas]]->axisY){

                    if(threadCar->d == 3){
                        threadCar->axisY--;
                    }else{
                        threadCar->axisX--;
                    }


                }else
                if(threadCar->axisX/25 == paradas[threadCar->direccion[contadorParadas]]->x
                    && threadCar->axisY/25 == paradas[threadCar->direccion[contadorParadas]]->y){

                    if(paradas[threadCar->direccion[contadorParadas]]->id == threadCar->vparadas[contadorParadas1]){
											if(threadCar->type == 1){//ES bus
												srand(time(NULL));
												sleep(1);
												int persons = 0;
												int bus_stop = rand() % 2;

												if(bus_stop == 0){
													persons = rand() % 25;

													if(threadCar->capacidadPersonas >= (threadCar->personasActual + persons)){
														usleep(persons*100000);
														threadCar->personasActual += persons;
													}
												}else{
													if(threadCar->capacidadPersonas == 0){

													}else {
														do{
															persons = rand() % 25;
														}while (persons>threadCar->personasActual);

														usleep(persons*125000);
														threadCar->personasActual -= persons;

													}

												}

												threadCar->run = 0;


                        threadCar->salida = paradas[threadCar->direccion[contadorParadas]]->id;

											//	contadorParadas1++;

												if(contadorParadas1 == threadCar->cantidadParadas-1){
													contadorParadas1 = 0;
												//	contadorParadas = 0;
													i = 0;
												}
												contadorParadas1++;


											}else if(threadCar->type == 2){//ES CAMION



											}else{///ES Carro
												switch (threadCar->accionparadas[contadorParadas1]) {
                            case 0:
                                sleep(1);
                                break;
                            case 1:
                                sleep(3);
                                break;
                            case 2:
                                sleep(2);
                                break;
                            case 3:
                                sleep(5);
                                break;
                            case 4:
                                sleep(10);
                                break;
                        }

                        threadCar->run = 0;


                        threadCar->salida = paradas[threadCar->direccion[contadorParadas]]->id;

                        contadorParadas1++;


											}

                    }else{

                    }
										contadorParadas++;

                }
            }//if repair

        }//WHILE

    }//for
   // printf("termino\n");

    return NULL;
}//Move car
//////////////////////////////////////////////////////////////////////////train/////////////////////////////////
void a1_dock(int cargo){
	while(stopa1 == 1 || (a1cargo+cargo)>100){
		sleep(1);
	}
	stopa1=1;
	int time = cargo*250000;
	usleep(time);
	a1cargo += cargo;

	stopa1=0;
}

void a1_dock_train(int cargo){
	while(stopa1 == 1 || (a1cargo-cargo)<=-1){
		sleep(1);
	}
	stopa1=1;
	sleep(cargo);
	a1cargo -= cargo;

	stopa1=0;
	//printf("Falta prioridad\n");
}

void a2_dock(int cargo){
	while(stopa2 == 1 || (a2cargo+cargo)>100){
		sleep(1);
	}
	stopa2=1;
	int time= cargo*250000;
	usleep(time);
	a2cargo += cargo;

	stopa2=0;
}
void a2_dock_train(int cargo){
	while(stopa2 == 1 || (a2cargo-cargo)<=-1){
		sleep(1);
	}
	stopa2=1;
	sleep(cargo);
	a2cargo-=cargo;

	stopa2=0;
}

void draw_train(cairo_t *cr){
	  char* tbuf[3];
		cairo_set_source_rgb (cr, 0.93, 0.42,0 );
    cairo_rectangle (cr,train->axisX ,train->axisY, 25, 25);
    cairo_fill (cr);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_move_to (cr,train->axisX+10,train->axisY+(25/2));

    sprintf(tbuf, "%d", train->cantidadVagones);

    cairo_show_text(cr, tbuf);
}//draw_train

void nort_bridge_behavior(){

    while(!interrupt){
        sleep(1);
    }
    interrupt=false;
}

void switchflag(){
    interrupt=true;
}

void south_bridge(){

    while(block==1){
        // espera que el candado sea puesto en 0
    }
    block=1;
}

void switchblock(){
    block=0;
}

void lateral_bridge1(){
	while(lock == 1){

	}
	lock = 1;
}

void block_lateral_bridge1(){
	lock = 0;
}

void train_stops(){
    if(train->matrizx==9 && train->matrizy==14){
        if(train->cargo==0){
            a1_dock_train(train->cantidadVagones);
            train->cargo=train->cantidadVagones;
        }else{
            a1_dock(train->cantidadVagones);
            train->cargo=0;
        }
    }else if(train->matrizx==24 && train->matrizy==13){
        if(train->cargo==0){
            a2_dock_train(train->cantidadVagones);
            train->cargo=train->cantidadVagones;
        }else{
            a2_dock_train(train->cantidadVagones);
            train->cargo=0;
        }
    }else if(train->matrizx==15 && train->matrizy==9){
        nort_bridge_behavior();
    }else if(train->matrizx==18 && train->matrizy==9){
        switchflag();
    }else if(train->matrizx==18 && train->matrizy==18){
        south_bridge();
    }else if(train->matrizx==15 && train->matrizy==18){
        switchblock();
    } else if(train->matrizx==9 && train->matrizy==15){
		//	sleep(1);
    } else if(train->matrizx==9 && train->matrizy==15){

    }
}

void move_train(){
    train->run=1;
    int d=0;

    while(train->run){
        if(train->axisX%25==0 && train->axisY%25==0){
            if(train->matrizx == 9 && train->matrizy ==10){
                d=1;
            }else if(train->matrizx == 13 && train->matrizy ==10){
                d=4;
            }else  if(train->matrizx == 13 && train->matrizy == 9){
                d=1;
            }else  if(train->matrizx == 20 && train->matrizy == 9){
                d=2;
            } else if(train->matrizx == 20 && train->matrizy == 10){
                d=1;
            } else if(train->matrizx == 24 && train->matrizy == 10){
                d=2;
            }else if(train->matrizx == 24 && train->matrizy == 17){
                d=3;
            }else if(train->matrizx == 20 && train->matrizy == 17){
                d=2;
            }else if(train->matrizx == 20 && train->matrizy == 18){
                d=3;
            }else if(train->matrizx == 13 && train->matrizy == 18){
                d=4;
            }else if(train->matrizx == 13 && train->matrizy == 17){
                d=3;
            }else if(train->matrizx == 9 && train->matrizy == 17){
                d=4;
            }
        }

        switch(d){
            case 1:
                train->axisX++;
                break;
            case 2:
                train->axisY++;
                break;
            case 3:
                train->axisX--;
                break;
            case 4:
                train->axisY--;
                break;
        }

        train->matrizx = train->axisX/25;
        train->matrizy = train->axisY/25;
        if(((train->axisX%25)==0 && (train->axisY%25)==0)){
            train_stops();
        }
        usleep(20000);
    }
}//MOVE TRAIN

void create_train(){
    train= malloc(sizeof(Train));
    train->matrizy = 10;
    train->matrizx= 9;
    train->axisX = 9*25;
    train->axisY = 10*25;
    train->cantidadVagones=2;
    train->cargo=0;
    pthread_create(&tTrain, NULL,(void*) move_train, NULL);
}


///////////////////////////////////////////////////BARCO////////////////////////////////////////

void disembark_cargo(Ship* ship){
	if(ship->matrizy==13){
		if(ship->dock==1 && ship->cargo!=-1){
			sleep(2);
			a1_dock(ship->cargo);
			ship->cargo=-1;
		}else if(ship->dock==2 && ship->cargo!=-1){
			sleep(2);
			a2_dock(ship->cargo);
			ship->cargo=-1;
		}
	}else if(ship->matrizy==7){
		nort_bridge_behavior();
	}else if(ship->matrizy==10){
		switchflag();
	}else if(ship->matrizy==16){
		south_bridge();
	}else if(ship->matrizy==19){
		switchblock();
	}
	if(ship->matrizy==3 || ship->matrizy==4 || ship->matrizy==2 ){
		ship->desapair = 1;
	}else{
		ship->desapair = 0;
	}
}//disembark_cargo

int check_colision_ship(int id){
	int found=0;

	for (int i = 0; i < ships_number; ++i){
		if(i!=id && ships[i]->run){
			if((ships[id]->axisY+25 >= ships[i]->axisY && ships[id]->axisY <=ships[i]->axisY+25 && ships[id]->axisY+25 < ships[i]->axisY+25)
				&&(ships[id]->axisX+25 >=ships[i]->axisX && ships[id]->axisX <= ships[i]->axisX+25)){
				found=1;
			}
		}
	}
	return found;
}

void move_ship(Ship* ship){
	while(ship->run){
		if(check_colision_ship(ship->id)==0){
			ship->axisY++;
			if(ship->matrizy >= 10 && ship->matrizy <= 12 && ship->dock==1){
				ship->axisX--;
			}else if(ship->matrizy >= 10 && ship->matrizy <= 12 && ship->dock==2){
				ship->axisX++;
			}else if (ship->matrizy >=14 && ship->matrizy <= 16 && ship->dock==1){
				ship->axisX++;
			}else if (ship->matrizy >=14 && ship->matrizy <= 16 && ship->dock==2){
				ship->axisX--;
			}else if(ship->matrizy==26){
				ship->run=false;
			}
			if((ship->axisX%25)==0 && (ship->axisY%25)==0){
				disembark_cargo(ship);
				ship->matrizy=ship->axisY/25;
				ship->matrizx=ship->axisX/25;
			}
		}
		usleep(25000);
	}
}//move_ship

void create_ship(int dock, int cargo){

	ships[ships_number] = malloc(sizeof(Ship));
	ships[ships_number]->id = ships_number;
	ships[ships_number]->dock = dock;
	ships[ships_number]->cargo = cargo;
	ships[ships_number]->matrizy = 1;
	ships[ships_number]->axisY = 25;
	ships[ships_number]->desapair = 0;
	if(dock==1){
		ships[ships_number]->axisX = 16*25;
		ships[ships_number]->matrizx=16;
	}else{
		ships[ships_number]->axisX= 17*25;
		ships[ships_number]->matrizx=17;
	}

	ships[ships_number]->run=1;
	pthread_create(&ship_thread[ships_number], NULL , (void*)move_ship ,(void*)ships[ships_number]);
	ships_number++;
}
//////////////////////////////////////////////////////////CAMION//////////////////////////////////////
void move_truck(void * car){
		Car* truck = car;
    truck->run=1;
		if(truck->dock == 1){
			while(truck->run){
				check_semaforo(truck);
				while(truck->paso == 0){
						check_semaforo(truck);
				}
				check_bridge(truck);
				while(truck->paso == 0){
						check_bridge(truck);
				}
				if(check_repair(truck) != 1 && check_colision(truck) != 1){
					if(truck->axisX%25==0 && truck->axisY%25==0){
	            if(truck->matrizx == 2 && truck->matrizy ==4){
	               truck->d=2;
	            }else if(truck->matrizx == 0 && truck->matrizy ==4){
	                truck->d=3;
	            }else  if(truck->matrizx == 0 && truck->matrizy == 0){
	                truck->d=1;
	            }else  if(truck->matrizx == 29 && truck->matrizy == 0){
	                truck->d=4;
	            } else if(truck->matrizx == 20 && truck->matrizy == 10){
	                truck->d=1;
	            } else if(truck->matrizx == 29 && truck->matrizy == 13){
	                truck->d=2;
	            }else if(truck->matrizx == 22 && truck->matrizy == 13){
								while((a2cargo - truck->cargo)<=0){

								}
								a2cargo -= truck->cargo;
								sleep(1);
	               truck->d=4;
	            }else if(truck->matrizx == 22 && truck->matrizy == 14){
	                truck->d=1;
	            }else if(truck->matrizx == 33 && truck->matrizy == 14){
	                truck->d=4;
	            }else if(truck->matrizx == 33 && truck->matrizy == 27){
	                truck->d=2;
	            }else if(truck->matrizx == 0 && truck->matrizy == 27){
	                truck->d=3;
	            }else if(truck->matrizx == 0 && truck->matrizy == 2){
									sleep(1);
	                truck->d=3;
	            } if(truck->matrizx == 25&& truck->matrizy == 13){
									sleep(1);
	            }
	        }

	        switch(truck->d){
	            case 1:
	                truck->axisX++;
	                break;
	            case 4:
	                truck->axisY++;
	                break;
	            case 2:
	                truck->axisX--;
	                break;
	            case 3:
	                truck->axisY--;
	                break;
	        }

					truck->matrizx = truck->axisX/25;
	        truck->matrizy = truck->axisY/25;
	        usleep(truck->speed);
				}

	    }
		}else{
			while(truck->run){
				check_semaforo(truck);
				while(truck->paso == 0){
						check_semaforo(truck);
				}
				check_bridge(truck);
				while(truck->paso == 0){
						check_bridge(truck);
				}
				if(check_repair(truck) != 1 && check_colision(truck) != 1){
					if(truck->axisX%25==0 && truck->axisY%25==0){
	            if(truck->matrizx == 31 && truck->matrizy ==4){
								sleep(1);
	                truck->d=2;
	            }else if(truck->matrizx == 4 && truck->matrizy ==4){
	                truck->d=4;
	            }else  if(truck->matrizx == 4 && truck->matrizy == 14){
	                truck->d=1;
	            }else  if(truck->matrizx == 11 && truck->matrizy == 14){
								while((a1cargo - truck->cargo)<=0){

								}
								a1cargo -= truck->cargo;
								sleep(1);
	                truck->d=3;
	            } else if(truck->matrizx == 11 && truck->matrizy == 13){
	                truck->d=2;
	            } else if(truck->matrizx == 0 && truck->matrizy == 13){
	                truck->d=3;
	            }else if(truck->matrizx == 0 && truck->matrizy == 0){
	                truck->d=1;
	            }else if(truck->matrizx == 33 && truck->matrizy == 0){
	                truck->d=4;
	            }else if(truck->matrizx == 33 && truck->matrizy == 4){
	                truck->d=2;
	            }else if(truck->matrizx == 8 && truck->matrizy == 14){
								sleep(1);
	            }else if(truck->matrizx == 10 && truck->matrizy == 14){
							//	lateral_bridge1();

							}

	        }

	        switch(truck->d){
	            case 1:
	                truck->axisX++;
	                break;
	            case 4:
	                truck->axisY++;
	                break;
	            case 2:
	                truck->axisX--;
	                break;
	            case 3:
	                truck->axisY--;
	                break;
	        }

	        truck->matrizx = truck->axisX/25;
	        truck->matrizy = truck->axisY/25;
	        usleep(truck->speed);

				}
		}
	}

}//MOVE Truck

void delete_car(int x, int y){
  int mx = x/25;
	int my = y/25;
	//printf("%d , %d\n", mx, my);
	if(count>0){
		for(int i = 0; i<count; i++){
			if(carros[i]->matrizx == mx && carros[i]->matrizy == my){
				 carros[i]->cantidadParadas = 0;
				 carros[i]->run = 0;
				 break;
			}
		}
	}
}
