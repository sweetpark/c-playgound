#include <string.h>
#include <stdio.h>

void addscalar(int n, int m, double a[n][n*m+300], double x);

void my_memset(size_t n; char p[n], size_t n, char v)
{
	  memset(p, v, n);
} 

void my_memset2(size_t n, char p[n], char v)
{
	  memset(p, v, n);
	  printf("p size:%d\n", sizeof(p)) ;
} 

int main(void)
{
	double b[4][308];
	addscalar(4, 2, b, 2.17);

	char a[10] ;

	my_memset(a, 10, 'a') ;

	my_memset2( 10, a, 'b') ;

	return 0;
}

void addscalar(int n, int m, double a[n][n*m+300], double x)
{
	int i, j, k ;
	for (i = 0; i < n; i++) {
		for (j = 0, k = n*m+300; j < k; j++) {
			a[i][j] += x;
		}
	}
}

