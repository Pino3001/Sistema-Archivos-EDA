#ifndef SISTEMA_H
#define SISTEMA_H

// Estructuras de Datos y Algoritmos - Curso 2023
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// Trabajo Obligatorio
// sistema.h
// Modulo de Definición del File System.


#include "definiciones.h"

typedef struct _sistema * Sistema;


TipoRet CREARSISTEMA(Sistema &s);
// Inicializa el sistema para que contenga únicamente al directorio RAIZ, sin subdirectorios ni archivos.
// Para mas detalles ver letra.

TipoRet DESTRUIRSISTEMA(Sistema &s);
// Destruye el sistema, liberando la memoria asignada a las estructuras que datos que constituyen el file system.
// Para mas detalles ver letra.

TipoRet CD (Sistema &s, Cadena nombreDirectorio);
// Cambia directorio. 
// Para mas detalles ver letra.
	
TipoRet MKDIR (Sistema &s, Cadena nombreDirectorio);
// Crea un nuevo directorio. 
// Para mas detalles ver letra.

TipoRet RMDIR (Sistema &s, Cadena nombreDirectorio);
// Elimina un directorio.
// Para mas detalles ver letra.

TipoRet MOVE (Sistema &s, Cadena nombre, Cadena directorioDestino);
// mueve un directorio o archivo desde su directorio origen hacia un nuevo directorio destino.
// Para mas detalles ver letra.

TipoRet DIR (Sistema &s, Cadena parametro);
// Muestra el contenido del directorio actual.
// Para mas detalles ver letra.

TipoRet CREATEFILE (Sistema &s, Cadena nombreArchivo);
// Crea un nuevo archivo en el directorio actual.
// Para mas detalles ver letra.

TipoRet DELETE (Sistema &s, Cadena nombreArchivo);
// Elimina un archivo del directorio actual, siempre y cuando no sea de sólo lectura.
// Para mas detalles ver letra.

TipoRet ATTRIB (Sistema &s, Cadena nombreArchivo, Cadena parametro);
// Agrega un texto al comienzo del archivo NombreArchivo.
// Para mas detalles ver letra.

TipoRet IC (Sistema &s, Cadena nombreArchivo, Cadena texto);
// Agrega un texto al final del archivo NombreArchivo.
// Para mas detalles ver letra.

TipoRet IF (Sistema &s, Cadena nombreArchivo, Cadena texto);
// Agrega un texto al final del archivo NombreArchivo.
// Para mas detalles ver letra.

TipoRet DC (Sistema &s, Cadena nombreArchivo, int k);
// Elimina los a lo sumo K primeros caracteres del archivo parámetro.
// Para mas detalles ver letra.

TipoRet DF (Sistema &s, Cadena nombreArchivo, int k);
// Elimina los a lo sumo K últimos caracteres del archivo parámetro.
// Para mas detalles ver letra.

TipoRet TYPE (Sistema &s, Cadena nombreArchivo);
// Imprime el contenido del archivo parámetro.
// Para mas detalles ver letra.

TipoRet SEARCH (Sistema &s, Cadena nombreArchivo, Cadena texto);
// Busca dentro del archivo la existencia del texto.
// Para mas detalles ver letra.

TipoRet REPLACE (Sistema &s, Cadena nombreArchivo, Cadena texto1, Cadena texto2);
// Busca y reemplaza dentro del archivo la existencia del texto1 por el texto2. 
// Para mas detalles ver letra.

#endif


