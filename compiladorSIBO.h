#ifndef _COMPILADOR
#define _COMPILADOR

using namespace std;
 #include <fstream>

/* El compilador comienza leyendo una declaracion del programa simple en la memoria, esta linea debe de estar separada en "tokens" (los cuales son piezas de una declaracion)
para procesarlos y compilarlos (la funcion strtok permitida, es usada para facilitar la tarea de divison de declaraciones). Cada declaracion comienza con un numero de linea. 
Cuando el compilador fragmenta la declaracion en fragmentos "tokens", si el token es un numero de linea, una variable("x") o una constante('C'), este es colocado en la tabla de simbolos.
Un numero de linea es colocado en la tabla de simbolos si y solo si el primer token es una declaracion. La tabla de simbolos(objeto de entrada tabla) es una lista de tipo array la cual pertenece a la entrada de la tabla, 
la cual representa cada simbolo en el programa. No hay restriccion en el numero de simbolos que pueden aparecer en el programa. Cada entrada de la tabla contiene tres miembros, un simbolo que contiene
la representacion de una variable, un numero de linea que es una constante. MemberType indica el tipo del simbolo, ya sea 'C', 'V' o 'L'. Member location referente de memoria de SIBO. Un hecho importante
era que las variables del programa, se colocaban siempre al final de la memoria de SIBO. La tabla de simbolos cumple con una funcion escencial en la conversion de programas simples a programas.sbml,
la instruccion del lenguaje simple esta compuesto por 4 elementos que se divide en dos partes, el codigo operaciones y el operando. El codigo de operacion esta determinado por los comandos
en el lenguaje simple. El codigo operando es una localizacion de memoria que contiene la informacion, la cual determina la forma en la cual el codigo operador va a ejecutar la instruccion.
El compilador va a buscar en la tabla de simbolos para determinar la localizacion de la memoria SIBO para cada simbolo que corresponde a una localizacion que puede ser usada para completar
las instrucciones sbml. El compilador de cada declaracion del lenguaje simple esta basada en un comando. Cuando un jump es compilado tiene una referencia no resuelta, la instruiccion sbml debe ser "marcada"
para indicar que en la segunda pasada del compilador se debe completar la instruccion. Las banderas son guardadas en un array de cierto numero de elementos similar al de la memoria de SIBO,  donde cada elemento 
es inicializado con "-1". Si la ubicacion de la memoria a un numero de linea en el programa simple se refiere a algo que todavia no es conocido. El numero de linea es guardado en array de banderas
en el elemento con la misma subscripcion que la instruccion incompleta. El operando de la instruccion incompleta se setea a 00 temporalmente. La compilacion del if/jump y el  set son mas complicadas
que otras declaraciones , son practicamente los unicos que producen mas de una instruccion de sbml ademas del for. Para el if/jump el compilador produce un codigo para probar la condicion y luego ramificarse
a otra linea si es necesario. El resultado de la ramificacion puede ser una referencia no resuelta. Para un set, el compilador produce un codigo para evaluar la arbitrariedad aritmetica compleja de 
una expresion. Las expresiones deben separar cada operando y operador con espacios. Es decir se utiliza el conversor de infijo a postfijo para posteriormente evaluar el postfijo. Para que el compilador genere
el .sbml: el evaluador contiene un gancho que  donde el evaluador puede generar instrucciones sbml antes que evaluar la expresion. Para habilitar este gancho para el compilador, el evaluador postfijo
debe ser modificado para buscar en la tabla de simbolo para cada simbolo que el encuentra. Determinar el simbolo correspondiente a la localizacion de memoria y pushear la localizacion de memoria dentro
del stack(en lugar del simbolo). Cuando un operador es encontrado en la expresion postfija , las pila es popeada dos veces  y el lenguaje maquina es producido usando las localizaciones de memoria y operandos.
El resultado de cada subexpresion es guardado temporalmente en la memoria y luego se pushea a la pila para la evaluacion del posfijo para que la expresion continue.  Cuando una evaluacion postfija
ha teminado completamente, la localizacion de la memoria que contiene el resultado es la unica localizacion que queda en el stack, esta es poepeada y las instrucciones sbml son generadas para asignarle el resultado
a la variable de la izquierda de input statement.
El segundo paso del compilador reesulve dos  tareas: resolver cada referencia no resuelta y hacer un ouput del sbml a un archivo. El primer paso de la segunda pasada
es buscar en el vector de banderas por una referencia que no haya sido resuelta(son los que contienen -1). Se localiza el objeto en la tabla de simbolos que contiene el simbolo almacenado en el array de banderas. Hay que asegurarnos que sea tipo 'L'(numero de linea)
Posterirmente, se inserta la ubicación de la memoria desde la ubicación del miembro en la instrucción con la referencia no resuelta (recordando que una instrucción que contiene una referencia no resuelta tiene el operando 00).
De esta forma repetimos el paso 1,2,3 hasta llegar al final del vector de banderas. 
Una vez que se completa el proceso de resolución, toda la matriz que contiene el código sbml se envía a un archivo de disco con una instrucción sbml por línea. 
SIBO puede leer este archivo para su ejecución (después de modificar el simulador para leer su entrada de un archivo).
 */

