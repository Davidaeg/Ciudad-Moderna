CC= gcc
SOURCES=main.c 

EXECUTABLE=ejecutable
DIR=Ejecutable

all:  $(EXECUTABLE)

$(EXECUTABLE):  $(DIR)
	$(CC) $(SOURCES) -o $(DIR)/$(EXECUTABLE) `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -pthread

$(DIR):
	mkdir Ejecutable/
