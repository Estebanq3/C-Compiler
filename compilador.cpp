#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "stack.h"
#include "compiladorSIBO.h"



 // función compilar llama a la primera pasada y la segunda pasada
 void CompiladorSIBO::compilar( ifstream &input, char *nombreArchivoSalida )
 {
 	cout<<"Compilando"<<endl; //borrar
	 entradaTabla tablaSimbolos[ TAMANOTABLASIMBOLO ]; // symbol table
	 int banderas[ MEMORIA ]; // array for forward references
	 int listaMaquina[ MEMORIA ]; // array for SML instructions
	 inicializarListas( banderas, listaMaquina, tablaSimbolos );
	 primeraPasada( banderas, listaMaquina, tablaSimbolos, input ); //primera pasada
	 segundaPasada( banderas, listaMaquina, tablaSimbolos ); //segunda pasada
	 printOutput( listaMaquina, nombreArchivoSalida );
 }



 // primeraPasada constructs the symbol table, creates SML, and banderas unresolved
 // references for jump and if/jump statements.
 void CompiladorSIBO::primeraPasada( int banderas[], int listaMaquina[], entradaTabla tablaSimbolos[],ifstream &input ){
 	cout<<"Primera pasada de compilador"<<endl; //borrar
	 char array[ TAMANOTABLASIMBOLO ]; // array para copiar una linea simple
	 int n = TAMANOTABLASIMBOLO;
	 int contadorData = MEMORIA - 1; // primer data
	 int contadorInstrucciones = 0; // primera instrucción
	 input.getline( array, n );
	 while ( !input.eof() ) {
		 separarToken( array, banderas, listaMaquina, tablaSimbolos, &contadorData,
		 &contadorInstrucciones );
		 input.getline( array, n );
 	 }
 }

//Tokeniza una declaración simple, procesa el número de línea y pasa el siguiente token al palabraClave para procesarlo
 void CompiladorSIBO::separarToken( char array[], int banderas[], int listaMaquina[],entradaTabla tablaSimbolos[], int *contadorDataPtr,int *contadorInstruccionesPtr ){
	cout<<"Separando tokens/tokenizando"<<endl; //borrar
	 char *tokenPtr = strtok( array, " " ); // tokeniza una linea
	 numeroDeLinea( tokenPtr, tablaSimbolos, *contadorInstruccionesPtr, *contadorDataPtr );
	 tokenPtr = strtok( 0, " \n" ); // obtiene el siguiente toke
	 palabraClave( tokenPtr, banderas, listaMaquina, tablaSimbolos, contadorDataPtr,contadorInstruccionesPtr);
 }

//revisa la tabla de símbolos, busca el símbolo y lo retorna
// se retorna la localización o un -1 si no es encontrado
int CompiladorSIBO::revisarSymTabla( entradaTabla tablaSimbolos[], int symbol, char type ){
 	cout<<"Revisando simbolo en la tabla"<<endl;
	 for ( int i = 0; i < TAMANOTABLASIMBOLO; ++i ){
		if ( ( symbol == tablaSimbolos[ i ].symbol ) && ( type == tablaSimbolos[ i ].type ) ){
		return tablaSimbolos[ i ].location; // retorna sbml localización
		}
	 }
	 return -1; // no encontró el símbolo
 }



