#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <err.h>

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

#include <json/json.h>

#include "stat.h"

#define STAT_SOCK "/tmp/map646_stat"

using namespace map646_stat;
void cleanup_sigint(int);
int fd;

int main()
{
   sockaddr_un addr;

   if (signal(SIGINT, cleanup_sigint) == SIG_ERR) {
      err(EXIT_FAILURE, "failed to register a SIGINT hook.");
   }
   while(true){

      std::string command;
      std::cin >> command;

      if(command == "send"){

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

         std::cout << "command: send" << std::endl;
         write(fd, "send", sizeof("send"));
         int n;
         read(fd, (void *)&n, sizeof(int));
         char buf[n];
         read(fd, buf, n);
         json_object *jobj = json_tokener_parse(buf);
         std::cout << json_object_to_json_string(jobj) << std::endl;
      }else if(command == "flush"){

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
         std::cout << "command: flush" << std::endl;
         write(fd, "flush", sizeof("flush"));
      }else if(command == "quit"){
         std::cout << "bye" <<std::endl;
         close(fd);
         exit(0);
      }else if(command == "toggle"){
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
         std::cout << "command: toggle" << std::endl;
         write(fd, "toggle", sizeof("toggle"));

      }else if(command == "time"){
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
         std::cout << "command: time" << std::endl;
         write(fd, "time", sizeof("time"));

      }else{
         std::cout << "unknown command: commands are send | flush | quit | toggle" << std::endl;
      }
   }

   return 0;
}

void cleanup_sigint(int dummy){
   close(fd);
   exit(0);
}
