//
//  main.cpp
//  pcap_lookupdev
//
//  Created by 崔翔飞 on 2017/3/25.
//  Copyright © 2017年 崔翔飞. All rights reserved.
//


#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>

int test_pcap_lookupdev(int argc, const char * argv[])
{
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    // 获得pcap认为要sniff的接口
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return(2);
    }
    printf("Device: %s\n", dev);
    /* Find the properties for the device */
    bpf_u_int32 net;
    bpf_u_int32 mask;   // out netmask
    // net是dev上ipv4的地址 & mask
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }
    struct in_addr in_ip {mask};
    char* sz_netmask = inet_ntoa(in_ip);
    fprintf(stdout, "netmask: %s\n", sz_netmask);
    in_ip.s_addr = net;
    char* sz_local_net_addr = inet_ntoa(in_ip);
    fprintf(stdout, "local ip net addr: %s\n", sz_local_net_addr);
    /* Open the session in promiscuous mode */
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return(2);
    }
    /* Grab a packet */
    struct pcap_pkthdr header;
    const u_char* packet = pcap_next(handle, &header);
    fprintf(stdout, "packet: %s\n", packet);
#ifdef __APPLE__
    fprintf(stdout, "head comment: %s\n", header.comment);
#endif
    pcap_close(handle);
    return(0);
}
