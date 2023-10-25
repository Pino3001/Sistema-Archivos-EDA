#include "definiciones.h"
#include "string.h"
#include "directorio.h"
#include <string.h>
#include <iostream>

using namespace std;

struct nodo_directorio
{
	Cadena nombrecarpeta;
	directorio padre;
	lista_file archivos;
	lista_dir subdirectorios;
};

// Lista de directorios hijos
struct nodo_lista_dir
{
	directorio dir;
	lista_dir sig;
};

// Lista de archivos
struct nodo_lista_file
{
	file archi;
	lista_file sig;
};

struct nodo_file
{
	Cadena nombreArchivo;
	Cadena extencion;
	Cadena texto;
	Atributo atr;
};

/* -- CREAR NUEVOS DIRECTORIOS Y QUITARLOS -- */

// Crea un directorio nuevo con el nombre dado por parametro;
directorio crear_directorio(Cadena nombre, directorio dirP)
{
	// Crea un directorio vacio de nombre "nombre".
	directorio d = new (nodo_directorio);
	d->nombrecarpeta = new char[MAX_NOM_DIR];
	strcpy(d->nombrecarpeta, nombre);
	d->subdirectorios = NULL;
	d->archivos = NULL;
	d->padre = NULL;
	if (dirP != NULL)
	{
		// dentro setea el padre
		agregar_subdirectorio(dirP, d, false);
	}

	return d; // Retorno nuevo directorio.
}

// Agrega de forma ordenada el sub-directorio en la lista destino. Si sobreescribir es true, remplaza el directorio con el mismo nombre en caso de existir.
directorio agregar_subdirectorio(directorio dDestino, directorio subDir, bool sobreescribir)
{
	if (dDestino->subdirectorios == NULL)
	{
		dDestino->subdirectorios = new (nodo_lista_dir);
		dDestino->subdirectorios->dir = subDir;
		subDir->padre = dDestino;
		dDestino->subdirectorios->sig = NULL;
		return dDestino;
	}
	lista_dir ld = dDestino->subdirectorios;
	if (strcmp(subDir->nombrecarpeta, ld->dir->nombrecarpeta) < 0)
	{
		cout << "marca 1\n";
		lista_dir nLd = new (nodo_lista_dir);
		nLd->dir = subDir;
		nLd->sig = dDestino->subdirectorios;
		dDestino->subdirectorios = nLd;
		subDir->padre = dDestino;
		return dDestino;
	}
	if (strcmp(subDir->nombrecarpeta, ld->dir->nombrecarpeta) == 0)
	{
		cout << "marca 2\n";
		if (sobreescribir)
		{
			directorio tem = destruir_directorio(ld->dir);
			ld->dir = subDir;
			ld->sig = dDestino->subdirectorios;
			dDestino->subdirectorios = ld;
			subDir->padre = dDestino;
			return dDestino;
		}
		else
		{
			cout << "subDir->nombrecarpeta " << subDir->nombrecarpeta << "\n";
			cout << "ld->dir->nombrecarpeta " << ld->dir->nombrecarpeta << "\n";
			return NULL;
		}
	}
	while (ld->sig != NULL && strcmp(subDir->nombrecarpeta, ld->sig->dir->nombrecarpeta) > 0)
	{
		ld = ld->sig;
	}

	if (ld->sig == NULL)
	{
		cout << "marca 3\n";
		if (dDestino->subdirectorios != NULL)
		{
			cout << dDestino->subdirectorios->dir->nombrecarpeta << "\n";
		}

		lista_dir nLd = new (nodo_lista_dir);
		nLd->dir = subDir;
		nLd->sig = NULL;
		ld->sig = nLd;
		subDir->padre = dDestino;
		return dDestino;
	}
	if (strcmp(subDir->nombrecarpeta, ld->sig->dir->nombrecarpeta) == 0)
	{
		cout << "marca 4\n";
		if (sobreescribir)
		{
			lista_dir lAux = ld->sig;
			directorio tem = destruir_directorio(ld->sig->dir);
			lAux->dir = subDir;
			lAux->sig = ld->sig->sig;
			ld->sig = lAux;
			subDir->padre = dDestino;
			return dDestino;
		}

		else
		{
			cout << "subDir->nombrecarpeta " << subDir->nombrecarpeta << "\n";
			cout << "ld->sig->dir->nombrecarpeta " << ld->sig->dir->nombrecarpeta << "\n";
			return NULL;
		}
	}
	else
	{
		cout << "marca 5 \n";
		cout << "subDir->nombrecarpeta " << subDir->nombrecarpeta << "\n";
		cout << "ld->sig->dir->nombrecarpeta " << ld->sig->dir->nombrecarpeta << "\n";
		cout << "strcmp(subDir->nombrecarpeta, ld->sig->dir->nombrecarpeta) " << strcmp(subDir->nombrecarpeta, ld->sig->dir->nombrecarpeta) << "\n";
		lista_dir nLd = new (nodo_lista_dir);
		nLd->dir = subDir;
		nLd->sig = ld->sig;
		ld->sig = nLd;
		subDir->padre = dDestino;
		return dDestino;
	}
}