//procesa los numeroDeLineas
 void CompiladorSIBO::numeroDeLinea( char *tokenPtr, entradaTabla tablaSimbolos[],
 int contadorInstrucciones, int contadorData )
 {
	 const char type = 'L';
	 int symbol;
	 if ( isdigit( tokenPtr[ 0 ] ) ) {
		 symbol = atoi( tokenPtr );
		 if ( -1 == revisarSymTabla( tablaSimbolos, symbol, type ) )
		 anadirASimbolosTabla( type, symbol, contadorData, tablaSimbolos,
		 contadorInstrucciones );
	 }
 }




 void CompiladorSIBO::palabraClave( char *tokenPtr, int banderas[], int listaMaquina[],entradaTabla tablaSimbolos[], int *contadorDataPtr,int *contadorInstruccionesPtr ){
	if ( strcmp( tokenPtr, "#" ) == 0 ); // no se generan instrucciones por comentarios, ni tampoco se suma uno al contador de instrucciones
	else if ( strcmp( tokenPtr, "input" ) == 0 ) {
		cout<<"Input codificado"<<endl;
		tokenPtr = strtok( 0, " " ); // asigna puntero al siguiente token
		read( tokenPtr, listaMaquina, tablaSimbolos, contadorDataPtr,contadorInstruccionesPtr );
	}
	else if ( strcmp( tokenPtr, "print" ) == 0 ) {
		cout<<"Print codificado"<<endl;
		tokenPtr = strtok( 0, " " ); // asigna puntero al siguiente token
		print( tokenPtr, listaMaquina, tablaSimbolos, contadorDataPtr,contadorInstruccionesPtr );
	}
	else if ( strcmp( tokenPtr, "jump" ) == 0 ) {
		cout<<"Jump simple codificado"<<endl;
		tokenPtr = strtok( 0, " " ); // asigna puntero al siguiente token
		jump( tokenPtr, banderas, listaMaquina, tablaSimbolos, contadorInstruccionesPtr );
	}
	else if ( strcmp( tokenPtr, "if" ) == 0 ) {
		cout<<"If jump codificado"<<endl;
		tokenPtr = strtok( 0, " " ); // asigna puntero al siguiente token
		jumpCondicional( tokenPtr, banderas, listaMaquina, tablaSimbolos, contadorDataPtr,contadorInstruccionesPtr );
	}
	else if ( strcmp( tokenPtr, "end" ) == 0 ) {
		cout<<"End codificado"<<endl;
		listaMaquina[ *contadorInstruccionesPtr ] = HALT * 100;
		++( *contadorInstruccionesPtr );
		tokenPtr = 0; // asigna puntero a 0
	}
	else if ( strcmp( tokenPtr, "set" ) == 0 ) {
		cout<<"Set codificado"<<endl;
		tokenPtr = strtok( 0, " " ); // asigna puntero al siguiente token
		set( tokenPtr, listaMaquina, tablaSimbolos, contadorDataPtr,contadorInstruccionesPtr );
	}
	else if( strcmp( tokenPtr, "for") == 0){
		cout<<"For encontrado"<<endl;
		tokenPtr = strtok( 0, " " );
		
	}
 }




 // read procesa input palabraClaves
 void CompiladorSIBO::read( char *tokenPtr, int listaMaquina[], entradaTabla tablaSimbolos[],int *contadorDataPtr, int *contadorInstruccionesPtr ){
	const char type = 'V'; //variable
	listaMaquina[ *contadorInstruccionesPtr ] = READ * 100;
	int symbol = tokenPtr[ 0 ];
	int tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );
	if ( -1 == tabsetest ) {
		anadirASimbolosTabla( type, symbol, *contadorDataPtr, tablaSimbolos,
		*contadorInstruccionesPtr );
		listaMaquina[ *contadorInstruccionesPtr ] += *contadorDataPtr;
		--( *contadorDataPtr );
	}
	else{
		listaMaquina[ *contadorInstruccionesPtr ] += tabsetest;
		++( *contadorInstruccionesPtr );
	}
 }




// print process print palabraClaves
 void CompiladorSIBO::print( char *tokenPtr, int listaMaquina[], entradaTabla tablaSimbolos[],int *contadorDataPtr, int *contadorInstruccionesPtr ){
	const char type = 'V'; //variable
	listaMaquina[ *contadorInstruccionesPtr ] = WRITE * 100;
	int symbol = tokenPtr[ 0 ];
	int tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

	 if ( -1 == tabsetest ) {
		anadirASimbolosTabla( type, symbol, *contadorDataPtr, tablaSimbolos,
		*contadorInstruccionesPtr );
		listaMaquina[*contadorInstruccionesPtr] += *contadorDataPtr;
		--( *contadorDataPtr );
	 }
	 else{
		listaMaquina[ *contadorInstruccionesPtr ] += tabsetest;
		++( *contadorInstruccionesPtr );
	}
}



