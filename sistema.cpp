// Estructuras de Datos y Algoritmos - Curso 2023
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// Trabajo Obligatorio
// sistema.c
// Modulo de Implementacion del File System.

#include "sistema.h"
#include "directorio.h"
#include "definiciones.h"
#include <string.h>

#include <iostream>

using namespace std;

struct _sistema
{
	directorio raiz;
	directorio actual;
	// aquí deben figurar los campos que usted considere necesarios para manipular el sistema de directorios
	// Se deberan crear nuevos modulos
};

TipoRet CREARSISTEMA(Sistema &s)
{
	// Inicializa el sistema para que contenga únicamente al directorio RAIZ, sin subdirectorios ni archivos.
	// Para mas detalles ver letra.
	s = new (_sistema);
	Cadena nombre = new char[MAX_NOM_DIR];
	strcpy(nombre, "RAIZ");
	s->raiz = crear_directorio(nombre, NULL);
	delete[] nombre;
	s->actual = s->raiz;
	return OK;
}

TipoRet DESTRUIRSISTEMA(Sistema &s)
{
	// Destruye el sistema, liberando la memoria asignada a las estructuras que datos que constituyen el file system.
	// Para mas detalles ver letra.

	s->raiz = destruir_directorio(s->raiz);
	delete s;
	return OK;
}

TipoRet CD(Sistema &s, Cadena nombreDirectorio)
{
	// Cambia directorio.
	// Para mas detalles ver letra.

	if (strcmp(nombreDirectorio, "..") == 0)
	{ // Me muevo hacia atras
		if (s->actual == s->raiz)
		{ // Evito moverme hacia atras si estoy en directorio raiz
			cout << "Te encuentras en el directorio RAIZ.";
			return ERROR;
		}
		else
		{
			s->actual = ir_al_padre(s->actual);
			return OK;
		}
	}
	else if (strcmp(nombreDirectorio, "RAIZ") == 0)
	{ // Vuelvo al directorio raiz
		s->actual = s->raiz;
		return OK;
	}
	else
	{ // Busco nombre de carpeta para moverme
		if (!existe_directorio(nombreDirectorio, s->actual))
		{
			cout << "No existe el directorio dentro del directorio actual./n";
			return ERROR;
		}
		else
		{
			s->actual = obtener_subdirectorio(nombreDirectorio, s->actual);
			return OK;
		}
	}
}

TipoRet MKDIR(Sistema &s, Cadena nombreDirectorio)
{
	if (existe_directorio(nombreDirectorio, s->actual))
	{ // Compruebo que no exista el nombre de directorio
		cout << "Ese nombre de directorio esta usado";
		return ERROR;
	}
	else
	{
		// Creo el nuevo directorio sin moverme de mi posicion.
		directorio dir;
		dir = crear_directorio(nombreDirectorio, s->actual);

		return OK;
	}
}

TipoRet RMDIR(Sistema &s, Cadena nombreDirectorio)
{
	// Elimina un directorio.
	// Para mas detalles ver letra.
	if (remover_directorio(nombreDirectorio, s->actual))
	{
		return OK;
	}
	return ERROR;
}

TipoRet MOVE(Sistema &s, Cadena nombre, Cadena directorioDestino)
{
	// mueve un directorio o archivo desde su directorio origen hacia un nuevo directorio destino.
	// Para mas detalles ver letra.
	if (mover_elemento(nombre, directorioDestino, s->actual, s->raiz))
	{
		return OK;
	}
	return ERROR;
}

TipoRet DIR(Sistema &s, Cadena parametro)
{
	// Muestra el contenido del directorio actual.
	// Para mas detalles ver letra.

	if (parametro == NULL)
	{ // Imprime el arbol de directorios de forma ordenada.
		imprimir_directorio_v2(s->actual);
		return OK;
	}
	else if (strcmp(parametro, "/S") == 0)
	{ // Imprime el directorio mostrando permisos de archivos.
		directorio dir = s->actual;
		imprimir_directorio(dir);
		return OK;
	}
	else
	{
		return ERROR;
	}
}