// Destruye el directorio pasado como parametro.
directorio destruir_directorio(directorio dir)
{
	if (dir != NULL)
	{
		// Destruir estructura de archivos.
		dir->archivos = destruir_lista_archivo(dir->archivos);
		// Destruir estructura de directorios.
		dir->subdirectorios = destruir_lista_directorio(dir->subdirectorios);
		delete[] dir->nombrecarpeta;
		// delete dir->padre;
		delete dir;
	}
	return dir;
}

// Destruyo lista de directorios recurcivamente.
lista_dir destruir_lista_directorio(lista_dir d)
{
	if (d != NULL)
	{
		// Destruyo la lista de directorios de donde estoy parado.
		d->sig = destruir_lista_directorio(d->sig);
		// Entro en la estructura de directorios para destruir el interior.
		d->dir = destruir_directorio(d->dir);
		delete d;
	}
	return d;
}

/* -- CREAR NUEVOS ARCHIVOS Y QUITARLOS -- */

// Crea un archivo vacio nuevo. Precondicion: nombre y ext no nulos.
void crear_archivo(Cadena nombreArchivo, directorio dirActual)
{

	Cadena nombre = strtok(nombreArchivo, ".");
	Cadena ext = strtok(NULL, ".");
	file f = new (nodo_file);
	f->nombreArchivo = new char(MAX_NOM_ARCH);
	strcpy(f->nombreArchivo, nombre);
	f->atr = Lectura_Escritura;
	f->extencion = new char(MAX_EXT_ARCH);
	if (ext == NULL)
	{
		strcpy(f->extencion, "txt");
	}
	else
	{
		strcpy(f->extencion, ext);
	}
	f->texto = NULL;
	// Engancho el nuevo archivo en la lista de archivos del directorio.
	agregar_archivo(dirActual, f, false);
}

// Agrega de forma ordenada el archivo en la lista destino. Si sobreescribir es true, remplaza el archivo.
file agregar_archivo(directorio dDestino, file archivo, bool sobreescribir)
{
	if (dDestino->subdirectorios == NULL)
	{
		dDestino->archivos = new (nodo_lista_f);
		dDestino->archivos->archi = archivo;
		dDestino->archivos->sig = NULL;
		return archivo;
	}
	lista_file lf = dDestino->archivos;
	if (strcmp(archivo->nombreArchivo, lf->archi->nombreArchivo) < 0)
	{
		lista_file nLf = new (nodo_lista_file);
		nLf->archi = archivo;
		nLf->sig = dDestino->archivos;
		dDestino->archivos = nLf;
		return archivo;
	}
	if (strcmp(archivo->nombreArchivo, lf->archi->nombreArchivo) == 0)
	{
		if (sobreescribir)
		{
			if (archivo->atr == Lectura_Escritura)
			{
				file tem = destruir_archivo(lf->archi);
				lf->archi = archivo;
				lf->sig = dDestino->archivos;
				dDestino->archivos = lf;
				return archivo;
			}
			else
			{
				cout << "El archivo a sobreescribir es de solo lectura";
				return NULL;
			}
		}
		else
			return NULL;
	}
	while (lf->sig != NULL && strcmp(archivo->nombreArchivo, lf->sig->archi->nombreArchivo) < 0)
	{
		lf = lf->sig;
	}

	if (lf->sig == NULL)
	{
		lista_file nLf = new (nodo_lista_file);
		nLf->archi = archivo;
		nLf->sig = NULL;
		lf->sig = nLf;
		return archivo;
	}
	if (strcmp(archivo->nombreArchivo, lf->sig->archi->nombreArchivo) == 0)
	{
		if (sobreescribir)
		{
			if (archivo->atr == Lectura_Escritura)
			{
				lista_file lAux = lf->sig;
				file tem = destruir_archivo(lf->sig->archi);
				lAux->archi = archivo;
				lAux->sig = lf->sig->sig;
				lf->sig = lAux;
				return archivo;
			}
			else
			{
				cout << "El archivo a sobreescribir es de solo lectura";
				return NULL;
			}
		}
		else
			return NULL;
	}
	else
	{
		lista_file nLf = new (nodo_lista_file);
		nLf->archi = archivo;
		nLf->sig = lf->sig;
		lf->sig = nLf;
		return archivo;
	}
}

