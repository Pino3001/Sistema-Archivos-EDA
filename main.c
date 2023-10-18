// Estructuras de Datos y Algoritmos - Curso 2023
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// Trabajo Obligatorio
// main.c
// Consola para ingreso de datos.

#include "definiciones.h"
#include "sistema.h"
#include <string.h>
#include <iostream>

using namespace std;


int main (){
	Sistema s;
	CREARSISTEMA(s);
	char * comando = new (char[MAX_COMANDO]);
	char * pch, * pch1, * pch2, *pch3;
	TipoRet ret;
	bool ejecutado = false;

	bool salir = false;

	cout << "\tCD nombreDirectorio\n";
	cout << "\tMKDIR nombreDirectorio\n";
	cout << "\tRMDIR nombreDirectorio\n";
	cout << "\tMOVE nombre directorioDestino\n";
	cout << "\tDIR parametro\n";
	cout << "\tCREATEFILE nombreArchivo\n";
	cout << "\tDELETE nombreArchivo\n";
	cout << "\tATTRIB nombreArchivo, parametro\n";
	cout << "\tIC nombreArchivo texto\n";
	cout << "\tIF nombreArchivo texto\n";
	cout << "\tDC nombreArchivo k\n";
	cout << "\tDF nombreArchivo k\n";
	cout << "\tTYPE nombreArchivo\n";
	cout << "\tSEARCH nombreArchivo texto\n";
	cout << "\tREPLACE nombreArchivo texto1 texto2\n";
	cout << "\tSALIR\n\n";
	
	do{

		cout << "> ";
		fflush( stdin );
		ejecutado = false;
		
		fgets (comando, MAX_COMANDO, stdin);
		
		pch = strtok (comando,"( ,)\n");
		
		if (strcasecmp (pch, "CD") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = CD (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "MKDIR") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = MKDIR (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "RMDIR") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = RMDIR (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "MOVE") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = MOVE (s, pch, pch1);
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "DIR") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
				ret = DIR (s, pch);
			else
				ret = DIR (s, NULL);
			ejecutado = true;
		}
		else if (strcasecmp (pch, "CREATEFILE") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = CREATEFILE (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "DELETE") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = DELETE (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "ATTRIB") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = ATTRIB (s, pch, pch1);
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}		
		else if (strcasecmp (pch, "IC") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = IC (s, pch, pch1);
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "IF") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = IF (s, pch, pch1);
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "DC") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = DC (s, pch, atoi(pch1));
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}				
			else if (strcasecmp (pch, "DF") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = DF (s, pch, atoi(pch1));
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "TYPE") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				ret = TYPE (s, pch);
				ejecutado = true;
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}				
		else if (strcasecmp (pch, "SEARCH") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL)
			{
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					ret = SEARCH (s, pch, pch1);
					ejecutado = true;
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "REPLACE") == 0){
			pch = strtok (NULL, "( ,)\n");
			if (pch != NULL){
				pch1 = strtok (NULL, "( ,)\n");
				if (pch1 != NULL){
					pch2 = strtok (NULL, "( ,)\n");
					if (pch2 != NULL){
						ret = REPLACE (s, pch, pch1, pch2);
						ejecutado = true;
					}
					else
						cout << " - ERROR: Faltan Parametros.\n";
				}
				else
					cout << " - ERROR: Faltan Parametros.\n";
			}
			else
				cout << " - ERROR: Faltan Parametros.\n";
		}
		else if (strcasecmp (pch, "salir") == 0){
			salir = true;
		}
		else
			cout << " - Comando Incorrecto\n";
		if (ejecutado){
			if (ret == OK)
				cout << " - OK\n";
			else if (ret == ERROR)
				cout << " - ERROR\n";
			else
				cout << " - NO IMPLEMENTADA\n";
		}
	}while (!salir);
	cout << "\n\n - CHAUU!!!!\n";

	DESTRUIRSISTEMA (s);
	delete [] comando;

}


