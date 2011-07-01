#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <err.h>

#include <dirent.h>

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
#include "stat_file.h"
#define STAT_SOCK "/tmp/map646_stat"

void cleanup_sigint(int);
int merge(json_object* jobj);
tm find_last_update(json_object* jobj, json_object* jyear, json_object* jmonth, json_object* jday, json_object* jhour);

int fd;

int main(int argc, char** argv)
{
   
   if (signal(SIGINT, cleanup_sigint) == SIG_ERR) {
      err(EXIT_FAILURE, "failed to register a SIGINT hook.");
   }
   
   std::string dirname;
   date ctime;
   
   /* read args */
   if(!(argc == 3 || argc == 5)){
      std::cout << "usage: -d dirname [-t] time" << std::endl;
      exit(1);
   }


   for(int i = 0; i < argc; i++){
      
      if(!strcmp("-d",argv[i])){
         if(i + 1 < argc){
            dirname = argv[i+1];
         }else{
            std::cout << "usage: -d dirname [-t] time" << std::endl;
            exit(1);
         }
      }
      
      if(!strcmp("-t", argv[i])){
         if( i + 1 < argc ){
            
            std::string s_time(argv[i+1]);
            ctime.set_time(s_time);

         }else{
            std::cout << "usage: -d dirname [-t] time" << std::endl;
            exit(1);
         }
      }
   }

   if(dirname == ""){
      std::cout << "usage: -d dirname [-t] time" << std::endl;
      exit(1);
   }


   /* get new stat data */
   sockaddr_un addr;

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

   write(fd, "send", sizeof("send"));

   int n;
   read(fd, (void *)&n, sizeof(int));
   char input[n];
   read(fd, input, n);
   json_object *new_jobj = json_tokener_parse(input);

   if(is_error(new_jobj)){
      perror("parse failed");
      exit(1);
   }

   /* flush map646 stat */
   if((fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
      perror("socket");
      exit(1);
   }

   if(connect(fd, (sockaddr *)&addr, sizeof(addr.sun_family) + strlen(STAT_SOCK)) < 0){
      perror("connect");
      exit(1);
   }
   write(fd, "flush", sizeof("flush"));

  /* update stat files*/
  stat_file_manager fm(dirname);
 
  if(!fm.empty()){
    fm.merge(ctime); 
  }
  
  fm.add(ctime, new_jobj);
}

void cleanup_sigint(int dummy){
   close(fd);
   exit(0);
}


int merge(json_object* jobj){
   if(jobj == NULL){
      warnx("merge() arg is NULL");
      return -1;
   }

   std::map<map646_stat::map646_in_addr, map646_stat::stat_chunk> stat;
   std::map<map646_stat::map646_in6_addr, map646_stat::stat_chunk> stat66;
   std::vector<std::string> key_buf;

   json_object_object_foreach(jobj, key, jcolumn){

      key_buf.push_back(key);

      json_object *jv4 = json_object_object_get(jcolumn, "v4");

      if(jv4 != NULL){

         json_object_object_foreach(jv4, key, jchunk){
            map646_stat::map646_in_addr addr(key);
            json_object_object_foreach(jchunk, key, jelement){

               int proto = map646_stat::get_proto_ID(key);
               
               if(jelement != NULL){
                  stat[addr].stat_element[proto].num += json_object_get_int(json_object_object_get(jelement, "num"));
                  json_object *jlen = json_object_object_get(jelement, "len");
                  if(jlen != NULL){
                     json_object_object_foreach(jlen, key, value){
                        std::stringstream ss;
                        int len;
                        ss << key;
                        ss >> len;
                        stat[addr].stat_element[proto].len[len] += json_object_get_int(value);
                     }
                  }

                  json_object *jport = json_object_object_get(jelement, "port");
                  if(jport != NULL){
                     json_object_object_foreach(jport, key, value){
                        std::stringstream ss;
                        int port;
                        ss << key;
                        ss >> port;
                        stat[addr].stat_element[proto].port_stat[port] += json_object_get_int(value);
                     }
                  }
               }
            }
         }
      }

      json_object *jv6 = json_object_object_get(jcolumn, "v6");

      if(jv6 != NULL){

         json_object_object_foreach(jv6, key6, jchunk6){
            map646_stat::map646_in6_addr addr6(key6);
            json_object_object_foreach(jchunk6, key, jelement){
               int proto = map646_stat::get_proto_ID(key);

               if(jelement != NULL){
                  stat66[addr6].stat_element[proto].num += json_object_get_int(json_object_object_get(jelement, "num"));
                  json_object *jlen = json_object_object_get(jelement, "len");
                  if(jlen != NULL){
                     json_object_object_foreach(jlen, key, value){
                        std::stringstream ss;
                        int len;
                        ss << key;
                        ss >> len;
                        stat66[addr6].stat_element[proto].len[len] += json_object_get_int(value);
                     }
                  }

                  json_object *jport = json_object_object_get(jelement, "port");
                  if(jport != NULL){
                     json_object_object_foreach(jport, key, value){
                        std::stringstream ss;
                        int port;
                        ss << key;
                        ss >> port;
                        stat66[addr6].stat_element[proto].port_stat[port] += json_object_get_int(value);
                     }
                  }
               }
            }
         }
      }
   }
  
   std::vector<std::string>::iterator it = key_buf.begin();

   while(it != key_buf.end()){
      json_object_object_del(jobj, it->c_str());
      it++;
   }

   if(stat.empty())
      json_object_object_add(jobj, "v4", NULL);
   else{
      std::map<map646_stat::map646_in_addr, map646_stat::stat_chunk>::iterator it = stat.begin();
      json_object *v4 = json_object_new_object();

      while(it != stat.end()){
         json_object *chunk = json_object_new_object();
         for(int i = 0; i < 6; i++){
            json_object *element = json_object_new_object();

            json_object *num = json_object_new_int(it->second.stat_element[i].num);

            if(num == 0){
               json_object_object_add(chunk, map646_stat::get_proto(i).c_str(), NULL);
            }else{
            json_object_object_add(element, "num", num);

            if(it->second.stat_element[i].len.empty()){
               json_object_object_add(element, "len", NULL);
            } else{
               std::map<int, int>::iterator len_it = it->second.stat_element[i].len.begin();
               json_object *len = json_object_new_object();
               while(len_it != it->second.stat_element[i].len.end()){
                  std::stringstream ss;
                  ss << len_it->first;
                  std::string s = ss.str();
                  json_object_object_add(len, s.c_str(), json_object_new_int(len_it->second));
                  len_it++;
               }
               json_object_object_add(element, "len", len);
            }

            if(it->second.stat_element[i].port_stat.empty()){
               json_object_object_add(element, "port", NULL);
            }else{
               std::map<int, int>::iterator port_it = it->second.stat_element[i].port_stat.begin();
               json_object *port = json_object_new_object();
               while(port_it != it->second.stat_element[i].port_stat.end()){
                  std::stringstream ss;
                  ss << port_it->first;
                  std::string s = ss.str();
                  json_object_object_add(port,s.c_str(), json_object_new_int(port_it->second));
                  port_it++;
               }
               json_object_object_add(element, "port", port);
            }
            json_object_object_add(chunk, map646_stat::get_proto(i).c_str(), element); 
            }
         }
         json_object_object_add(v4, (it->first.get_addr()).c_str(), chunk);
         it++;
      }
      json_object_object_add(jobj, "v4", v4);
   }

   if(stat66.empty())
      json_object_object_add(jobj, "v6", NULL);
   else{
      std::map<map646_stat::map646_in6_addr, map646_stat::stat_chunk>::iterator it6 = stat66.begin();
      json_object *v6 = json_object_new_object();

      while(it6 != stat66.end()){
         json_object *chunk = json_object_new_object();
         for(int i = 0; i < 6; i++){
            json_object *element = json_object_new_object();

            json_object *num = json_object_new_int(it6->second.stat_element[i].num);

            if(num == 0){
               json_object_object_add(chunk, map646_stat::get_proto(i).c_str(), NULL);
            }else{

               json_object_object_add(element, "num", num);

               if(it6->second.stat_element[i].len.empty()){
                  json_object_object_add(element, "len", NULL);
               } else{
                  std::map<int, int>::iterator len_it = it6->second.stat_element[i].len.begin();
                  json_object *len = json_object_new_object();
                  while(len_it != it6->second.stat_element[i].len.end()){
                     std::stringstream ss;
                     ss << len_it->first;
                     std::string s = ss.str();
                     json_object_object_add(len, s.c_str(), json_object_new_int(len_it->second));
                     len_it++;
                  }
                  json_object_object_add(element, "len", len);
               }
               
               if(it6->second.stat_element[i].port_stat.empty()){
                  json_object_object_add(element, "port", NULL);
               }else{
                  std::map<int, int>::iterator port_it = it6->second.stat_element[i].port_stat.begin();
                  json_object *port = json_object_new_object();
                  while(port_it != it6->second.stat_element[i].port_stat.end()){
                     std::stringstream ss;
                     ss << port_it->first;
                     std::string s = ss.str();
                     json_object_object_add(port,s.c_str(), json_object_new_int(port_it->second));
                     port_it++;
                  }
                  json_object_object_add(element, "port", port);
               }
               json_object_object_add(chunk, map646_stat::get_proto(i).c_str(), element); 
            }
         }
         json_object_object_add(v6, (it6->first.get_addr()).c_str(), chunk);
         it6++;
      }

      json_object_object_add(jobj, "v6", v6);
   }

   return 0;
}

tm find_last_update(json_object* jobj, json_object* jyear, json_object* jmonth, json_object* jday, json_object* jhour){

   std::stringstream slastyear, slastmonth, slastday, slasthour, slastmin;
   tm lastupdate;
   {
      int year = 0, temp;
      json_object_object_foreach(jobj, key, value){
         std::stringstream ss;
         ss << key;
         ss >> temp;
         if(temp > year)
            year = temp;
      }
      slastyear << year;
      lastupdate.tm_year = year - 1900;

      jyear = json_object_object_get(jobj, slastyear.str().c_str());
   }
   {
      int month = 0, temp;
      json_object_object_foreach(jyear, key, value){
         std::stringstream ss;
         ss << key;
         ss >> temp;
         if(temp > month)
            month = temp;
      }
      slastmonth << month;
      lastupdate.tm_mon = month - 1;

      jmonth = json_object_object_get(jyear, slastmonth.str().c_str());
   }
   {
      int day = 0, temp;
      json_object_object_foreach(jmonth, key, value){
         std::stringstream ss;
         ss << key;
         ss >> temp;
         if(temp > day)
            day = temp;
      }
      slastday << day;
      lastupdate.tm_mday = day;

      jday = json_object_object_get(jmonth, slastday.str().c_str());
   }
   {
      int hour = 0, temp;
      json_object_object_foreach(jday, key, value){
         std::stringstream ss;
         ss << key;
         ss >> temp;
         if(temp > hour)
            hour = temp;
      }
      slasthour << hour;
      lastupdate.tm_hour = hour;

      jhour = json_object_object_get(jday, slasthour.str().c_str());
   }
   {
      int min = 0, temp;
      json_object_object_foreach(jhour, key, value){
         std::stringstream ss;
         ss << key;
         ss >> temp;
         if(temp > min)
            min = temp;
      }
      slastmin << min;
      lastupdate.tm_min = min;
   }

   return lastupdate;

}