class CompiladorSIBO{
const int MAXIMO = 81; // longitud de líneas de código
const int TAMANOTABLASIMBOLO = 4095;// maximo tamaño de la tabla de símbolos
const int MEMORIA = 4095; //memoria de SIBOL

// estructura de la tabla de símbolos
struct entradaTabla {
public:
	int location; // SIBOL memory location 000-999
	char type; // 'C' = constante, 'V' = variable,
	int symbol; // or 'L' = line number
};

public:
 //funciones del compilador
 int revisarSymTabla( entradaTabla *, int, char ); //revisa la tabla de simbolos 
 int revisarOperando( entradaTabla *, char *, int *, int *, int * );
 void anadirASimbolosTabla( char, int, int, entradaTabla *, int );
 void anadirLineaABanderas( int, int, int *, int *, const int * );
 void compilar( ifstream &, char * );
 void printOutput( const int [], char * );
 void numeroDeLinea( char *, entradaTabla *, int, int );
 void inicializarListas( int *, int *, entradaTabla * );

 void primeraPasada( int *, int *, entradaTabla *, ifstream & );
 void segundaPasada( int *, int *, entradaTabla * );
 void separarToken( char *, int *, int *, entradaTabla *, int *, int * );
 void palabraClave( char *, int *, int *, entradaTabla *, int *, int * );

 void set( char *, int *, entradaTabla *, int *, int * );
 void read( char *, int *, entradaTabla *, int *, int * );
 void print( char *, int *, entradaTabla *, int *, int * );
 void jump( char *, int *, int *, entradaTabla *, int * );
 void jumpCondicional( char *, int *, int *, entradaTabla *, int *, int * );
 void evaluarExpresion( int, int, char *, int *, entradaTabla *,int *, int *, char * );
 int crearInputSBML( int, int, int *, int *, int *, char );
 void infixToPostfix( char *, char *, int, entradaTabla *, int *, int *, int * );
 void evaluadorPostfix( char *, int *, int *, int *, int, entradaTabla * );
 bool esOperador( char );
 bool precedencia( char, char );

 // SIBO Machine Language (SBML) Operation Codes
 enum SIBOLCodigoOperaciones { READ = 2, WRITE = 4, READS=3, WRITES=5,MODULO= 'A', LOAD = 0, STORE = 1, ADD = 6,
 SUBTRACT = 7, DIVIDE = 9, MULTIPLY = 8, BRANCH = 'C',
 BRANCHNEG = 'D', BRANCHZERO = 'C', HALT = 'F' };

};
 #endif
