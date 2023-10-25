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
		directorio dir = existe_directorio(nombreDirectorio, s->actual);
		if (dir == NULL)
		{
			cout << "No existe el directorio dentro del directorio actual./n";
			return ERROR;
		}
		else
		{
			s->actual = dir;
			return OK;
		}
	}
}

TipoRet MKDIR(Sistema &s, Cadena nombreDirectorio)
{
	if (existe_directorio(nombreDirectorio, s->actual) != NULL)
	{ // Compruebo que no exista el nombre de directorio
		cout << "Ese nombre de directorio esta usado";
		return ERROR;
	}
	else
	{
		// Creo el nuevo directorio sin moverme de mi posicion.
		crear_directorio(nombreDirectorio, s->actual);
		cout << "\n";
		DIR(s, "/S");
		return OK;
	}
}

TipoRet RMDIR(Sistema &s, Cadena nombreDirectorio)
{
	// Elimina un directorio.
	// Para mas detalles ver letra.
	directorio dir = quitar_subdirectorio(nombreDirectorio, s->actual);
	if (dir != NULL)
	{
		destruir_directorio(dir);
		return OK;
	}
	else
	{
		cout << "No existe el directorio.";
		return ERROR;
	}
}

TipoRet MOVE(Sistema &s, Cadena nombre, Cadena directorioDestino)
{
	// mueve un directorio o archivo desde su directorio origen hacia un nuevo directorio destino.
	// Para mas detalles ver letra.
	if (mover_elemento(nombre, directorioDestino, s->actual, s->raiz))
	{
		cout << "\n";
		DIR(s, "/S");
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
		cout << "Ya existe un archivo con ese nombre dentro del directorio actual\n";
		return ERROR;
	}
	else
	{ // Separo la extencion y nombre del parametro.

		crear_archivo(nombreArchivo, s->actual);
		return OK;
	}
}

TipoRet DELETE(Sistema &s, Cadena nombreArchivo)
{
	// Elimina un archivo del directorio actual, siempre y cuando no sea de sólo lectura.
	// Para mas detalles ver letra.
	// Separo la extencion y el nombre del paramentro.

	file archivo = quitar_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		destruir_archivo(archivo);
		return OK;
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio, compruebe que haya ingresado el nombre y la extencion correctamente.\n";
		return ERROR;
	}
}

TipoRet ATTRIB(Sistema &s, Cadena nombreArchivo, Cadena parametro)
{
	// Agrega un texto al comienzo del archivo NombreArchivo.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (cambiar_atributo(archivo, parametro))
		{
			return OK;
		}
		else
		{
			cout << "No se ingreso un parametro correcto. \n";
			return ERROR;
		}
	}
	else
	{
		return ERROR;
	}
}

TipoRet IC(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Agrega un texto al final del archivo NombreArchivo.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (insertar_texto_inicio(archivo, texto))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe el archivo " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet IF(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Agrega un texto al final del archivo NombreArchivo.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (insertar_texto_final(archivo, nombreArchivo))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet DC(Sistema &s, Cadena nombreArchivo, int k)
{
	// Elimina los a lo sumo K primeros caracteres del archivo parámetro.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (eliminar_K_elementos_iniciales(archivo, k))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet DF(Sistema &s, Cadena nombreArchivo, int k)
{
	// Elimina los a lo sumo K últimos caracteres del archivo parámetro.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (eliminar_K_elementos_finales(archivo, k))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet TYPE(Sistema &s, Cadena nombreArchivo)
{
	// Imprime el contenido del archivo parámetro.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (imprimir_texto(archivo))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet SEARCH(Sistema &s, Cadena nombreArchivo, Cadena texto)
{
	// Busca dentro del archivo la existencia del texto.
	// Para mas detalles ver letra.

	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (search_texto(archivo, texto))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}

TipoRet REPLACE(Sistema &s, Cadena nombreArchivo, Cadena texto1, Cadena texto2)
{
	// Busca y reemplaza dentro del archivo la existencia del texto1 por el texto2.
	// Para mas detalles ver letra.
	file archivo = existe_archivo(nombreArchivo, s->actual);
	if (archivo != NULL)
	{
		if (remplazar_texto(archivo, texto1, texto2))
		{
			return OK;
		}
		else
		{
			return ERROR;
		}
	}
	else
	{
		cout << "No existe " << nombreArchivo << " en el directorio.";
		return ERROR;
	}
}
