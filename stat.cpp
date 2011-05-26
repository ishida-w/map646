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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <assert.h>
#include <err.h>
#include <unistd.h>

#if !defined(__linux__)
#include <sys/types.h>
#include <sys/param.h>
#endif
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>

#include <net/if.h>
#if defined(__linux__)
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/fib_rules.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#else
#include <ifaddrs.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <net/if_tun.h>
#endif
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>


#include "stat.h"
#include "mapping.h"

#include <iostream>
#include <string>

namespace map646_stat{

int statif_alloc(){
   int stat_listen_fd;   
   sockaddr_un saddr;
   
   if((stat_listen_fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
      errx(EXIT_FAILURE, "failed to create stat socket");
   }

   memset((char *)&saddr, 0, sizeof(saddr));

   saddr.sun_family = AF_UNIX;
   strcpy(saddr.sun_path, STAT_SOCK);

   unlink(STAT_SOCK);
   if(bind(stat_listen_fd, (sockaddr *)&saddr, sizeof(saddr.sun_family) + strlen(STAT_SOCK)) < 0){
      errx(EXIT_FAILURE, "failed to bind stat socket");
   }

   if(listen(stat_listen_fd, 1) < 0){
         errx(EXIT_FAILURE, "failed to listen to stat socket");
   }

   return stat_listen_fd;
}

int stat::update(const uint8_t *bufp, ssize_t len, uint8_t d){
   assert(bufp != NULL);

   switch(d){
      case FOURTOSIX:{
         ip* ip4_hdrp = (ip*)bufp;
         int hash = get_hash_index((const uint8_t*)&ip4_hdrp->ip_dst, sizeof(in_addr));
         uint8_t ip4_proto = ip4_hdrp->ip_p;
         if(ip4_proto == IPPROTO_ICMP){
            stat_4to6[hash].icmp.num++;
         }else if(ip4_proto == IPPROTO_TCP){
            stat_4to6[hash].tcp.num++;
         }else if(ip4_proto == IPPROTO_UDP){
            stat_4to6[hash].udp.num++;
         }
         break;
      }
      case SIXTOFOUR:{
         ip6_hdr* ip6_hdrp = (ip6_hdr*)bufp;
         in_addr service_addr;
         if(mapping_convert_addrs_6to4(&ip6_hdrp->ip6_src, NULL, &service_addr, NULL) < 0)
            break;
         int hash = get_hash_index((const uint8_t*)&service_addr, sizeof(in_addr));
         uint8_t ip6_proto =  ip6_hdrp->ip6_nxt;
         if(ip6_proto == IPPROTO_ICMPV6){
            stat_6to4[hash].icmp.num++;
         }else if(ip6_proto == IPPROTO_TCP){
            stat_6to4[hash].tcp.num++;
         }else if(ip6_proto == IPPROTO_UDP){
            stat_6to4[hash].udp.num++;
         }
         break;
         }
      case SIXTOSIX_GtoI:{
         ip6_hdr* ip6_hdrp = (ip6_hdr*)bufp;
         int hash = get_hash_index((const uint8_t*)&ip6_hdrp->ip6_dst, sizeof(in6_addr));
         uint8_t ip6_proto =  ip6_hdrp->ip6_nxt;
         if(ip6_proto == IPPROTO_ICMPV6){
            stat66_GtoI[hash].icmp.num++;
         }else if(ip6_proto == IPPROTO_TCP){
            stat66_GtoI[hash].tcp.num++;
         }else if(ip6_proto == IPPROTO_UDP){
            stat66_GtoI[hash].udp.num++;
         }
         break;
      }
      case SIXTOSIX_ItoG:{
         ip6_hdr* ip6_hdrp = (ip6_hdr*)bufp;
         in6_addr service_addr;
         if(mapping66_convert_addrs_ItoG(&ip6_hdrp->ip6_src, NULL, &service_addr, NULL) < 0)
            break;
         int hash = get_hash_index((const uint8_t*)&service_addr, sizeof(in6_addr));
         uint8_t ip6_proto =  ip6_hdrp->ip6_nxt;
         if(ip6_proto == IPPROTO_ICMPV6){
            stat66_ItoG[hash].icmp.num++;
         }else if(ip6_proto == IPPROTO_TCP){
            stat66_ItoG[hash].tcp.num++;
         }else if(ip6_proto == IPPROTO_UDP){
            stat66_ItoG[hash].udp.num++;
         }
         break;
      }
   }

   return 0;
}

int stat::get_hash_index(const uint8_t *data, size_t data_len){
   assert(data != NULL);
   assert(data_len > 0);

   uint16_t *datap = (uint16_t *)data;
   data_len = data_len >> 1;
   uint32_t sum = 0;
   while(data_len--){
      sum += *datap++;
   }

   return (sum % STAT_TABLE_HASH_SIZE);
}

int stat::get_hist(int len){
   return 0;
}
/*
void stat::send_info(int fd){
   std::cout << "stat_::send_info() called." << std::endl;
   std::string info;
   std::map<int, stat_element>::iterator it = v4_stat.begin();
   char ch[sizeof(uint64_t)];
//   info.append("4to6\n");
//   info.append("icmp in\n");

   if(!v4_stat.empty()){
      info.append("num: ");
      sprintf(ch, "%llu", (*it).second.icmp.num);
      info.append(ch);
   }
   
//   write(fd, str.c_str(), sizeof(str.c_str()));
   write(fd, info.c_str(), sizeof(info.c_str()));
}
*/
}
