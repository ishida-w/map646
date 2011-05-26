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
         int update(const uint8_t *bufp, ssize_t len, uint8_t d);
//         void send_info(int fd);

      private:
         int get_hist(int len);
         int get_hash_index(const uint8_t *data, size_t data_len);
         std::map<int, stat_element> stat_4to6;
         std::map<int, stat_element> stat_6to4;
         std::map<int, stat_element> stat66_GtoI;
         std::map<int, stat_element> stat66_ItoG;
  };

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

}

#endif
