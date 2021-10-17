output: main.o background.o player.o laser.o gSettings.o file_io.o fcns.o
	g++ main.o background.o player.o laser.o gSettings.o file_io.o fcns.o -o DL -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp
background.o: background.cpp background.h
	g++ -c background.cpp
player.o: player.cpp player.h
	g++ -c player.cpp
laser.o: laser.cpp laser.h
	g++ -c laser.cpp
gSettings.o: gSettings.cpp gSettings.h
	g++ -c gSettings.cpp
file_io.o: file_io.cpp file_io.h
	g++ -c file_io.cpp
fcns.o: fcns.cpp fcns.h
	g++ -c fcns.cpp

clean:
	rm *.o DL