// Destruye lista de archivos recursivamente.
lista_file destruir_lista_archivo(lista_file lf)
{
	if (lf != NULL)
	{
		lf->sig = destruir_lista_archivo(lf->sig);
		lf->archi = destruir_archivo(lf->archi);
		delete lf;
	}
	return lf;
}

// Destruir nodo_file.
file destruir_archivo(file a)
{
	delete[] a->nombreArchivo;
	delete[] a->extencion;
	delete[] a->texto;
	// delete a->atr;
	delete a;
	return a;
}

// Destruye el archivo dado.
bool buscar_destruir_archivo_(Cadena nombre, Cadena ext, directorio dir)
{
	// Busca y destruye el archivo dado, si este existe.
	lista_file lf = dir->archivos;
	lista_file aux;
	while ((lf != NULL) && ((strcmp(lf->archi->nombreArchivo, nombre) != 0) || (strcmp(lf->archi->extencion, ext) != 0)))
	{
		aux = lf;
		lf = lf->sig;
	}
	if (lf == NULL)
	{
		cout << "NO existe el archivo.\n";
		return false;
	}
	else if (lf->archi->atr == Lectura)
	{
		cout << "No es posible eliminar el archivo " << lf->archi->nombreArchivo << ", ya que es de solo lectura. \n";
		return false;
	}
	else if (dir->archivos == lf)
	{
		dir->archivos = lf->sig;
		lf->archi = destruir_archivo(lf->archi);
		delete lf;
		return true;
	}
	else
	{
		aux->sig = lf->sig;
		lf->archi = destruir_archivo(lf->archi);
		delete lf;
		return true;
	}
}

/* -- MANIPULAR ARCHIVOS Y DIRECTORIOS --*/

// Mueve un subdirectorio o archivo a una destino dado.
bool mover_elemento(Cadena nombre, Cadena nomDestino, directorio dirActual, directorio dirRaiz)
{
	// Retorna true en caso de que la operacion se haya realizado correctamenete.
	// lista_dir ld = dirActual->subdirectorios;
	directorio dDestino = mover_puntero_a_destino(nomDestino, dirRaiz);
	if (dDestino == NULL)
	{
		// No se encontro el directorio destino.
		cout << "No se encontro el directorio destino. \n";
		return false;
	}
	else
	{
		if (pertenece_al_path(nomDestino, dirActual, nombre))
		{
			cout << "No se puede mover a esa carpeta. \n";
			return false;
		}
		directorio buscado = quitar_subdirectorio(nombre, dirActual);
		if (buscado != NULL)
		{
			agregar_subdirectorio(dDestino, buscado, true);
			return true;
		}
		else
		{
			// buscar archivo
			file fbuscado = quitar_archivo(nombre, dirActual);
			if (fbuscado != NULL)
			{
				file x = agregar_archivo(dDestino, fbuscado, true);
				return true;
			}
			else
			{
				cout << "No existe el un archivo o directorio en la ruta dada.";
				return false;
			}
		}
	}
}

// Coloca en el archivo el nuevo Atributo.
bool cambiar_atributo(file archivo, Cadena atributo)
{
	// Devuelve true en caso de realizar la operacion correctamente, false en caso contrario.
	// Busco el archivo en la lista de archivos del directorio.

	Atributo atri;
	// Compruebo y asigno a la variable de tipo Atributo el valor que debo colocar en el archivo.
	if (strcasecmp(atributo, "+W") == 0)
	{
		atri = Lectura_Escritura;
	}
	else if (strcasecmp(atributo, "-W") == 0)
	{
		atri = Lectura;
	}
	else
	{
		return false;
	}
	archivo->atr = atri;
	return true;
}

