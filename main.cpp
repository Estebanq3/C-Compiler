#include <iostream>
using namespace std;
#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "compiladorSibo.h"

CompiladorSIBO compil;

 int main(){ //lee el archivo a compilar, un máximo de 15 caractéres, y posteriormente lo compila para generar un output de un archivo sbml

	char inFileName[ 15 ] = "";
	char nombreArchivoSalida[ 15 ] = "";
	int last = 0;

	cout << "Introduzca el archivo a compilar: ";
	cin >> setw( 15 ) >> inFileName;
	while ( isalnum( inFileName[ last ] ) != 0 ) {
		nombreArchivoSalida[ last ] = inFileName[ last ];
		last++;
	}
	nombreArchivoSalida[ last ] = '\0'; // se le anexa un caracter nulo a final
	strcat( nombreArchivoSalida, ".sbml" ); // añade sblm al archivo de salida
	ifstream inFile( inFileName, ios::in );
	if ( inFile ){
		cout<< "Entrando a Compilar"<<endl; //borrar
	 	compil.compilar( inFile, nombreArchivoSalida );
	}
	else{
	 	cerr << "Error al abrir archivo. Terminando el proceso.\n";
	}
	return 0;
}