// jump process jump palabraClaves
 void CompiladorSIBO::jump( char *tokenPtr, int banderas[], int listaMaquina[],entradaTabla tablaSimbolos[], int *contadorInstruccionesPtr ){
	const char type = 'L'; //linea
	listaMaquina[*contadorInstruccionesPtr] = BRANCH * 100;
	int symbol = atoi( tokenPtr );
	int tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );
	anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
	++( *contadorInstruccionesPtr );
}





// jumpCondicional process if/jump commands
 void CompiladorSIBO::jumpCondicional( char *tokenPtr, int banderas[], int listaMaquina[],entradaTabla tablaSimbolos[], int *contadorDataPtr,int *contadorInstruccionesPtr ){
	int operand1Loc = revisarOperando( tablaSimbolos, tokenPtr, contadorDataPtr,
	contadorInstruccionesPtr, listaMaquina );
	char *operatorPtr = strtok( 0, " " ); // get the operator
	tokenPtr = strtok( 0, " " ); // get the right operand of comparison operator
	int operand2Loc = revisarOperando( tablaSimbolos, tokenPtr, contadorDataPtr,
	contadorInstruccionesPtr, listaMaquina );
	tokenPtr = strtok( 0, " " ); // read in the jump palabraClave
	char *jumpLinePtr = strtok( 0, " " ); // read in the jump line number
	evaluarExpresion( operand1Loc, operand2Loc, operatorPtr, listaMaquina,
	tablaSimbolos, contadorInstruccionesPtr, banderas, jumpLinePtr );
}




// revisarOperando ensures that the operands of an if/jump statement are
 // in the symbol table.
 int CompiladorSIBO::revisarOperando( entradaTabla tablaSimbolos[], char *symPtr, int *contadorDataPtr,int *contadorInstruccionesPtr, int listaMaquina[] ){
	 char type;
	 int tabsetest, operand, temp;
	 if ( isalpha( symPtr[ 0 ] ) ){
		 type = 'V';
		 operand = symPtr[ 0 ];
		 tabsetest = revisarSymTabla( tablaSimbolos, operand, type );

		if ( tabsetest == -1 ) {
			 anadirASimbolosTabla( type, operand, *contadorDataPtr, tablaSimbolos,
			 *contadorInstruccionesPtr );
			 temp = *contadorDataPtr;
			 --( *contadorDataPtr );
			 return temp;
		 }
		else
		return tabsetest;
	 }
	 // if the symbol is a digit or a signed digit si el sím
	 else if ( isdigit( symPtr[ 0 ] ) ||( ( symPtr[ 0 ] == '-' || symPtr[ 0 ] == '+' ) && isdigit( symPtr[ 1 ] ) != 0 ) ) {
		 type = 'C';
		 operand = atoi( symPtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, operand, type );

		 if ( tabsetest == -1 ) {
			 anadirASimbolosTabla( type, operand, *contadorDataPtr, tablaSimbolos,
			 *contadorInstruccionesPtr );
			 listaMaquina[ *contadorDataPtr ] = operand;
			 temp = *contadorDataPtr;
			 --( *contadorDataPtr );
			 return temp ;
		 }
		 else{
		 return tabsetest;
		 }
	 }

 return 0; //retorna para alimentar procesos de compilador
 }




 //evaluarExpresion crea SBML para operadores condicionales
 void CompiladorSIBO::evaluarExpresion( int operator1Loc, int operator2Loc, char *operandPtr,int listaMaquina[], entradaTabla tablaSimbolos[],int *contadorInstruccionesPtr, int banderas[], char *jumpLinePtr ){
	int tabsetest;
	int symbol;
	const char type = 'V';

	 if ( strcmp( operandPtr, "==" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHZERO * 100;
		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );
		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );
	}
	else if ( strcmp( operandPtr, "!=" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;

		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );
		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );

		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;

		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );

		 ++( *contadorInstruccionesPtr );
	 }
	 else if ( strcmp( operandPtr, ">" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;

		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );
	 }
	 else if ( strcmp( operandPtr, "<" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;
		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );
	 }
	else if ( strcmp( operandPtr, ">=" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;

		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHZERO * 100;

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );
	 }
	 else if ( strcmp( operandPtr, "<=" ) == 0 ) {
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator1Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += operator2Loc;
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHNEG * 100;

		 symbol = atoi( jumpLinePtr );
		 tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );

		 listaMaquina[ *contadorInstruccionesPtr ] = BRANCHZERO * 100;

		 anadirLineaABanderas( tabsetest, symbol, banderas, listaMaquina, contadorInstruccionesPtr );
		 ++( *contadorInstruccionesPtr );
	 }
 }


