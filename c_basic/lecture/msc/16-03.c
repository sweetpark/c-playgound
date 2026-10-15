#include <stdio.h>

int foo() {
	static int i = 0 ;

	return ++i ;
}

int bar() {
	static int i = 100 ;

	return ++i ;
}

int baz() {
	static int i = 0 ;

	return i % 2 ;
}

int g() {
	int p1, p2 ;

	p1 = foo();
	p2 = bar();
	 
	if (baz()) {
		  return p1;
	}
	else {
		  p2 = p1;
	}

	return p2;
}

int main(void) {
	printf("g()=%d\n", g() ) ;
	printf("g()=%d\n", g() ) ;
	printf("g()=%d\n", g() ) ;
	printf("g()=%d\n", g() ) ;

	return 0 ;
}
