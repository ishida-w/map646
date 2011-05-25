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

#ifndef __STAT_H__
#define __STAT_H__

#define STAT_SOCK "/tmp/map646_stat"
#include <map>

namespace map646_stat{

   const int IN = true;
   const int OUT = false;
   const int STAT_TABLE_HASH_SIZE =  1009;

   int statif_alloc();
   
   struct stat_element;

   class stat{
      public:
         void update(const in_addr* addr, int port, uint8_t proto, bool inOrOut, size_t len, bool error);
         void update(const in_addr* addr, uint8_t proto, bool inOrOut, size_t len, bool error);
         void update(const in6_addr* addr, int port, uint8_t proto, bool inOrOut, size_t len, bool error);
         void update(const in6_addr* addr, uint8_t proto, bool inOrOut, size_t len, bool error);

         void send_info(int fd);
      private:
         int get_hist(int len);
         int get_hash_index(const void *data, int data_len);
         std::map<int, stat_element> v4_stat;
         std::map<int, stat_element> v6_stat;
  };

   struct stat_element{
      struct icmp{
         uint64_t num;
         uint64_t error;
         uint64_t len[5];
      }icmp_in, icmp_out;
      
      struct tcp{
         uint64_t num;      
         uint64_t error;
         uint64_t len[5];
         std::map<int, uint64_t> port_stat;
      }tcp_in, tcp_out;
      
      struct udp{
         uint64_t num;
         uint64_t error;
         uint64_t len[5];
         std::map<int, uint64_t> port_stat;
      }udp_in, udp_out;
   };

}

#endif