//el segundo paso resuelve las instrucciones SBML incompsetas para futuras referencias
void CompiladorSIBO::segundaPasada( int banderas[], int listaMaquina[], entradaTabla tablaSimbolos[] ){
	cout<<"Realizando segunda pasada para terminar los procesos incompletos"<<endl;
	const char type = 'L';
	for ( int i = 0; i < MEMORIA; ++i ) {
			if ( banderas[ i ] != -1 ) {
				 int symbol = banderas[ i ];
				 int flagLocation = revisarSymTabla( tablaSimbolos, symbol, type );
				 listaMaquina[ i ] += flagLocation;
		    }
	}
}



void CompiladorSIBO::set( char *tokenPtr, int listaMaquina[], entradaTabla tablaSimbolos[],int *contadorDataPtr, int *contadorInstruccionesPtr){
	const char type = 'V';
	char infixArray[ TAMANOTABLASIMBOLO ] = "", postfixArray[ TAMANOTABLASIMBOLO ] = "";
	int tabsetest, symbol, location;
	static int subscript = 0;

	symbol = tokenPtr[ 0 ];
	tabsetest = revisarSymTabla( tablaSimbolos, symbol, type );

	if ( -1 == tabsetest ) {
		anadirASimbolosTabla( type, symbol, *contadorDataPtr, tablaSimbolos,
		*contadorInstruccionesPtr );
		 location = *contadorDataPtr;
		--( *contadorDataPtr );
	 }
	 else{
	 location = tabsetest;
	 tokenPtr = strtok( 0, " " ); // agarra símbolo equivalente
	 tokenPtr = strtok( 0, " " ); // obtiene el siguiente token
	 }

	 while ( tokenPtr != 0 ) {
		 revisarOperando( tablaSimbolos, tokenPtr, contadorDataPtr,
		 contadorInstruccionesPtr, listaMaquina );
		 infixArray[ subscript ] = tokenPtr[ 0 ];
		 ++subscript;
		 tokenPtr = strtok( 0, " " ); // obtiene el siguiente token
	 }

	 infixArray[ subscript ] = '\0';
	 infixToPostfix( infixArray, postfixArray, location, tablaSimbolos,
	 contadorInstruccionesPtr, contadorDataPtr, listaMaquina );
	 subscript = 0; // reset
 }



