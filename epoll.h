#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/epoll.h>

#define LISTEN_PORT (12345) ///< 待ち受けポート
#define MAX_EVENTS (1000) ///< 最大イベント数
#define MAX_BACKLOG (10) ///< リクエスト待機キューの格納数

struct epollev
{
   void (*callback)(int, void *); ///< callback function.
   void *arg; ///< user data.
};

struct epollop
{
   int epfd; ///< epoll fd.
   struct epoll_event *events; ///< epoll events.
   int num_events; ///< maximum number of events.
   struct epollev *fds; ///< fds
};
static epollop g_epollop;

static void callback_accept( int fd, void *arg );
static void callback_client( int fd, void *arg );

//! @brief    強制終了
static void server_exit( const char *log )
{
   perror( log );
   exit( EXIT_FAILURE );
}

//! @brief    イベント初期化
static void server_init()
{
   int epfd, nfiles = MAX_EVENTS;

   if( ( epfd = epoll_create( nfiles ) ) == -1 ){
      server_exit( "epoll_create()" );
   }
   g_epollop.epfd = epfd;
   g_epollop.events = new struct epoll_event[nfiles];
   memset( g_epollop.events, 0, sizeof(struct epoll_event)*nfiles );
   g_epollop.num_events = MAX_EVENTS;
   g_epollop.fds = new epollev[nfiles];
   memset( g_epollop.fds, 0, sizeof(struct epollev)*nfiles );
}

//! @brief    イベント追加
static void server_add( int fd, int events, void (*callback)(int, void *), void *arg )
{
   struct epoll_event epev = {0, {0}};
   struct epollev *ev = &g_epollop.fds[fd];

   int op = EPOLL_CTL_ADD;
   if( ev->callback != NULL ){
      op = EPOLL_CTL_MOD;
   }

   epev.data.fd = fd;
   epev.events = events;
   if( epoll_ctl( g_epollop.epfd, op, fd, &epev ) == -1 ){
      perror( "epoll_ctl()" );
      return;
   }
   ev->callback = callback;
   ev->arg = arg;
}

//! @brief    イベント削除
static void server_del( int fd )
{
   struct epoll_event epev = {0, {0}};
   struct epollev *ev = &g_epollop.fds[fd];
   int op;

   op = EPOLL_CTL_DEL;
   epev.data.fd = fd;
   memset( ev, 0, sizeof( struct epollev ) );

   if( epoll_ctl( g_epollop.epfd, op, fd, &epev ) == -1 ){
      perror( "epoll_ctl()" );
      return;
   }
   close( fd );
}

//! @brief    受付
static void server_listen()
{
   int sock;
   struct sockaddr_in saddr;

   if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ){
      server_exit( "socket()" );
   }

   memset( &saddr, 0, sizeof( saddr ) );
   saddr.sin_family    = AF_INET;
   saddr.sin_addr.s_addr = htonl( INADDR_ANY );
   saddr.sin_port = htons( LISTEN_PORT );

   if( bind( sock, (struct sockaddr *) &saddr, sizeof(saddr) ) != 0 ){
      close( sock );
      server_exit( "bind()" );
   }

   if( listen( sock, MAX_BACKLOG ) < 0 ){
      close( sock );
      server_exit( "listen()" );
   }

   server_add( sock, EPOLLIN, callback_accept, NULL );
}

//! @brief    メインループ
static int server_loop( )
{
   fprintf( stdout, "in server_loop.\n" );

   while( true ){
      int i, res;
      int timeout = -1;
      struct epoll_event *events = g_epollop.events;

      res = epoll_wait( g_epollop.epfd, events, g_epollop.num_events, timeout );
      if( res == -1 ){
         server_exit( "epoll_wait()" );
      }

      for( i = 0; i < res; ++i ){
         int fd = events[i].data.fd;
         struct epollev *ev = &g_epollop.fds[fd];

         ev->callback( fd, ev->arg );
      }
   }
   return 0;
}

//! @brief    受付コールバック
static void callback_accept( int fd, void *arg )
{
   fprintf( stdout, "in callback_accept. fd:%d\n", fd );

   int sock;
   struct sockaddr saddr;
   socklen_t len = sizeof( struct sockaddr_in );

   if( ( sock = accept( fd, (struct sockaddr *)&saddr, &len ) ) < 0 ){
      perror( "accept()" );
      return;
   }

   int flag = fcntl( sock, F_GETFL, 0 );
   fcntl( sock, F_SETFL, flag | O_NONBLOCK );

   fprintf( stdout, "accept. sock:%d\n", sock );
   server_add( sock, EPOLLIN, callback_client, NULL );
}

//! @brief    クライアントコールバック
static void callback_client( int fd, void *arg )
{
   fprintf( stdout, "in callback_client. fd:%d\n", fd );

   char buff[1024];
   int ret = ::recv( fd, buff, sizeof(buff), 0 );
   if( ret <= 0 ){
      fprintf( stdout, "exit. fd:%d ret:%d\n", fd, ret );
      server_del( fd );
   }
   else{
      fprintf( stdout, "fd:%d -> %s\n", fd, buff );
   }
}

int main()
{
   server_init();
   server_listen();
   return server_loop();
}
