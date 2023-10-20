
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
// Crea un directorio nuevo con el nombre dado por parametro;
directorio crear_directorio(Cadena nombre, directorio dirP)
{
	// Crea un directorio vacio de nombre "nombre".
	directorio d = new (nodo_directorio);
	d->nombrecarpeta = new char[MAX_NOM_DIR];
	strcpy(d->nombrecarpeta, nombre);
	d->subdirectorios = NULL;
	d->archivos = NULL;
	d->padre = dirP; // Si la funcion es llamada por CREARSISTEMA dirP == NULL.
	if (dirP != NULL)
	{
		// crear lista de archivos vacia
		lista_dir ld = new (nodo_lista_dir);
		ld->dir = d;
		ld->sig = dirP->subdirectorios;
		dirP->subdirectorios = ld;
	}

	return d; // Retorno nuevo directorio.
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

// Devuelvo el padre del directorio.
directorio ir_al_padre(directorio dir)
{
	return dir->padre;
}

// Compruebo la existencia del directorio pasado.
bool existe_directorio(Cadena nombre, directorio dir)
{
	// Devuelve True en caso de encontrar el directorio pasado.
	lista_dir ld = dir->subdirectorios;
	if (ld == NULL)
	{
		return false;
	}
	else
	{
		while (ld != NULL && strcmp(ld->dir->nombrecarpeta, nombre) != 0)
		{
			ld = ld->sig;
		}
		if (ld == NULL)
			return false;
		else
			return true;
	}
}

// Comprueba la existencia del archivo pasado.// BORRAR
file existe_archivo(Cadena nombreArchivo, directorio dir)
{
	// Devuelve el archivo en caso de encontrarlo, NULL en caso contrario.
	lista_file fl = dir->archivos;
	if (fl == NULL)
	{
		return NULL;
	}
	else
	{ // Separo la extencion y el nombre del paramentro.
		Cadena nombre = strtok(nombreArchivo, ".");
		Cadena ext = strtok(NULL, ".");
		if (nombre == NULL || ext == NULL)
		{
			cout << "No se ingreso una extencion. \n";
			return NULL;
		}
		while (fl != NULL && (strcmp(fl->archi->nombreArchivo, nombre) != 0 || strcmp(fl->archi->extencion, ext) != 0))
		{
			fl = fl->sig;
		}
		if (fl == NULL)
		{
			cout << "No existe el archivo dentro del directorio.";
			return NULL;
		}
		else
		{
			return fl->archi;
		}
	}
}

// Obtengo el directorio solicitado.
directorio obtener_subdirectorio(Cadena nombre, directorio dir)
{
	// Pre condicion: la lista de subdirectorios no es vacio y la carpera con el nombre existe en ella.

	bool flg = true;
	lista_dir ld = dir->subdirectorios;
	do
	{
		if (strcmp(nombre, ld->dir->nombrecarpeta) == 0)
		{
			flg = false;
		}
		else
		{
			ld = ld->sig;
		}
	} while ((ld->sig != NULL) && (flg == true));
	return ld->dir;
}

// Elimina el directorio con el nombre dado.
bool remover_directorio(Cadena nombre, directorio dir)
{
	// Devuelvo true en caso de encontrar y eliminar el directorio, false en caso contrario.
	lista_dir ld = dir->subdirectorios;
	lista_dir aux = ld;
	if (ld == NULL)
	{ // Compruebo que el directorio dado posea subdirectorios.
		cout << "No existen directorios en " << dir->nombrecarpeta << ". \n";
		return false;
	}
	else
	{ // Busco el nombre dentro de la lista de directorios.
		while (ld != NULL && strcmp(ld->dir->nombrecarpeta, nombre) != 0)
		{
			aux = ld; // aux pasa a ser ld anterior.
			ld = ld->sig;
		}
		if (ld == NULL)
		{ // Si salgo por NULL, no encontre el nombre de directorio.
			cout << "No se encontro el directorio dentro de " << dir->nombrecarpeta << ". \n";
			return false;
		}
		else if (dir->subdirectorios == ld)
		{ // Encontre el directorio y es el primero de la lista.
			dir->subdirectorios = ld->sig;
			ld->dir = destruir_directorio(ld->dir);
			delete ld;
			return true;
		}
		else
		{ // Encontre el directorio y no es el primero.
			aux->sig = ld->sig;
			ld->dir = destruir_directorio(ld->dir);
			delete ld;
			return true;
		}
	}
}

// Devuelve el directorio primero/anterior buscado, NULL en caso de no encontrarse el directorio.
lista_dir buscar_directorio_lista(Cadena nombre, lista_dir ld)
{
	// Si el nombre es el primero de la lista, devuelvo el primero y no el anterior.
	//  ld puede ser NULL.
	if (ld != NULL)
	{
		lista_dir dAux = ld;
		// Busco si existe el directorio con el nombre dado dentro de s->actual.
		while (ld != NULL && strcmp(ld->dir->nombrecarpeta, nombre) != 0)
		{
			dAux = ld;
			ld = ld->sig;
		}
		if (ld == NULL)
		{
			// No existe directorio con ese nombre.
			return NULL;
		}
		else
		{
			// dAux == ld en caso de ser el primero, dAux->sig = ld en cualquier otro caso.
			return dAux;
		}
	}
	else
	{
		return NULL;
	}
}

// Devuelve el archivo primero/anterior de una lista de archivos, NULL en caso de no encontrarse el directorio.
lista_file buscar_archivo_en_lista(Cadena nombreArchivo, lista_file fl)
{
	// Pfl puede ser NULL.
	if (fl != NULL)
	{
		Cadena nombre = strtok(nombre, ".");
		Cadena ext = strtok(NULL, ".");
		if (nombre == NULL || ext == NULL)
		{
			// No existe texto antes del punto o despues del mismo.
			return NULL;
		}
		else
		{
			// Busco el archivo, si el archivo es el primero, devuelvo el primero, en caso contrario devuelvo el anterior al buscado.
			lista_file fAux = fl;
			while (fl != NULL && (strcmp(fl->archi->nombreArchivo, nombre) != 0 || strcmp(fl->archi->extencion, ext) != 0))
			{
				fAux = fl; // Me paro en el anterior.
				fl = fl->sig;
			}
			if (fl == NULL)
			{
				return NULL;
			}
			else
			{
				// fAux == fl si es el primero, fAux->sig == fl en cualquier otro.
				return fAux;
			}
		}
	}
	else
	{
		// No existen archivos en el directorio
		return NULL;
	}
}

// Mueve un subdirectorio o archivo a una destino dado.
bool mover_elemento(Cadena nombre, Cadena nomDestino, directorio dirActual, directorio dirRaiz)
{
	// Retorna true en caso de que la operacion se haya realizado correctamenete.
	lista_dir ld = dirActual->subdirectorios;
	directorio dDestino = mover_puntero_a_destino(nomDestino, dirRaiz);
	if (dDestino == NULL)
	{
		// No se encontro el directorio destino.
		cout << "No se encontro el directorio destino. \n";
		return false;
	}
	else
	{
		lista_dir dAux = buscar_directorio_lista(nombre, dirActual->subdirectorios);
		if (dAux != NULL)
		{
			// Encontre un subdirectorio con el nombre dado.
			// Procedo a buscar en el directorio destino que no exista uno con el mismo nombre.
			lista_dir dirD = buscar_directorio_lista(nombre, dDestino->subdirectorios);
			if (pertenece_al_path(nomDestino, dirActual, dirD->dir))
			{
				return false;
			}
			else if (dirD != NULL)
			{
				// Se encontro un directorio con el mismo nombre en el destino.
				// Se elimina ese directorio dentro del directorio destino.
				remover_directorio(nombre, dDestino);
			}
			// Procedo a enganchar los directorios.
			if (strcmp(dAux->dir->nombrecarpeta, nombre) == 0)
			{
				lista_dir d = dDestino->subdirectorios;
				lista_dir dirSwap = dirActual->subdirectorios;
				dirActual->subdirectorios = dirActual->subdirectorios->sig;
				dDestino->subdirectorios = dirSwap;
				dDestino->subdirectorios->sig = d;
				dirSwap->dir->padre = dDestino;
				return true;
			}
			else
			{
				lista_dir aux = dAux->sig;
				dAux->sig = dAux->sig->sig;
				lista_dir d = dDestino->subdirectorios;
				dDestino->subdirectorios = aux;
				dDestino->subdirectorios->sig = d;
				dDestino->subdirectorios->dir->padre = dDestino;
				return true;
			}
		}
		else
		{
			// No encontre directorios con el nombre dado. Busco archivos.
			lista_file fl = dirActual->archivos;
			lista_file fAux = buscar_archivo_en_lista(nombre, fl);
			if (fAux != NULL)
			{
				// Encontre el archivo.
				lista_file fileD = buscar_archivo_en_lista(nombre, dDestino->archivos);
				if (fileD != NULL)
				{
					// Existe un archivo con el mismo nombre dentro del directorio destino.
					// Destruyo ese archivo.
					remover_archivo(nombre, fileD, dDestino); // fileD es el anterior del que se desea borrar.
				}
				if (strcmp(fAux->archi->nombreArchivo, nombre) == 0)
				{
					// Procedo a enganchar.
					// El archivo a eliminar es el primero de la lista.
					lista_file f = dDestino->archivos;
					lista_file dirSwap = dirActual->archivos;
					dirActual->archivos = dirActual->archivos->sig;
					dDestino->archivos = dirSwap;
					dDestino->archivos->sig = f;
					return true;
				}
				else
				{ // El archivo a mover no es el primero de la lista.
					lista_file aux = fAux->sig;
					fAux->sig = fAux->sig->sig;
					lista_file f = dDestino->archivos;
					dDestino->archivos = aux;
					dDestino->archivos->sig = f;
					return true;
				}
			}
			else
			{
				cout << "No se encontro un directorio y un archivo con ese nombre. \n";
				return false;
			}
		}
	}
}

// Me muevo al directorio final de la cadena de directorios dada como paramentro.
directorio mover_puntero_a_destino(Cadena dirDestino, directorio dirRaiz)
{
	// Se da un puntero al directorio RAIZ para realizar la busqueda del directorio destino.
	directorio dirFinal = dirRaiz;
	Cadena head = strtok(dirDestino, "/");
	Cadena tail = strtok(NULL, "/");
	Cadena destino = dirDestino;
	if (strcmp(head, "RAIZ") != 0)
	{
		return NULL;
	}

	while (tail != NULL && dirFinal != NULL)
	{

		// Parto la cadena de directorios por /. Pre-condicion: Ningun directorio puede poseer una "/" en su nombre.
		// Por cada token de la cadena encontrada, busco el directorio dentro del sistema. En el momento que no se encuentre un directorio dado, dirFinal == NULL terminando la iteracion.
		head = strtok(tail, "/");
		tail = strtok(NULL, "/");
		cout << "buscar  " << head << " en " << dirFinal->nombrecarpeta << "\n";
		dirFinal = ir_directorio(dirFinal, head);
	}
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

bool pertenece_al_path(Cadena pathDestino, directorio dir, directorio aBorrar)
{
	Cadena pathActual = new char[100];
	pathActual = obtener_path(pathActual, dir);
	strcat(pathActual, "/");
	strcat(pathActual, aBorrar->nombrecarpeta);
	unsigned int lenActual = strlen(pathActual);
	unsigned int lenDestino = strlen(pathDestino);

	if (pathActual > pathDestino)
	{
		return false;
	}
	else if (strncmp(pathActual, pathDestino, lenDestino) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// Destruye el archivo dado, sin importar el Atributo.
bool remover_archivo(Cadena nombre, lista_file lf, directorio dir)
{
	// Pre-condicion: lf no es NULL.
	// lf puede ser el anterior a borrar o el primero, compruebo cual es el caso y ejecuto la accion de borrar en base a eso.
	if (lf->archi->atr == Lectura)
	{
		cout << "No es posible eliminar el archivo " << lf->archi->nombreArchivo << ", ya que es de solo lectura. \n";
		return false;
	}
	else if (strcmp(lf->archi->nombreArchivo, nombre) == 0)
	{
		if (lf->archi->atr == Lectura)
		{
			cout << "No es posible eliminar el archivo " << lf->archi->nombreArchivo << ", ya que es de solo lectura. \n";
			return false;
		}
		// Engancha la lista si es el primero de la lista.
		dir->archivos = lf->sig->sig;
		lf->archi = destruir_archivo(lf->archi);
		delete lf->sig;
		return true;
	}
	else
	{
		if (lf->sig->archi->atr == Lectura)
		{
			cout << "No es posible eliminar el archivo " << lf->archi->nombreArchivo << ", ya que es de solo lectura. \n";
			return false;
		}
		// Engancha la lista si no es el primero.
		// Lista es el miembro anterior a destruir de la lista.
		lista_file archiDestruir = lf->sig;
		lf->sig = lf->sig->sig;
		archiDestruir->archi = destruir_archivo(archiDestruir->archi);
		delete archiDestruir;
		return true;
	}
}

Cadena obtener_path(Cadena nombre, directorio dir)
{
	if (dir == NULL)
	{
		return nombre;
	}
	if (dir->padre == NULL)
	{
		strcat(nombre, dir->nombrecarpeta);
		return nombre;
	}
	imprimir_path(dir->padre);
	strcat(nombre, "/");
	strcat(nombre, dir->nombrecarpeta);
}
// Destruye el archivo dado.
bool buscar_destruir_archivo_(Cadena nombre, Cadena ext, directorio dir)
{

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

// Crea un archivo vacio nuevo. Precondicion: nombre y ext no nulos.
void crear_archivo(Cadena nombre, Cadena ext, directorio dir)
{

	Cadena str = strtok(nombre, ".");
	file f = new (nodo_file);
	f->nombreArchivo = new char(MAX_NOM_ARCH);
	strcpy(f->nombreArchivo, nombre);
	f->extencion = new char(MAX_EXT_ARCH);
	if(ext == NULL){
		strcpy(f->extencion, ".txt");
	}
	else{
		strcpy(f->extencion, ext);
	}
	strcpy(f->extencion, ext);
	f->atr = Lectura_Escritura;
	f->texto = NULL;
	// Engancho el nuevo archivo en la lista de archivos del directorio.
	lista_file ld = new (nodo_lista_file);
	ld->archi = f;
	ld->sig = dir->archivos;
	dir->archivos = ld;
}

// Coloca en el archivo el nuevo Atributo.
bool cambiar_atributo(Cadena nombre, Cadena ext, Atributo atr, directorio dir)
{
	// Devuelve true en caso de realizar la operacion correctamente, false en caso contrario.
	// Busco el archivo en la lista de archivos del directorio.
	lista_file lf = dir->archivos;
	while (lf != NULL && (strcmp(lf->archi->nombreArchivo, nombre) != 0 && (strcmp(lf->archi->extencion, ext) != 0)))
	{
		lf = lf->sig;
	}
	if (lf == NULL)
	{
		cout << "El archivo no existe dentro del directorio. \n";
		return false;
	}
	else
	{
		lf->archi->atr = atr;
		return true;
	}
}

bool insertar_texto_archivo(Cadena nombreArchivo, Cadena texto, directorio dir)
{

	lista_file fl = buscar_archivo_en_lista(nombreArchivo, dir->archivos);
	if (fl != NULL)
	{
		if (strcmp(fl->archi->nombreArchivo, nombreArchivo) == 0)
		{
			// Es el primero de la lista.
			if (fl->archi->atr == Lectura_Escritura)
			{
				Cadena txt = new char[MAX_COMANDO];
				strcpy(texto, txt);
				fl->archi->texto = txt;
				return true;
			}
			else
			{
				cout << "El archivo es de solo lectura.\n";
				return false;
			}
		}
		else
		{
			// NO es el primero de la lista.
			if (fl->sig->archi->atr == Lectura_Escritura)
			{
				Cadena txt = new char[MAX_COMANDO];
				strcpy(texto, txt);
				fl->sig->archi->texto = txt;
				return true;
			}
			else
			{
				cout << "El archivo es de solo lectura. \n";
				return false;
			}
		}
	}
	else
	{
		cout << "No se encontro un archivo con el nombre dado.\n";
		return false;
	}
}

bool insertar_texto_final(Cadena nombreArchivo, Cadena texto, directorio dir)
{
	lista_file fl = buscar_archivo_en_lista(nombreArchivo, dir->archivos);
	if (fl != NULL)
	{
		if (strcmp(fl->archi->nombreArchivo, nombreArchivo) == 0)
		{
			// Es el primero de la lista.
			if (fl->archi->atr == Lectura_Escritura)
			{
				/*MANEJAR EL TAMAÑO DEL TEXTO*/
				Cadena txtArchivo = fl->archi->texto;
				strcat(txtArchivo, texto);
				fl->archi->texto = txtArchivo;
				return true;
			}
			else
			{
				cout << "El archivo es de solo lectura.\n";
				return false;
			}
		}
		else
		{
			// NO es el primero de la lista.
			if (fl->sig->archi->atr == Lectura_Escritura)
			{
				/*MANEJAR EL TAMAÑO DEL TEXTO*/
				Cadena txtArchivo = fl->archi->texto;
				strcat(txtArchivo, texto);
				fl->sig->archi->texto = txtArchivo;
				return true;
			}
			else
			{
				cout << "El archivo es de solo lectura. \n";
				return false;
			}
		}
	}
	else
	{
		cout << "No se encontro un archivo con el nombre dado.\n";
		return false;
	}
}