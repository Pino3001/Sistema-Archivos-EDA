#ifndef DIRECTORIO_H
#define DIRECTORIO_H

typedef struct nodo_directorio *directorio;

typedef struct nodo_dir_hijo *dir_hijo;

typedef struct nodo_file *file;

typedef struct nodo_lista_file *lista_file;

typedef struct nodo_lista_dir *lista_dir;

// Crea un directorio nuevo con el nombre dado por parametro.
directorio crear_directorio(Cadena nombre, directorio dirP);

// Destruye el directorio pasado como parametro.
directorio destruir_directorio(directorio dir);

// Destruye lista de archivos recursivamente.
lista_file destruir_lista_archivo(lista_file lf);

// Destruir nodo_file.
file destruir_archivo(file a);

// Destruyo lista de directorios recurcivamente.
lista_dir destruir_lista_directorio(lista_dir d);

// Devuelvo el padre del directorio.
directorio ir_al_padre(directorio dir);

// Compruebo la existencia del directorio pasado.
bool existe_directorio(Cadena nombre, directorio dir);

// Comprueba la existencia del archivo pasado.
file existe_archivo(Cadena nombreArchivo, directorio dir);

// Obtengo el directorio solicitado.
directorio obtener_subdirectorio(Cadena nombre, directorio dir);

// Elimina el directorio con el nombre dado.
bool remover_directorio(Cadena nombre, directorio dir);

// Devuelve el directorio primero/anterior buscado, NULL en caso de no encontrarse el directorio.
lista_dir buscar_directorio_lista(Cadena nombre, lista_dir ld);

// Mueve un subdirectorio o archivo a una destino dado.
bool mover_elemento(Cadena nombre, Cadena nomDestino, directorio dir, directorio dirRaiz);

// Me muevo al directorio final de la cadena de directorios dada como paramentro.
directorio mover_puntero_a_destino(Cadena dirDestino, directorio dirRaiz);

// Devuelvo un puntero al directorio solicitado.
directorio ir_directorio(directorio dir, Cadena nombreDir);

bool pertenece_al_path(Cadena pathDestino, directorio dir, directorio aBorrar);

// Destruye el archivo dado.
bool remover_archivo(Cadena nombre, lista_file lf, directorio dir);

bool buscar_destruir_archivo_(Cadena nombre, Cadena ext, directorio dir);

// Imprime todos los subdirectorios de dir en forma ordenada y recursivamente.
void imprimir_directorio(directorio dir);

// Imprime el nombre de archivo y su extencion.
void imprimir_nombre_archivo(file archi);

// Imprime el path recursivamente.
void imprimir_path(directorio dir);

// Imprime solo los directorios y archivos (Junto con sus permisos) del directorio dado.
void imprimir_directorio_v2(directorio dir);

// Crea un archivo vacio nuevo. Precondicion: nombre y ext no nulos.
void crear_archivo(Cadena nombre, Cadena ext, directorio dir);

// Coloca en el archivo el nuevo Atributo.
bool cambiar_atributo(Cadena nombre, Cadena ext, Atributo atr, directorio dir);

bool insertar_texto_archivo(Cadena nombreArchivo, Cadena texto, directorio dir);

bool insertar_texto_final(Cadena nombreArchivo, Cadena texto, directorio dir);
#endif
