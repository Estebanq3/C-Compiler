#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;
#include "stacknd.h"

/*Clase stack:
Las pilas, las cuales son un tipo de contenedor son usadas por el compilador para ayudar al proceso de evaluar las expresiones y generar lenguaje maquina. Los compiladores
evalauan las expresiones aritmeticas con ayuda de las pilas, evaluando constantes, operadores y parentesis presentes. De esta forma el proceso de infijo a postfijo se puede procesar, para 
que las computadoras puedan obtener una expresion clara o entendible para ellas. Por lo tanto si la computadora desea evaluar una expresion infija, primero debe pasarla a posfija
para posterirormente evaluar la version postifja de la expresion infija. En la pasada de izquierda a derecha los contenedores participan activamente en el algoritmo, cada proceso utiliza
un objeto stack para apoyarse de acuerdo a la operacion que realiza, asimismo en cada algoritmo las pilas se utilizan de distintas formas. Por ejemplo, para las operaciones las 
pilas siguen un algortimo diferente a cuando simplemente se evaluan constantes, ya que cuando son constantes, estas son enviadas directamente a la expresion postfija, sin tener que pasar ningun
proceso muy elaborado en la pila. Las operaciones como suma, resta... tienen un proceso mas rigurosos en la pila para definir su orden de acuerdo a su precedencia, metodo
utilizado en la clase compilador*/

/*En esta clase creamos el .h de la pila que sera utilizada en el compilador para los procesos descritos anteriormente, esta consta de un constructor por omision, un destructor, y 
metodos respectivos a una pila para poder realizar operaciones especificas como el push o el pop. */

template < class T >
class Stack {
	public:
		Stack(); // constructor por defecto
		~Stack(); // destructor
		void push( T & ); // insertar item en la pila
		T pop(); // remover item de la pila
		bool isEmpty() const; // verificar que la pila está vacía
		T stackTop() const; // retornar el tope de la pila
		void print() const; // output del stack
		
	private:
		StackNode< T > *topPtr; // puntero al primer stacknode
};

// funciones miembro
template < class T >
Stack< T >::Stack() { 
	topPtr = 0; 
}

template < class T >
Stack< T >::~Stack(){
	 StackNode< T > *tempPtr, *currentPtr = topPtr;

	 while ( currentPtr != 0 ) {
	 tempPtr = currentPtr;
	 currentPtr = currentPtr -> nextPtr;
	 delete tempPtr;
	 }
 }


 template < class T >
 void Stack< T >::push( T &d ){
	 StackNode< T > *newPtr = new StackNode< T >( d, topPtr );
	 topPtr = newPtr;
 }

 template < class T >
 T Stack< T >::pop(){
	 StackNode< T > *tempPtr = topPtr;

	 topPtr = topPtr -> nextPtr;
	 T poppedValue = tempPtr -> data;
	 delete tempPtr;
	 return poppedValue;
 }

 template < class T >
 bool Stack< T >::isEmpty() const {
  	return topPtr == 0; 
}

 template < class T >
 T Stack< T >::stackTop() const{ 
 	return !isEmpty() ? topPtr -> data : 0; 
 }

 template < class T >
 void Stack< T >::print() const{
	 StackNode< T > *currentPtr = topPtr;

	 if ( isEmpty() ){ // Stack is empty
	 	cout << "PILA VACIA\n";
	}
	 else { // Stack is not empty
	 	cout << "La pila es:\n";
	 }
	 while ( currentPtr != 0 ) {
		cout << currentPtr -> data << ' ';
		currentPtr = currentPtr -> nextPtr;
	}
	cout << '\n';
}
#endif