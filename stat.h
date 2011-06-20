/*
 * Copyright 2010 IIJ Innovation Institute Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY IIJ INNOVATION INSTITUTE INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL IIJ INNOVATION INSTITUTE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#define STAT_SOCK "/tmp/map646_stat"
#include <map>
namespace map646_stat{

   int statif_alloc();

   struct stat_chunk{

#define ICMP_IN  0
#define ICMP_OUT 1
#define TCP_IN   2
#define TCP_OUT  3
#define UDP_IN   4
#define UDP_OUT  5

      struct _stat_element{
         int num;      
         int error;
         //int len[11];
         std::map<int, int> len;
         std::map<int, int> port_stat;
      }stat_element[6];

   };

   struct map646_in_addr{
      in_addr addr;
      map646_in_addr(const in_addr a){
         addr = a;
      }
      map646_in_addr(const char* a){
         inet_pton(AF_INET, a, (uint8_t*)&addr);
      }
      bool operator==(const map646_in_addr &rhs)const{
         return addr.s_addr == rhs.addr.s_addr;
      }
      bool operator<(const map646_in_addr &rhs)const{
         return addr.s_addr < rhs.addr.s_addr;
      }
      bool operator>(const map646_in_addr &rhs)const{
         return addr.s_addr > rhs.addr.s_addr;
      }
      std::string get_addr()const{
         char str[256];
         inet_ntop(AF_INET, &addr, str, 64);
         return std::string(str);
      }
   };

   struct map646_in6_addr{
      in6_addr addr;
      map646_in6_addr(const in6_addr a){
         addr = a;
      }
      map646_in6_addr(const char* a){
         inet_pton(AF_INET6, a, (uint8_t*)&addr);
      }
      bool operator==(const map646_in6_addr &rhs)const{
         for(int i = 0; i < 4; i++){
            if(addr.__in6_u.__u6_addr32[i] != rhs.addr.__in6_u.__u6_addr32[i]){
               return false;
            }
         }
         return true;
      }
      bool operator<(const map646_in6_addr& rhs)const{
         for(int i = 0; i < 4; i++){
            if(addr.__in6_u.__u6_addr32[i] < rhs.addr.__in6_u.__u6_addr32[i])
               return true;
            else if(addr.__in6_u.__u6_addr32[i] > rhs.addr.__in6_u.__u6_addr32[i])
               return false;
         }
         return false;
      }
      bool operator>(const map646_in6_addr &rhs)const{
         for(int i = 0; i < 4; i++){
            if(addr.__in6_u.__u6_addr32[i] < rhs.addr.__in6_u.__u6_addr32[i])
               return false;
            else if(addr.__in6_u.__u6_addr32[i] > rhs.addr.__in6_u.__u6_addr32[i])
               return true;
         }
         return false;
      }
      std::string get_addr()const{
         char str[256];
         inet_ntop(AF_INET6, &addr, str, 256);
         return std::string(str);
      }
   };

   class stat{
      public:
         int update(const uint8_t *bufp, ssize_t len, uint8_t d);
         int show();
         void flush();
         int send(int fd);
      private:
         std::string get_json();
         int get_hist(int len);
         std::map<map646_in_addr, stat_chunk> stat;
         std::map<map646_in6_addr, stat_chunk> stat66;
   };
   
   std::string get_proto(int proto);
   int get_proto_ID(const char* proto);
}