void CompiladorSIBO::anadirASimbolosTabla( char type, int symbol, int contadorData,entradaTabla tablaSimbolos[], int contadorInstrucciones){
 	cout<<"Anadiendo simbolos a la tabla"<<endl;
	 static int contadorDeSimbolo = 0;
	 tablaSimbolos[ contadorDeSimbolo ].type = type;
	 tablaSimbolos[ contadorDeSimbolo ].symbol = symbol;
	 if ( type == 'V' || type == 'C' ){
	 	tablaSimbolos[ contadorDeSimbolo ].location = contadorData;
	 }
	 else{
		 tablaSimbolos[ contadorDeSimbolo ].location = contadorInstrucciones;
		 ++contadorDeSimbolo;
	 }
 }



 void CompiladorSIBO::anadirLineaABanderas( int tabsetest, int symbol, int banderas[],int listaMaquina[], const int *contadorInstruccionesPtr){
 	cout<<"Anadiendo linea a banderas"<<endl;
	 if ( tabsetest == -1 ){
	 	banderas[ *contadorInstruccionesPtr ] = symbol;
	 }
	 else{
	 	listaMaquina[ *contadorInstruccionesPtr ] += tabsetest;
	 }
 }



 void CompiladorSIBO::printOutput( const int listaMaquina[], char *nombreArchivoSalida ){
 	cout<<"Imprimir archivo de salida"<<endl;
	 ofstream output( nombreArchivoSalida, ios::out );
	 if ( !output ){
	 	cerr << "File was not opened.\n";
	 }
	 else{ // imprime cada celda
		 for ( int i = 0; i <= MEMORIA - 1; ++i ){
		 output << listaMaquina[ i ] << '\n';
		 cout<<"Lista máquina:"<< listaMaquina[i]<<endl;
		 }
	 }
 }


void CompiladorSIBO::inicializarListas( int banderas[], int listaMaquina[], entradaTabla tablaSimbolos[] )
 {
 	entradaTabla initEntry = { 0, 0, -1 };

	for ( int i = 0; i < MEMORIA; ++i ) {
		 banderas[ i ] = -1;
		 listaMaquina[ i ] = 0;
		 tablaSimbolos[ i ] = initEntry;
	}
}





// infixToPostfix converts an infix expression to a postfix expression
void CompiladorSIBO::infixToPostfix( char infix[], char postfix[], int getsVariable,entradaTabla tablaSimbolos[], int *contadorInstruccionesPtr,int *contadorDataPtr, int listaMaquina[] ){
	 Stack< int > intStack;
	 int contadorInfix, postfixCount, popValue;
	 bool higher;
	 int leftParen = '('; // made int

	 // push a left paren onto the stack and add a right paren to infix
	 intStack.push( leftParen );
	 strcat( infix, ")" );

	 // convert the infix expression to postfix
	 for ( contadorInfix = 0, postfixCount = 0; intStack.stackTop();
	 ++contadorInfix ) {

		 if ( isalnum( infix[ contadorInfix ] ) ){
		 postfix[ postfixCount++ ] = infix[ contadorInfix ];
		 }
		 else if ( infix[ contadorInfix ] == '(' ){
		 intStack.push( leftParen );
		 }
		 else if ( esOperador( infix[ contadorInfix ] ) ) {
			 higher = true; // used to store value of precedencia test
			 while ( higher ) {
				 if ( esOperador( static_cast< char >( intStack.stackTop() ) ) )
				 if ( precedencia( static_cast< char >( intStack.stackTop() ),
				 infix[ contadorInfix ] ) )

				postfix[ postfixCount++ ] =
				static_cast< char >( intStack.pop() );
				 else
				 higher = false;
				 else
				 higher = false;
			 }
			 // See chapter 21 for a discussion of reinterpret_cast
			 intStack.push( reinterpret_cast< int & > ( infix[ contadorInfix ] ) );
		}
		 else if ( infix[ contadorInfix ] == ')' )
		 while ( ( popValue = intStack.pop() ) != '(' )
		 postfix[ postfixCount++ ] = static_cast< char >( popValue );
	 }
	 postfix[ postfixCount ] = '\0';
	 evaluadorPostfix( postfix, contadorDataPtr, contadorInstruccionesPtr,
	 listaMaquina, getsVariable, tablaSimbolos );
 }


 // check if c is an operator
 bool CompiladorSIBO::esOperador( char c ){
	 if ( c == '+' || c == '-' || c == '*' || c == '/' || c == '^' )
	 return true;
	 else
	 return false;
 }

 // If the precedencia of operator1 is >= operator2,
