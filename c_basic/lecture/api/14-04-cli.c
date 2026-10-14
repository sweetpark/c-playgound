#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "14-04.h"

int init_sock(int *pn_fd )
{
	int fd = -1;
	int rc = 0 ;
	int opt = 1 ;

	if( pn_fd == NULL )
	{
		printf("invalid pn_fd address\n") ;
		return -1 ;
	}

	fd = socket( AF_INET, SOCK_STREAM, 0 ) ;
	if( fd < 0 )
	{
		printf("socket fail<%d:%s>\n", errno, strerror(errno) ) ;
		return -2 ;
	}

	rc = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int) ) ;
	if( rc < 0 )
	{
		printf("setsockopt fail<%d:%s>\n", errno, strerror(errno) ) ;
		return -3 ;
	}

	int flag ;
	flag = fcntl( fd, F_GETFL, 0 );
	if( flag < 0 )
	{
		printf("fcntl(F_GETFL) fail <%d:%s>\n", errno, strerror(errno) ) ;
		return -3 ;
	}

	rc = fcntl( fd, F_SETFL, flag | O_NONBLOCK ) ;
	if( rc < 0 ) 
	{
		printf("fcntl(F_SETFL) fail <%d:%s>\n", errno, strerror(errno) ) ;
		return -4 ;
	}

	*pn_fd = fd ;
	return 1 ;
}


int connect_sock( int sock_fd )
{
	int rc = 0 ;
	struct sockaddr_in t_addr ;
	int cnt = 0 ;

	if( sock_fd < 0 )
	{
		printf("Invalid sock_fd(%d)\n", sock_fd) ;
		return -1 ;
	}

	t_addr.sin_family = AF_INET ;
	t_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" ) ;
	t_addr.sin_port = htons( 51000 ) ;

	while(cnt < 5)
	{
		rc = connect( sock_fd, (struct sockaddr *)&t_addr, sizeof( t_addr ) );
		if( rc < 0 )
		{
			if( errno != EINPROGRESS )
			{
				printf( "connect fail <%d,%s>", errno,strerror(errno) );

				return -2;
			}
			else
			{
				cnt++ ;
				sleep(1) ;
				continue ;
			}
		}

		break ;
	}

	printf("Connect to Server\n") ;

	return 1 ;
}

int check_writeable( int fd )
{
	fd_set t_fdset, t_fdtmp, t_fderr;
	int rc ;

	if( fd < 0 )
	{
		printf("invalid fd(%d)\n", fd) ;
		return -1 ;
	}

	FD_ZERO(&t_fdset) ;
	FD_SET(fd, &t_fdset) ;

	while(1)
	{
		t_fdtmp = t_fdset ;
		t_fderr = t_fdset ;

		rc = select( fd+1, 0, &t_fdtmp, &t_fderr, NULL ) ;
		if( rc < 0 )
		{
			if( errno == EAGAIN )
			{
				continue ;
			}
			else
			{
				printf("select error=%d(%s)\n",errno, strerror(errno)) ;
				return -2 ;
			}
		}

		if( FD_ISSET( fd, &t_fdtmp ) )
		{
			break ;
		}
		else if( FD_ISSET( fd, &t_fderr ) )
		{
			printf("fd(%d) error\n", fd) ;
			return -3 ;
		}
	}

	return 1 ;
}

int write_sock( int fd, unsigned char *pbuf, unsigned int len )
{
	int retry_cnt ;
	int rc ;
	unsigned int write_len = 0 ;

	if( pbuf == NULL )
	{
		printf("invalid pbuf\n") ;
		return -1 ;
	}

	if( fd < 0 )
	{
		printf("invalid fd(%d)\n", fd) ;
		return -1 ;
	}

	if( len == 0 )
	{
		return 1 ;
	}

#define TEST_WRITE_RETRY_CNT		10
	retry_cnt = 0 ;
	while(retry_cnt < TEST_WRITE_RETRY_CNT )
	{
		rc = write(fd, (void *)(pbuf+write_len), len-write_len);
		if( rc < 0 )
		{
			if( ( errno == EWOULDBLOCK ) || ( errno == EAGAIN ) || ( errno == EINTR ) )
			{
				retry_cnt++ ;
				continue ;
			}
			else {
				printf("write error=%d(%s)\n",errno, strerror(errno)) ;
				return -2 ;
			}
		}

		write_len += rc ;

		if( write_len >= len )
		{
			return 1 ;
		}

	}

	return -4 ;
}

