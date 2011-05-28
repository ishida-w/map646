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

   struct stat_element{
      struct {
         uint64_t num;
         uint64_t error;
         uint64_t len[5];
      }icmp;

      struct {
         uint64_t num;      
         uint64_t error;
         uint64_t len[5];
         std::map<int, uint64_t> port_stat;
      }tcp;

      struct {
         uint64_t num;
         uint64_t error;
         uint64_t len[5];
         std::map<int, uint64_t> port_stat;
      }udp;
   };

   struct map646_in_addr{
      in_addr addr;
      map646_in_addr(in_addr a){
         addr = a;
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
      map646_in6_addr(in6_addr a){
         addr = a;
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
         std::cout << "map646_in6_addr: get_addr()" << std::endl;
         char str[256];
         inet_ntop(AF_INET6, &addr, str, 256);
         return std::string(str);
      }
   };


   class stat{
      public:
         int update(const uint8_t *bufp, ssize_t len, uint8_t d);
         int show();
      private:
         int get_hist(int len);
         std::map<map646_in_addr, stat_element> stat_4to6;
         std::map<map646_in_addr, stat_element> stat_6to4;
         std::map<map646_in6_addr, stat_element> stat66_GtoI;
         std::map<map646_in6_addr, stat_element> stat66_ItoG;
   };
}
