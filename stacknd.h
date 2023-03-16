#ifndef STACKND_H
#define STACKND_H
#include <iostream>

/*Definicion de la clase stackNode, la cual viene siendo el elemento o la celda de cada pila, por lo tanto esta clase
trabaja directamente en colaboracion con la clase stack, la cual es la pila, que resulta ser un contenedor utilizado para
la transformacion de expresiones infijas a postfijas y luego evaluarlas, esto con el fin de servir como clases de ayuda para
el compilador de lenguaje simple para transformarlo en lenguaje capacitado para ser ejecutado en SIBO. */

template < class T > class Stack;

 template < class T >
 class StackNode {
	 friend class Stack< T >;

	 public:
	 StackNode( const T & = 0, StackNode * = 0 );
	 T getData() const;
	 private:
	 T data;
	 StackNode *nextPtr;
 };

 template < class T >
 StackNode< T >::StackNode( const T &d, StackNode< T > *ptr )
 {
	 data = d;
	 nextPtr = ptr;
 }

 template < class T >
 T StackNode< T >::getData() const { 
 	return data; 
 }

 #endif
