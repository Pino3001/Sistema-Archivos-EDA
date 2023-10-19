todo: main.o sistema.o directorio.o
	g++ -o main main.o sistema.o directorio.o
main.o: main.cpp
	g++ -c main.cpp
sistema.o: sistema.h sistema.cpp
	g++ -c sistema.cpp
directorio.o: directorio.h directorio.cpp
	g++ -c directorio.cpp
limpiar:
	rm *.o
	rm main

