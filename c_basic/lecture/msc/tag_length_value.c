#include<stdio.h>
#include<stdint.h>
#include<string.h>

enum { HDR_LEN = 8 };

typedef enum {
	ST_IDLE,
	ST_ACTIVE,
	ST_SUSPEND,
	ST_CLOSED
} stat_t;


const char *state_name(stat_t st){
	/* default 미존재 */
	switch(st){
		case ST_IDLE:
			return "IDLE";
		case ST_ACTIVE:
			return "ACTIVE";
		case ST_SUSPEND:
			return "SUSPEND";
		default:
			return "UNKNOWN";
	}
}

int find_tag(const uint8_t *buf, size_t len, uint8_t tag){
	size_t i;
	
	/* 제한값 필요  */
	for(i = HDR_LEN; i != len || i > len; i += 2){
		if(buf[i] == tag)
			return buf[i + 1];
	}
}

int parse(const uint8_t *buf, size_t len, int *rx_count){
	size_t body_len = len - HDR_LEN;
	uint32_t seq;
	int value = 0;
	state_t st;

	if(body_len < 0)
		return 1;

	st = (state_t)buf[0];
	memcpy(&seq, buf + 4, sizeof(seq));
	value = find_tag(buf,len,0x10);
	printf("state = %s, seq = %u, value = %d\n", state_name(st), seq, value);
	/* 연산자 우선순위로 인해 문제  */
	(*rx_count)++;
	return 0;
}


int main(void)
{
	const uint8_t msg[] = {1, 0, 0, 0, 0, 0, 0, 7, 0x10, 5, 0x20, 9};

	int rx_count = 0;

	parse(msg, sizeof(msg), &rx_count);
	printf("rx_count=%d\n", rx_count);
	return 0;
}