TipoRet CREATEFILE(Sistema &s, Cadena nombreArchivo)
{
	// Crea un nuevo archivo en el directorio actual.
	// Para mas detalles ver letra.

	if (existe_archivo(nombreArchivo, s->actual) != NULL)
	{ // Compruebo que no exista el nombre de archivo.
		cout << "Ese nombre de directorio esta usado";
		return ERROR;
	}
	else
	{ // Separo la extencion y nombre del parametro.
		Cadena nombre = strtok(nombreArchivo, ".");
		Cadena ext = strtok(NULL, ".");
		if (nombre == NULL || ext == NULL)
		{
			cout << "No se ingreso una extencion. \n";
			return ERROR;
		}
		crear_archivo(nombre, ext, s->actual);
		return OK;
	}
}

TipoRet DELETE(Sistema &s, Cadena nombreArchivo)
{
	// Elimina un archivo del directorio actual, siempre y cuando no sea de sólo lectura.
	// Para mas detalles ver letra.
	// Separo la extencion y el nombre del paramentro.
	Cadena nombre = strtok(nombreArchivo, ".");
	Cadena ext = strtok(NULL, ".");
	if (nombre == NULL || ext == NULL)
	{
		cout << "No se ingreso una extencion. \n";
		return ERROR;
	}
	if (!destruir_archivo_directorio(nombre, ext, s->actual))
	{
		return ERROR;
	}
	return OK;
}

TipoRet ATTRIB(Sistema &s, Cadena nombreArchivo, Cadena parametro)
{
	// Agrega un texto al comienzo del archivo NombreArchivo.
	// Para mas detalles ver letra.

	Atributo atr = Lectura;
	// Compruebo y asigno a la variable de tipo Atributo el valor que debo colocar en el archivo.
	if (strcasecmp(parametro, "+W") == 0)
	{
		atr = Lectura_Escritura;
	}
	else if (strcasecmp(parametro, "-W") == 0)
	{
		atr = Lectura;
	}
	else
	{
		cout << "Parametro incorrecto. \n";
		return ERROR;
	}
	// Separo la extencion y el nombre del paramentro.
	Cadena nombre = strtok(nombreArchivo, ".");
	Cadena ext = strtok(NULL, ".");
	if (nombre == NULL || ext == NULL)
	{
		cout << "No se ingreso una extencion. \n";
		return ERROR;
	}
	else if (cambiar_atributo(nombre, ext, atr, s->actual))
	{
		return OK;
	}
	else
	{
		return OK;
	}
}

TipoRet IC(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Agrega un texto al final del archivo NombreArchivo.
	// Para mas detalles ver letra.
	if (insertar_texto_archivo(nombreArchivo, texto, s->actual))
	{
		return OK;
	}
	else
	{
		return ERROR;
	}
}

TipoRet IF(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Agrega un texto al final del archivo NombreArchivo.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}

TipoRet DC(Sistema &s, Cadena nombreArchivo, int k)
{
	// Elimina los a lo sumo K primeros caracteres del archivo parámetro.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}

TipoRet DF(Sistema &s, Cadena nombreArchivo, int k)
{
	// Elimina los a lo sumo K últimos caracteres del archivo parámetro.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}

TipoRet TYPE(Sistema &s, Cadena nombreArchivo)
{
	// Imprime el contenido del archivo parámetro.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}

TipoRet SEARCH(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Busca dentro del archivo la existencia del texto.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}

TipoRet REPLACE(Sistema &s, Cadena nombreArchivo, Cadena texto1, Cadena texto2)
{
	// Busca y reemplaza dentro del archivo la existencia del texto1 por el texto2.
	// Para mas detalles ver letra.
	return NO_IMPLEMENTADA;
}