// Quita el directorio de nombre dado, de la lista de directorios del padre. Devuelve el directorio que ha sido desenganchado.
directorio quitar_subdirectorio(Cadena nombre, directorio padre)
{
	lista_dir ld = padre->subdirectorios;
	if (ld == NULL)
		return NULL;
	if (strcmp(ld->dir->nombrecarpeta, nombre) == 0)
	{
		directorio result = ld->dir;
		lista_dir aBorrar = padre->subdirectorios;
		padre->subdirectorios = padre->subdirectorios->sig;
		delete aBorrar;
		return result;
	}
	while (ld->sig != NULL && strcmp(ld->sig->dir->nombrecarpeta, nombre) != 0)
	{
		ld = ld->sig;
	}
	if (ld->sig == NULL)
	{
		return NULL;
	}
	else
	{
		directorio result = ld->sig->dir;
		lista_dir aBorrar = ld->sig;
		ld->sig = ld->sig->sig;
		delete aBorrar;
		return result;
	}
}

// Quita el archivo de nombre dado, de la lista de archivos del padre. Devuelve el archivo que ha sido desenganchado.
file quitar_archivo(Cadena nombreArchivo, directorio padre)
{
	Cadena nombre = strtok(nombreArchivo, ".");
	Cadena ext = strtok(NULL, ".");
	if (nombre == NULL || ext == NULL)
	{
		return NULL;
	}

	lista_file lf = padre->archivos;
	if (lf == NULL)
		return NULL;
	if (strcmp(lf->archi->nombreArchivo, nombre) == 0 && strcmp(lf->archi->extencion, ext) == 0)
	{
		file result = lf->archi;
		lista_file aBorrar = padre->archivos;
		padre->archivos = padre->archivos->sig;
		delete aBorrar;
		return result;
	}
	while (lf->sig != NULL && (strcmp(lf->archi->nombreArchivo, nombre) != 0) || (strcmp(lf->archi->extencion, ext) != 0))
	{
		lf = lf->sig;
	}
	if (lf->sig == NULL)
	{
		return NULL;
	}
	else
	{
		file result = lf->sig->archi;
		lista_file aBorrar = lf->sig;
		lf->sig = lf->sig->sig;
		delete aBorrar;
		return result;
	}
}

/* -- Recorrer y mostrar estructura -- */

// Me muevo al directorio final de la cadena de directorios dada como paramentro.
directorio mover_puntero_a_destino(Cadena dirDestino, directorio dirRaiz)
{
	// Se da un puntero al directorio RAIZ para realizar la busqueda del directorio destino.
	directorio dirFinal = dirRaiz;
	Cadena head = strtok(dirDestino, "/");
	Cadena tail = strtok(NULL, "/");
	if (strcmp(head, "RAIZ") != 0)
	{
		return NULL;
	}
	while (tail != NULL && dirFinal != NULL)
	{
		// Parto la cadena de directorios por /. Pre-condicion: Ningun directorio puede poseer una "/" en su nombre.
		// Por cada token de la cadena encontrada, busco el directorio dentro del sistema. En el momento que no se encuentre un directorio dado, dirFinal == NULL terminando la iteracion.
		dirFinal = ir_directorio(dirFinal, tail);
		head = tail;
		tail = strtok(NULL, "/");
	}
	cout << "encontre el directorio? " << dirFinal->nombrecarpeta;
	return dirFinal;
}

// Devuelvo un puntero al directorio solicitado.
directorio ir_directorio(directorio dir, Cadena nombreDir)
{
	// Si no encuentro el subdirectorio devuelvo NULL.
	lista_dir ld = dir->subdirectorios;
	while (ld != NULL && strcmp(ld->dir->nombrecarpeta, nombreDir) != 0)
	{
		ld = ld->sig;
	}
	if (ld == NULL)
	{ // Caso: No encuentro el nombre de directorio dentro del directorio dado.
		return NULL;
	}
	// Caso: Encontre el directorio.
	return ld->dir;
}

