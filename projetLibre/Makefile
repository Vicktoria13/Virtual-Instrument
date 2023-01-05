CC = g++
EXEC_NAME = main
SRC = main.cpp

LIBS = `pkg-config --cflags --libs opencv4 `

main: main.o play.o
	$(CC) -o main main.o play.o -lSDL2main -lSDL2 -lSDL2_mixer $(LIBS)

main.o: main.cpp play.h
	$(CC) -c main.cpp -lSDL2main -lSDL2 -lSDL2_mixer  $(LIBS)
 
play.o: play.cpp play.h
	g++ -c play.cpp -lSDL2main -lSDL2 -lSDL2_mixer  $(LIBS)
	
 


 
