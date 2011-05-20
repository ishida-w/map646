#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define STAT_SOCK "/tmp/map646_stat"

main()
{
   sockaddr_un addr;
   int fd;
   int len;
   char buf[1024];
   int ret;

   ret = 1024;

   if((fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
      perror("socket");
      exit(1);
   }

   memset((char *)&addr, 0, sizeof(addr));

   addr.sun_family = AF_UNIX;
   strcpy(addr.sun_path, STAT_SOCK); 

   if(connect(fd, (sockaddr *)&addr, sizeof(addr.sun_family) + strlen(STAT_SOCK)) < 0){
      perror("connect");
      exit(1);
   }
   
   while(ret = read(fd, buf, 1024)){
      std::cout << buf << std::endl;
   }



}

