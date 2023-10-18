
#include "definiciones.h"
#include "string.h"
#include "directorio.h"

struct nodo_directorio{
	Cadena nombre;
	// archivos archs;
	// Ver el arbol de directorios
};


directorio crear_directorio(Cadena nombre){
// Crea un directorio vacio de nombre "nombre".
	directorio d = new(nodo_directorio);
	d->nombre = new char[MAX_NOM_DIR];
	strcpy(d->nombre, nombre);
	// crear lista de archivos vacia
	return d;
}

directorio destruir_directorio(directorio d){
// Destruye el directorio pasado como parametro.
	// Destruir estructura de archivos
	delete [] d->nombre;
	delete d;
	return d;
}