bool CompiladorSIBO::precedencia( char operator1, char operator2 ){
	 if ( operator1 == '^' )
	 return true;
	 else if ( operator2 == '^' )
	 return false;
	 else if ( operator1 == '*' || operator1 == '/' )
	 return true;
	 else if ( operator1 == '+' || operator1 == '-' )
	 if ( operator2 == '*' || operator2 == '/' )
	 return false;
	 else
	 return true;
	 return false;
 }

 // evaluate postfix expression and produce code
void CompiladorSIBO::evaluadorPostfix( char *expr, int *contadorDataPtr,int *contadorInstruccionesPtr, int listaMaquina[],int getsVariable, entradaTabla tablaSimbolos[] ){
		 Stack< int > intStack;
		 int popRightValue, popLeftValue, accumResult, symbolLocation, symbol;
		 char type, array[ 2 ] = "";
		 int i;

		 strcat( expr, ")" );
	for ( i = 0; expr[ i ] != ')'; ++i ){
		if ( isdigit( expr[ i ] ) ) {
			 type = 'C';
			 array[ 0 ] = expr[ i ];
			 symbol = atoi( array );

			 symbolLocation = revisarSymTabla( tablaSimbolos, symbol, type );
			 intStack.push( symbolLocation );
	     }
		 else if ( isalpha( expr[ i ] ) ) {
		 type = 'V';
		 symbol = expr[ i ];
		 symbolLocation = revisarSymTabla( tablaSimbolos, symbol, type );
		 intStack.push( symbolLocation );
		 }
		 else {
		 popRightValue = intStack.pop();
		 popLeftValue = intStack.pop();
		 accumResult = crearInputSBML( popRightValue, popLeftValue, listaMaquina,
		 contadorInstruccionesPtr, contadorDataPtr,
		 expr[ i ] );
		 intStack.push( accumResult );
		 }

		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += intStack.pop();
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = STORE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += getsVariable;
		 ++( *contadorInstruccionesPtr );
	
	}
 }

int CompiladorSIBO::crearInputSBML( int right, int left, int listaMaquina[],int *contadorInstruccionesPtr, int *contadorDataPtr, char oper ){
	 int location;

	 switch( oper ) {
		 case '+':
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += left;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = ADD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += right;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = STORE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += *contadorDataPtr;
		 location = *contadorDataPtr;
		 --( *contadorDataPtr );
		 ++( *contadorInstruccionesPtr );
		 return location;
		 case '-':
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += left;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = SUBTRACT * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += right;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = STORE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += *contadorDataPtr;
		 location = *contadorDataPtr;
		 --( *contadorDataPtr );
		 ++( *contadorInstruccionesPtr );
		 return location;
		 case '/':
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += left;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = DIVIDE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += right;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = STORE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += *contadorDataPtr;
		 location = *contadorDataPtr;
		 --( *contadorDataPtr );
		 ++( *contadorInstruccionesPtr );
		 return location;
		 case '*':
		 listaMaquina[ *contadorInstruccionesPtr ] = LOAD * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += left;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = MULTIPLY * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += right;
		 ++( *contadorInstruccionesPtr );
		 listaMaquina[ *contadorInstruccionesPtr ] = STORE * 100;
		 listaMaquina[ *contadorInstruccionesPtr ] += *contadorDataPtr;
		 location = *contadorDataPtr;
		 --( *contadorDataPtr );
		 ++( *contadorInstruccionesPtr );
		 return location;
		 default:
		 cerr << "ERROR: operator not recognized.\n";
		 break;
	 }

	 return 0; // default return
 }







