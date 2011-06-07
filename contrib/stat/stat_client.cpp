#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>

#include "../../stat.h"

#define STAT_SOCK "/tmp/map646_stat"

using namespace map646_stat;

main()
{
   sockaddr_un addr;
   int fd;
   int len;
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

   int n;
   read(fd, (void *)&n, sizeof(int));
   char buf[n+100];

   read(fd, buf, n+10);

   std::cout << buf << std::endl;
  /* 
   for(int i = 0; i < time; i++){
      if(sizeof(map646_in_addr) == read(fd, buf, sizeof(map646_in_addr))){
         map646_in_addr* addr = (map646_in_addr*)buf;
         std::cout << addr->get_addr() << std::endl;
      }else{
         std::cout << "error handling" << std::endl;
      }

      if(sizeof(stat_chunk) == read(fd, buf, sizeof(stat_chunk))){
         stat_chunk chunk;
         chunk = *(stat_chunk *)buf;
      }
      
   }
   read(fd, buf, sizeof(int));

   time = *((int*)buf);

   for(int i = 0; i < time; i++){
      if(sizeof(map646_in6_addr) == read(fd, buf, sizeof(map646_in6_addr))){
         map646_in6_addr* addr = (map646_in6_addr*)buf;
         std::cout << addr->get_addr() << std::endl;
      }else{
         std::cout << "error handling" << std::endl;
      }

      if(sizeof(stat_element) == read(fd, buf, sizeof(stat_element))){
         stat_element element;
         element = *(stat_element *)buf;
         std::cout << element.icmp_in.num << std::endl;
      }
   }
*/
}
