#include <stdio.h>

size_t getlen(const int *input, size_t maxlen, int delim) {
	size_t i ;
	for (i = 0; i < maxlen; ++i) {
		if (input[i] == delim) {
			return i;
		}
		/* if문 이외 로직일때 return 이 없어서 안전성이 떨어짐 */
	}
}

int data[] = { 1, 2, 5 };
void func(int userdata, int delim) {
	size_t i;
	/* sizeof 도 3이 아닌 12가 나옴 ->  / sizeof(int) 필요 */
	i = getlen(data, sizeof(data), delim);
	data[i] = userdata;
}

int main(void)
{
	func(100, 2) ;

	size_t n ;

	for( n = 0; n < 3; n++ ) 
		printf("data[%zu] = %d\n", n, data[n]) ;

	return 0 ;
}