// Busca si el directorio aBorrar pertenece al path del padre.
bool pertenece_al_path(Cadena pathDestino, directorio dir, Cadena nombreCarpeta)
{
	// Obtengo la cadena del path actual para compararla con la del path aBorrar.
	Cadena pathActual = obtener_path(pathActual, dir);
	strcat(pathActual, "/");
	strcat(pathActual, nombreCarpeta);
	unsigned int lenActual = strlen(pathActual);
	unsigned int lenDestino = strlen(pathDestino);

	// Si el pathActual es mayor al pathDestino, pathDestino no es subdirectorio del actual.
	if (pathActual > pathDestino)
	{
		return false;
	}
	else if (strncmp(pathActual, pathDestino, lenDestino) == 0)
	{
		// Si son iguales pathDestino es subdirectorio del actual.
		return true;
	}
	else
	{
		// No comparten path.
		return false;
	}
}

// Obtiene una cadena con los directorios separados por /.
Cadena obtener_path(Cadena nombre, directorio dir)
{
	if (dir->padre == NULL)
	{
		strcat(nombre, dir->nombrecarpeta);
		return nombre;
	}
	if (dir != NULL)
	{
		obtener_path(nombre, dir->padre);
		strcat(nombre, "/");
		strcat(nombre, dir->nombrecarpeta);
	}
	return nombre;
}

// Devuelvo el padre del directorio.
directorio ir_al_padre(directorio dir)
{
	return dir->padre;
}

// Compruebo la existencia del directorio pasado.
directorio existe_directorio(Cadena nombre, directorio dir)
{
	// Devuelve True en caso de encontrar el directorio pasado.
	lista_dir ld = dir->subdirectorios;
	if (ld == NULL)
	{
		return NULL;
	}
	else
	{
		while (ld != NULL && strcmp(ld->dir->nombrecarpeta, nombre) != 0)
		{
			ld = ld->sig;
		}
		if (ld == NULL)
			return NULL;
		else
			return ld->dir;
	}
}

// Comprueba la existencia del archivo pasado.// BORRAR
file existe_archivo(Cadena nombreArchivo, directorio dir)
{
	// Devuelve el archivo en caso de encontrarlo, NULL en caso contrario.
	lista_file fl = dir->archivos;
	Cadena aux = new char[4];
	Cadena nombre = strtok(nombreArchivo, ".");
	Cadena ext = strtok(NULL, ".");
	if (nombre == NULL)
	{
		// No existe texto antes del punto o despues del mismo.
		cout << "No se ingreso un nombre de archivo correcto.";
		return NULL;
	}
	if (fl == NULL)
	{
		delete[] aux;
		return NULL;
	}
	else if (ext == NULL)
	{
		strcpy(aux, "txt");
	}
	else
	{
		strcpy(aux, ext);
	}
	// Separo la extencion y el nombre del paramentro.
	while (fl != NULL && (strcmp(fl->archi->nombreArchivo, nombre) != 0 || strcmp(fl->archi->extencion, aux) != 0))
	{
		fl = fl->sig;
	}
	if (fl == NULL)
	{
		delete[] aux;
		return NULL;
	}
	else
	{
		delete[] aux;
		return fl->archi;
	}
}

// Imprime el nombre de archivo y su extencion.
void imprimir_nombre_archivo(file archi)
{
	cout << archi->nombreArchivo << '.' << archi->extencion << "\t";
}

// Imprime el path recursivamente.
void imprimir_path(directorio dir)
{

	if (dir == NULL)
	{
		return;
	}
	if (dir->padre == NULL)
	{
		cout << dir->nombrecarpeta;
		return;
	}
	imprimir_path(dir->padre);
	cout << '/' << dir->nombrecarpeta;
}

// Imprime todos los subdirectorios de dir en forma ordenada y recursivamente.
void imprimir_directorio(directorio dir)
{ /*Crear funcion que genere una lista de archivos ordenada*/
	imprimir_path(dir);
	cout << "\n";
	lista_file lf = dir->archivos;
	while (lf != NULL)
	{
		// Imprimo el camino del directorio y al final agreaga el archivo en caso de que exista.
		imprimir_path(dir);
		cout << "/";
		imprimir_nombre_archivo(lf->archi);
		cout << "\n";
		lf = lf->sig;
	}
	lista_dir ld = dir->subdirectorios;
	while (ld != NULL)
	{
		// Recurcion: Una vez se hayan impreso todos los archivos del directorio, ingreso a los subdirectorios del directorio.
		// una vez no existan mas subdirectorios me muevo al directorio siguiente.
		imprimir_directorio(ld->dir);
		ld = ld->sig;
	}
}

