#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct gadget gadget;
struct gadget {
	int i;
	double d;
};

typedef struct widget widget;
struct widget {
	char c[20];
	int i;
	double d;
};

int main(void)
{
	gadget *p;
	//widget *p;
	p = malloc(sizeof(gadget)); 
	if (p != NULL) {
		p->i = 0;
		p->d = 0.0;
	}

	printf("p address=%p (size:%zu), p->i addresss: %p\n", p, sizeof(gadget), &p->i) ;

	memset( p, 0x00, sizeof(*p) ) ;
	return 0;
} 
