#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tm *make_tm(int year, int mon, int day, int hour, int min, int sec) {
	struct tm *tmb;
	tmb = (struct tm *)malloc(sizeof(tmb));
	if (tmb == NULL) {
		return NULL;
	}

	*tmb = (struct tm) {
		.tm_sec = sec, .tm_min = min, .tm_hour = hour,
		.tm_mday = day, .tm_mon = mon, .tm_year = year
	};

	return tmb;
}

void free_tm( struct tm *tmb) {
	if( tmb != NULL ) {
		free(tmb) ;
	}
}

void prt_tm( struct tm *tmb ) {
	if( tmb == NULL ) return ;

	printf("%04d/%02d/%02d %02d:%02d:%02d\n", tmb->tm_year, tmb->tm_mon, tmb->tm_mday, tmb->tm_hour, tmb->tm_min, tmb->tm_sec ) ;
}

int main(void) {
	struct tm *tmb ;

	tmb = make_tm( 2023, 7, 10, 14, 52, 37 ) ;
	if( tmb == NULL ) {
		return 1 ;
	}

	prt_tm(tmb) ;

	free_tm(tmb) ;

	return 0 ;
}
