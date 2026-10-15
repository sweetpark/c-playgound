#include <stdio.h>

typedef enum { Red, Green, Blue } Color;
const char* f(Color c) {
	switch (c) {
		case Red: return "Red";
		case Green: return "Green";
		case Blue: return "Blue";
	}
}

void g() {
	size_t i ;

	for( i = Red; i <= Blue; i++ )
		puts(f(i)) ;
}

int main(void)
{
	g() ;
}
