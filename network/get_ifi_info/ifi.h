/*************************************************************************
  > File Name: ifi.h
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 17 Jul 2016 01:56:50 AM PDT
 ************************************************************************/

#ifndef _IFI_H_
#define _IFI_H_

#include <iostream>
using namespace std;

#include <net/if.h> // for interface

#define IFI_NAME  16
#define IFI_HADDR 8

struct ifi_info
{
  char  ifi_name[IFI_NAME];   // interface name, null-terminated
  short ifi_index;            // interface index? index for waht ? 
  short ifi_mtu;              // interface MTU
  unsigned char ifi_haddr[IFI_HADDR]; // hardware address
  unsigned short ifi_hlen;    // bytes in hardware address: 0, 6, 8
  short ifi_flags;
  short ifi_myflags;
  struct sockaddr*  ifi_addr; // primary address
  struct sockaddr*  ifi_brdaddr;  // broadcast address
  struct sockaddr*  ifi_dstaddr;  // destination address
  struct ifi_info*  ifi_next; // next of these structures
};

#define IFI_ALIAS 1

struct ifi_info*  get_ifi_info(int, int);
void free_ifi_info(struct ifi_info*);

#endif
