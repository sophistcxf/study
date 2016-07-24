/*************************************************************************
  > File Name: get_ifi_info.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 17 Jul 2016 04:53:20 AM PDT
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#include "ifi.h"
#include "error.h"

struct ifi_info* get_ifi_info(int family, int doaliases)
{
  struct ifconf ifc;
  int sockfd = socket(family, SOCK_DGRAM, 0);
  if (sockfd == -1) exit(-1);
  int len = 100 * sizeof(struct ifreq);
  int lastlen = 0;
  char lastname[IFNAMSIZ];
  char *buf;
  struct sockaddr_in *sinptr;
  struct sockaddr_in6 *sin6ptr;
  // 不断尝试调用ioctl，直到分配的缓存足够，或者返回EINVAL
  // 有些实现ioctl的返回值并不确保函数执行是完整的，当缓存不够时数据可能被截断
  while (true) {
    buf = (char*)malloc(len);
    ifc.ifc_len = len;
    ifc.ifc_buf = buf;
    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
      if (errno != EINVAL || lastlen != 0)
        err_sys("ioctl error");
    } else {
      if (ifc.ifc_len == lastlen) break;
      lastlen = ifc.ifc_len;
    }
    len += 10 * sizeof(struct ifreq);
    free(buf);
  }
  struct ifi_info *ifihead = NULL;
  struct ifi_info **ifipnext = &ifihead;
  lastname[0] = 0;
  char *sdlname = NULL;
  char *ptr, *cptr, *haddr;
  struct ifreq *ifr, ifrcopy;
  int myflags, flags, idx = 0, hlen = 0;
  for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
    ifr = (struct ifreq *) ptr;

#ifdef HAVE_SOCKADDR_SA_LEN
    len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
    switch (ifr->ifr_addr.sa_family) {
#ifdef IPV6
      case AF_INET6:
        len = sizeof(struct sockaddr_in6); break;
#endif
      case AF_INET:
      default:
        len = sizeof(struct sockaddr); break;
      }
#endif /* HAVE_SOCKADDR_SA_LEN */
    ptr += sizeof(ifr->ifr_name) + len;
#ifdef HAVE_SOCKADDR_DL_STRUCT
#endif
    if (ifr->ifr_addr.sa_family != family) continue; 
    myflags = 0;
    if ((cptr = strchr(ifr->ifr_name, ';')) != NULL) *cptr = 0;
    if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0) {
      if (doaliases == 0) continue;
      myflags = IFI_ALIAS;
    }
    memcpy(lastname, ifr->ifr_name, IFNAMSIZ);
    ifrcopy = *ifr;
    ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
    flags = ifrcopy.ifr_flags;
    // ignore if interface not up
    if ((flags & IFF_UP) == 0) continue;
		struct ifi_info *ifi = (struct ifi_info *)calloc(1, sizeof(struct ifi_info));
		*ifipnext = ifi;			/* prev points to this new one */
		ifipnext = &ifi->ifi_next;	/* pointer to next one goes here */

		ifi->ifi_flags = flags;		/* IFF_xxx values */
		ifi->ifi_myflags = myflags;	/* IFI_xxx values */
#if defined(SIOCGIFMTU) && defined(HAVE_STRUCT_IFREQ_IFR_MTU)
		ioctl(sockfd, SIOCGIFMTU, &ifrcopy);
		ifi->ifi_mtu = ifrcopy.ifr_mtu;
#else
		ifi->ifi_mtu = 0;
#endif
		memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
		ifi->ifi_name[IFI_NAME-1] = '\0';
		/* If the sockaddr_dl is from a different interface, ignore it */
		if (sdlname == NULL || strcmp(sdlname, ifr->ifr_name) != 0)
			idx = hlen = 0;
		ifi->ifi_index = idx;
		ifi->ifi_hlen = hlen;
		if (ifi->ifi_hlen > IFI_HADDR)
			ifi->ifi_hlen = IFI_HADDR;
		if (hlen)
			memcpy(ifi->ifi_haddr, haddr, ifi->ifi_hlen);

		switch (ifr->ifr_addr.sa_family) {
		case AF_INET:
			sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
			ifi->ifi_addr = (struct sockaddr *)calloc(1, sizeof(struct sockaddr_in));
			memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));

#ifdef	SIOCGIFBRDADDR
			if (flags & IFF_BROADCAST) {
				ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
				sinptr = (struct sockaddr_in *) &ifrcopy.ifr_broadaddr;
				ifi->ifi_brdaddr = (struct sockaddr *)calloc(1, sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
			}
#endif

#ifdef	SIOCGIFDSTADDR
			if (flags & IFF_POINTOPOINT) {
				ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
				sinptr = (struct sockaddr_in *) &ifrcopy.ifr_dstaddr;
				ifi->ifi_dstaddr = (struct sockaddr *)calloc(1, sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
			}
#endif
			break;

		case AF_INET6:
			sin6ptr = (struct sockaddr_in6 *) &ifr->ifr_addr;
			ifi->ifi_addr = (struct sockaddr *)calloc(1, sizeof(struct sockaddr_in6));
			memcpy(ifi->ifi_addr, sin6ptr, sizeof(struct sockaddr_in6));

#ifdef	SIOCGIFDSTADDR
			if (flags & IFF_POINTOPOINT) {
				ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
				sin6ptr = (struct sockaddr_in6 *) &ifrcopy.ifr_dstaddr;
				ifi->ifi_dstaddr = (struct sockaddr *)calloc(1, sizeof(struct sockaddr_in6));
				memcpy(ifi->ifi_dstaddr, sin6ptr, sizeof(struct sockaddr_in6));
			}
#endif
			break;

		default:
			break;
		}
	}
	free(buf);
	return(ifihead);	/* pointer to first structure in linked list */
}
void
free_ifi_info(struct ifi_info *ifihead)
{
	struct ifi_info	*ifi, *ifinext;

	for (ifi = ifihead; ifi != NULL; ifi = ifinext) {
		if (ifi->ifi_addr != NULL)
			free(ifi->ifi_addr);
		if (ifi->ifi_brdaddr != NULL)
			free(ifi->ifi_brdaddr);
		if (ifi->ifi_dstaddr != NULL)
			free(ifi->ifi_dstaddr);
		ifinext = ifi->ifi_next;	/* can't fetch ifi_next after free() */
		free(ifi);					/* the ifi_info{} itself */
	}
}