int send_msg( int fd, test_msg_t *pt_msg )
{
	int rc ;

	if( pt_msg == NULL )
	{
		printf("invalid pt_msg\n") ;
		return -1 ;
	}

	if( fd < 0 )
	{
		printf("Invalid fd(%d)\n", fd ) ;
		return -1 ;
	}

	if( pt_msg->t_hd.len > TEST_BODY_MAX_LEN )
	{
		printf("invalid len(%u)\n", pt_msg->t_hd.len ) ;
		return -1 ;
	}

	rc = check_writeable( fd ) ;
	if( rc < 0 )
	{
		printf("check_writeable fail, rc=%d\n", rc ) ;
		return -2 ;
	}

	int len = pt_msg->t_hd.len ;

	pt_msg->t_hd.len = htonl(pt_msg->t_hd.len) ;

	rc = write_sock( fd, (unsigned char *)&pt_msg->t_hd, sizeof(pt_msg->t_hd) ) ;
	if( rc < 0 )
	{
		printf("write_sock(Header) fail, rc=%d\n", rc) ;
		return -3 ;
	}

	printf("Send Header[%d:%d]\n", pt_msg->t_hd.type, len) ;
	if( len > 0 )
	{
		printf("Body:%s\n", pt_msg->body) ;
		rc = write_sock( fd, pt_msg->body, len) ;
		if( rc < 0 )
		{
			printf("write_sock(Body:%d) fail, rc=%d\n", len, rc ) ;
			return -5 ;
		}
		printf("Send Body\n") ;
	}

	return 1 ;
}

int proc_cli(int fd, char *p_msg)
{
	int rc = 1 ;
	test_msg_t t_msg ;

	if( fd < 0 )
	{
		printf("invalid fd(%d)\n", fd) ;
		return -1;
	}

	if( p_msg == NULL )
	{
		printf("invalid p_msg\n") ;
		return -1;
	}
	else if( strlen(p_msg) >= TEST_BODY_MAX_LEN )
	{
		printf("invalid msg length(%d)\n", strlen(p_msg) ) ;
		return -1;
	}

	memset( &t_msg, 0x00, sizeof(t_msg) ) ;	

	t_msg.t_hd.type = TEST_TYPE_PRT ;
	t_msg.t_hd.len = strlen(p_msg)+1 ;
	memcpy(t_msg.body, p_msg, t_msg.t_hd.len) ;

	rc = send_msg(fd, &t_msg ) ;
	if( rc < 0 )
	{
		printf("send_msg fail,rc=%d\n",rc) ;
		return -2 ;
	}

	printf("MSG Send success\n") ;

	return 1 ;
}

int proc_end(int fd)
{
	int rc = 1 ;
	test_msg_t t_msg;

	if( fd < 0 )
	{
		printf("invalid fd(%d)\n", fd) ;
		return -1;
	}

	memset( &t_msg, 0x00, sizeof(t_msg) ) ;	

	t_msg.t_hd.type = TEST_TYPE_END ;
	t_msg.t_hd.len = 0 ;

	rc = send_msg(fd, &t_msg ) ;
	if( rc < 0 )
	{
		printf("send_msg fail,rc=%d\n",rc) ;
		return -2 ;
	}

	printf("END MSG Send success\n") ;

	return 1 ;
}

int main(void)
{
	int rc = -1 ;
	int fd = -1 ;

	char msg[] = "------------------------------------------------------------------\n"
		"						say goodbye                                  \n"
		"						say goodbye                                  \n"
		"						say goodbye                                  \n"
		"						say goodbye                                  \n"
		"						say goodbye                                  \n"
		"						say goodbye                                  \n"
		"--------------------------------------------------------------------\n";
	rc = init_sock( &fd ) ;
	if( rc < 0 )
	{
		printf("init_sock fail,rc=%d\n", rc) ;
		goto end ;
	}

	rc = connect_sock( fd ) ;
	if( rc < 0 )
	{
		printf("connect_sock fail,rc=%d\n",rc) ;
		goto end; 
	}

	rc = proc_cli(fd, "Hi Hello\n") ; 
	if( rc < 0 )
	{
		printf("proc_cli fail,rc=%d\n", rc) ;
		goto end;
	}

	rc = proc_cli(fd, msg) ; 
	if( rc < 0 )
	{
		printf("proc_cli fail,rc=%d\n", rc) ;
		goto end;
	}

	rc = 0 ;

	rc = proc_end(fd) ;
	if( rc < 0 )
	{
		printf("proc_end fail, rc=%d\n", rc) ;
		goto end ;
	}

end :
	if( fd >= 0 ) 
	{
		close(fd) ;
	}

	return rc ;
}
