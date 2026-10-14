#ifndef _14_04_H_
#define _14_04_H_

#pragma pack(push, 1)

typedef enum _test_type_e
{
	TEST_TYPE_NONE = 0x00,
	TEST_TYPE_PRT = 0x01,
	TEST_TYPE_FILE = 0x02,
	TEST_TYPE_END = 0x03,
	TEST_TYPE_MAX
} test_type_e ;

typedef struct _test_header_s
{
	unsigned char		type ;
	unsigned int		len ;
} test_header_t ;

#define TEST_BODY_MAX_LEN	1024
typedef struct _test_msg_s
{
	test_header_t		t_hd ;
	unsigned char		body[ TEST_BODY_MAX_LEN ] ;
} test_msg_t ;

#pragma pack(pop)

#endif /* _14_04_H_ */
