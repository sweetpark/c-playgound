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

	struct sockaddr_in t_addr ;
	memset( &t_addr, 0x00, sizeof(t_addr) ) ;

	t_addr.sin_family = AF_INET ;
	t_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" ) ;
	t_addr.sin_port = htons( 51000 ) ;

	rc = bind( fd, (struct sockaddr *)&t_addr, sizeof(t_addr) ) ;
	if( rc < 0 ) 
	{
		printf("bind fail <%d:%s>\n", errno, strerror(errno)) ;
		close(fd) ;
		return -4 ;
	}

	rc = listen( fd, 5 ) ;
	if( rc < 0 )
	{
		printf("listen fail <%d:%s>\n", errno, strerror(errno));
		close(fd) ;
		return -5 ;
	}

	*pn_fd = fd ;
	return 1 ;
}


int accept_sock( int sock_fd, int *pn_newfd )
{
	int rc = 0 ;
	int fd = -1 ;

	if( pn_newfd == NULL )
	{
		printf("Invalid pn_newfd address\n") ;
		return -1 ;
	}

	if( sock_fd < 0 )
	{
		printf("Invalid sock_fd(%d)\n", sock_fd) ;
		return -1 ;
	}

	printf("Wait Connecting\n") ;
	fd = accept( sock_fd, NULL, NULL ) ;
	if( fd < 0 )
	{
		printf("accept fail <%d:%s>\n", errno, strerror(errno)) ;
		return -2 ;
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

	*pn_newfd = fd ;

	return 1 ;
}

int wait_msg( int fd )
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

		rc = select( fd+1, &t_fdtmp, 0, &t_fderr, NULL ) ;
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

int read_sock( int fd, unsigned char *pbuf, unsigned int len )
{
	int retry_cnt ;
	int rc ;
	unsigned int read_len = 0 ;

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

#define TEST_READ_RETRY_CNT		10
	retry_cnt = 0 ;
	while(retry_cnt < TEST_READ_RETRY_CNT )
	{
		rc = read(fd, (void *)(pbuf+read_len), len-read_len);
		if( rc < 0 )
		{
			if( ( errno == EWOULDBLOCK ) || ( errno == EAGAIN ) || ( errno == EINTR ) )
			{
				retry_cnt++ ;
				continue ;
			}
			else 
			{
				printf("read error=%d(%s)\n",errno, strerror(errno)) ;
				return -2 ;
			}
		}
		else if( rc == 0 )
		{
			printf("read zero(%d peer close)\n", fd) ;
			return -3 ;
		}

		read_len += rc ;

		if( read_len >= len )
		{
			return 1 ;
		}

	}

	return -4 ;
}

int recv_msg( int fd, test_msg_t *pt_msg )
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

	rc = wait_msg( fd ) ;
	if( rc < 0 )
	{
		printf("wait_msg fail, rc=%d\n", rc ) ;
		return -2 ;
	}
	printf("Arrive Msg\n") ;

	rc = read_sock( fd, (unsigned char *)&pt_msg->t_hd, sizeof(pt_msg->t_hd) ) ;
	if( rc < 0 )
	{
		printf("read_sock(Header) fail, rc=%d\n", rc) ;
		return -3 ;
	}

	pt_msg->t_hd.len = ntohl(pt_msg->t_hd.len) ;

	if( pt_msg->t_hd.len > TEST_BODY_MAX_LEN )
	{
		printf("invalid len(%u)\n", pt_msg->t_hd.len ) ;
		return -4 ;
	}

	if( pt_msg->t_hd.len > 0 )
	{
		rc = read_sock( fd, pt_msg->body, pt_msg->t_hd.len) ;
		if( rc < 0 )
		{
			printf("read_sock(Body:%d) fail, rc=%d\n", pt_msg->t_hd.len, rc ) ;
			return -5 ;
		}
	}

	return 1 ;
}

void proc_serv(int fd)
{
	int rc ;
	test_msg_t t_msg ;

	if( fd < 0 )
	{
		printf("invalid fd(%d)\n", fd) ;
		return ;
	}

	while(1)
	{
		memset( &t_msg, 0x00, sizeof(t_msg) ) ;	

		rc = recv_msg(fd, &t_msg ) ;
		if( rc < 0 )
		{
			printf("recv_msg fail\n") ;
			break ;
		}

		if( t_msg.t_hd.type == TEST_TYPE_PRT )
		{
			printf("Recv :%s\n", t_msg.body ) ;
		}
		else if( t_msg.t_hd.type == TEST_TYPE_FILE )
		{
			printf("type(TEST_TYPE_FILE)) is developing\n") ;
		}
		else if( t_msg.t_hd.type == TEST_TYPE_END )
		{
			printf("Recv END MSG\n") ;
			break ;
		}
		else
		{
			printf("Unknown type:%d\n", t_msg.t_hd.type ) ;
		}
	}
}

int main(void)
{
	int listen_fd = -1 ;
	int acc_fd = -1 ;
	int rc = -1 ;

	rc = init_sock( &listen_fd ) ;
	if( rc < 0 )
	{
		printf("init_sock fail,rc=%d\n", rc) ;
		return -1 ;
	}

	rc = accept_sock( listen_fd, &acc_fd ) ;
	if( rc < 0 )
	{
		printf("accept_sock fail, rc=%d\n", rc) ;
		close( listen_fd ) ;
		return -2 ;
	}

	printf("Connect\n") ;

	proc_serv( acc_fd ) ;

	close( acc_fd ) ;

	close(listen_fd) ;

	return 0 ;
}
