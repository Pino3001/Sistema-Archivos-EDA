todo: main.o sistema.o directorio.o
	g++ -o main main.o sistema.o directorio.o
main.o: main.c
	g++ -c main.c
sistema.o: sistema.h sistema.c
	g++ -c sistema.c
directorio.o: directorio.h directorio.c
	g++ -c directorio.c
limpiar:
	rm *.o
	rm main