// Imprime solo los directorios y archivos (Junto con sus permisos) del directorio dado.
void imprimir_directorio_v2(directorio dir)
{
	imprimir_path(dir);
	cout << "\n";
	lista_file lf = dir->archivos;
	while (lf != NULL)
	{
		imprimir_nombre_archivo(lf->archi);
		// Manejo la impresion de el enumerado Atributo.
		if (lf->archi->atr == Lectura)
		{
			cout << "Lectura";
		}
		else if (lf->archi->atr = Lectura_Escritura)
		{
			cout << "Lectura/Escritura";
		}
		else
		{
			cout << "Escritura";
		}
		cout << "\n";
		lf = lf->sig;
	}
	lista_dir ld = dir->subdirectorios;
	while (ld != NULL)
	{ // Imprimo los subdirectorios.
		cout << ld->dir->nombrecarpeta << "\n";
		ld = ld->sig;
	}
}

/* -- MANIPULAR EL ATRIBUTO TEXTO DE LOS ARCHIVOS -- */

// Inserta un texto al inicio de archi->texto.
bool insertar_texto_inicio(file archivo, Cadena texto)
{
	if (archivo->atr == Lectura_Escritura)
	{
		if (archivo->texto == NULL)
		{
			// Primera inserccion de texto en el archivo.
			Cadena txt = new char[MAX_COMANDO];
			strcpy(txt, texto);
			Cadena temp = archivo->texto;
			archivo->texto = txt;
			delete[] temp;
			return true;
		}
		else
		{
			// No es la primera inserccion de texto en el archivo.
			int cantHay = strlen(texto);
			int cantPuedo = MAX_COMANDO - cantHay;
			strncat(texto, " ", cantPuedo);
			strcat(texto, archivo->texto);
			strncpy(archivo->texto, texto, MAX_COMANDO);
			return true;
		}
	}
	else
	{
		cout << "El archivo es de solo lectura.\n";
		return false;
	}
}

// Inserta un texto al final de archi->texto.
bool insertar_texto_final(file archivo, Cadena texto)
{

	if (archivo->atr == Lectura_Escritura)
	{
		if (archivo->texto == NULL)
		{
			// Primera inserccion de texto en el archivo.
			Cadena txt = new char[MAX_COMANDO];
			strcpy(txt, texto);
			Cadena temp = archivo->texto;
			archivo->texto = txt;
			delete[] temp;
			return true;
		}
		else
		{
			// No es la primera inserccion de texto en el archivo.
			int cantHay = strlen(archivo->texto);
			int cantPuedo = MAX_COMANDO - cantHay;
			strncat(archivo->texto, " ", cantPuedo);
			strncat(archivo->texto, texto, cantPuedo - 1);
			return true;
		}
	}
	else
	{
		cout << "El archivo es de solo lectura.\n";
		return false;
	}
}

// Eliminar del inicio del texto una cantidad dada de caracteres.
bool eliminar_K_elementos_iniciales(file archivo, int K)
{

	if (archivo->atr == Lectura_Escritura)
	{
		if (archivo->texto == NULL)
		{
			cout << "El archivo esta vacio! ";
			return false;
		}
		Cadena textElim = new char[MAX_COMANDO];
		int tam = strlen(archivo->texto);
		if (K > tam)
		{
			// Tengo que eliminar todo el texto.
			textElim = NULL;
			Cadena temp = archivo->texto;
			archivo->texto = textElim;
			delete[] temp;
		}
		else
		{
			// Va a quedar texto en la cadane.
			for (int i = K; i < tam; i++)
			{
				textElim[i - K] = archivo->texto[i];
			}
			// Luego de quitar la cantidad dada inserto al final un \0 para que me quede la cadena completa.
			textElim[tam - K] = '\0';
			strncpy(archivo->texto, textElim, MAX_COMANDO);
			delete[] textElim;
			return true;
		}
	}
	else
	{
		cout << "El archivo es de solo lectura. \n";
		return false;
	}
	return false;
}

