/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

typedef unsigned __int64 uint64_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef __int64 int64_t;
typedef int int32_t;
typedef __int16 int16_t;
typedef char int8_t;

// IP Protocol
#define IP_PROTO_TCP			0x06
#define IP_PROTO_UDP			0x11
#define IP_PROTO_ICMP			0x01
#define IP_PROTO_ICMP_V6	0x3A

// TCP flags 
#define TCP_FIN 0x01 
#define TCP_SYN 0x02 
#define TCP_RST 0x04 
#define TCP_PSH 0x08 
#define TCP_ACK 0x10 
#define TCP_URG 0x20 
#define TCP_ECE 0x40 
#define TCP_CWR 0x80 

/**
 * @ingroup PcapViewer
 * @brief IPv4 header
 */
typedef struct _Ip4Header_
{
	uint8_t		ver_ihl;				// Version (4 bits) + Internet header length (4 bits)
	uint8_t		tos;						// Type of service 
	uint16_t	tlen;						// Total length = IPv4 header 를 포함한 패킷 길이
	uint16_t	identification; // Identification
	uint16_t	flags_fo;				// Flags (3 bits) + Fragment offset (13 bits)
	uint8_t		ttl;						// Time to live
	uint8_t		proto;					// Protocol
	uint16_t	crc;						// Header checksum
	uint32_t	saddr;					// Source address
	uint32_t	daddr;					// Destination address
	uint32_t	op_pad;					// Option + Padding
} Ip4Header;

/**
 * @ingroup PcapViewer
 * @brief IPv6 base header
 */
typedef struct _Ip6Header_
{
	uint8_t		ver_ihl;
	uint8_t		flow_level[3];
	uint16_t	payload_length;		// Payload length : IPv6 header 를 제외한 패킷 길이
	uint8_t		next_header;			// Protocol
	uint8_t		hop_limit;
	uint8_t		saddr[16];				// Source address
	uint8_t		daddr[16];				// Destination address
} Ip6Header;

/**
 * @ingroup PcapViewer
 * @brief IPv6 option extension header
 */
typedef struct _Ip6OptionHeader_
{
	uint8_t		next_header;			// Protocol
	uint8_t		header_len;				// option header length
} Ip6OptionHeader;

/**
 * @ingroup PcapViewer
 * @brief UDP header
 */
typedef struct _UdpHeader_
{
	uint16_t sport;		// Source port
	uint16_t dport;		// Destination port
	uint16_t len;			// Datagram length
	uint16_t crc;			// Checksum
} UdpHeader;

/**
 * @ingroup PcapViewer
 * @brief TCP Header
 */
typedef struct _TcpHeader_ 
{
	uint16_t sport;	// Source port
	uint16_t dport;	// Destination port
	uint32_t seqnum; // Sequence Number
	uint32_t acknum; // Acknowledgement number
	uint8_t	 hlen;		// Header length - 상위 4bit 만 header 길이이다.
	uint8_t	 flags;	// packet flags
	uint16_t win;		// Window size
	uint16_t crc;		// Header Checksum
	uint16_t urgptr; // Urgent pointer...still don't know what this is...
} TcpHeader;

bool IsUdpPacket( Ip4Header * psttIp4Header );
bool IsTcpPacket( Ip4Header * psttIp4Header );
int GetIpHeaderLength( Ip4Header * psttIp4Header );
void GetIpString( uint32_t iIp, char * pszIp, int iIpSize );
int GetTcpHeaderLength( TcpHeader * psttTcpHeader );

#endif
