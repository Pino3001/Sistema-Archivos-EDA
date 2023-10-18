#ifndef DIRECTORIO_H
#define DIRECTORIO_H

typedef struct nodo_directorio * directorio;

directorio crear_directorio(Cadena nombre);
// Crea un directorio vacio de nombre "nombre".

directorio destruir_directorio(directorio d);
// Destruye el directorio pasado como parametro.


#endif