// Eliminar del final del texto una cantidad dada de caracteres.
bool eliminar_K_elementos_finales(file archivo, int K)
{
	// Es el primero // Primera inserccion de texto en el archivo.de la lista.
	if (archivo->atr == Lectura_Escritura)
	{
		if (archivo->texto == NULL)
		{
			cout << "El archivo esta vacio! ";
			return false;
		}
		Cadena textElim = new char[MAX_COMANDO];
		int tam = strlen(archivo->texto);
		if (K > tam)
		{
			textElim = NULL;
			Cadena temp = archivo->texto;
			archivo->texto = textElim;
			delete[] temp;
		}
		else
		{
			for (int i = 0; i < tam - K; i++)
			{
				textElim[i] = archivo->texto[i];
			}
			textElim[tam - K] = '\0';
			strncpy(archivo->texto, textElim, MAX_COMANDO);
			delete[] textElim;
			return true;
		}
	}
	else
	{
		cout << "El archivo es de solo lectura. \n";
		return false;
	}
	return false;
}

// Imprime el contenido del archivo dado.
bool imprimir_texto(file archivo)
{
	if (archivo->texto == NULL)
	{
		cout << "El archivo esta vacio! ";
		return false;
	}
	else
	{
		int cont = 0;
		for (int i = 0; archivo->texto[i] != '\0'; i++)
		{
			cont++;
		}
		for (int i = 0; i <= cont + 4; i++)
		{
			cout << "_";
		}
		cout << "\n"
			 << "\n";
		cout << ">> "
			 << "\"" << archivo->texto
			 << "\"";
		cout << "\n";
		for (int i = 0; i <= cont + 4; i++)
		{
			cout << "_";
		}
		cout << "|";
		return true;
	}
	return false;
}

// Busca el archivo dado.
bool search_texto(file archivo, Cadena texto)
{

	// Funcion que busca la primera aparicion del texto en la cadena dada, devuelve un puntero al primer caracter.
	Cadena resultado = strstr(archivo->texto, texto);
	if (resultado != NULL)
	{
		cout << "El texto buscado forma parte del archivo.";
		return true;
	}
	else
	{
		cout << "No existe el texto buscado en el archivo! \n";
		return false;
	}
}

// Busca el archivo dado y remplaza el texto (si es que existe) por el texto 2.
bool remplazar_texto(file archivo, Cadena texto, Cadena texto2)
{
	// Busca el archivo en la lista de directorios.
	Cadena resultado = strstr(archivo->texto, texto);
	if (resultado != NULL)
	{
		// Se encontro el texto en lf->archi->texto.
		while (resultado != NULL)
		{
			// Itero mientras se siga dando la ocurrencia en el texto.
			Cadena txtTemp = new char[MAX_COMANDO];
			int tamBuscado = strlen(texto);
			int tamRemplazo = strlen(texto2);
			int tamResul = strlen(resultado);
			int tamDiff = tamRemplazo - tamBuscado;
			int tamDiff2 = tamBuscado - tamRemplazo;

			for (int i = 0; i < tamResul + tamDiff; i++)
			{
				// Recorro el texto desde el puntero resultado agregandole o restando (dependiendo del caso) la diferencia de tamaño de los dos textos.
				if (i <= tamRemplazo && texto2[i] != '\0')
				{
					// Agrego en los primeros lugares el contenido de texto2.
					txtTemp[i] = texto2[i];
				}
				else
				{
					// Luego de agregado texto2, inserto el resto del puntero resultado, omitiendo el resto de texto o haciendo mas lugar para texto2.
					txtTemp[i] = resultado[i + tamDiff2];
				}
			}
			for (int i = 0; i < tamResul + tamDiff; i++)
			{
				// Copio en resultado txtTemp con el texto ya remplasado.
				resultado[i] = txtTemp[i];
			}
			// Inserto el valor nulo para dejar la cadena corectamente.
			resultado[tamResul + tamDiff] = '\0';
			// Vuelvo a comprobar que no existan mas ocurrencias en lo que queda de texto.
			resultado = strstr(archivo->texto, texto);
			delete[] txtTemp;
		}
		return true;
	}
	else
	{
		cout << texto << " no forma parte del texto del archivo.";
		return false;
	}
}
