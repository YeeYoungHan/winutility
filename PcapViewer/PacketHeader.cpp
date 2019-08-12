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

#include "stdafx.h"
#include "PacketHeader.h"
#include <stdio.h>

/**
 * @ingroup PcapViewer
 * @brief UDP 패킷인지 검사한다.
 * @param psttIp4Header IPv4 헤더
 * @returns UDP 패킷이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsUdpPacket( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		if( psttIp4Header->proto == IP_PROTO_UDP )
		{
			return true;
		}
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		Ip6Header	* psttIp6Header = ( Ip6Header * )psttIp4Header;

		if( psttIp6Header->next_header == IP_PROTO_UDP )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup PcapViewer
 * @brief TCP 패킷인지 검사한다.
 * @param psttIp4Header IPv4 헤더
 * @returns TCP 패킷이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsTcpPacket( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		if( psttIp4Header->proto == IP_PROTO_TCP )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		Ip6Header	* psttIp6Header = ( Ip6Header * )psttIp4Header;

		if( psttIp6Header->next_header == IP_PROTO_TCP )
		{
			return true;
		}
	}

	return false;
}

/** IP 헤더 길이를 리턴한다.
 * @ingroup PcapViewer
 * @brief IP 헤더 길이를 리턴한다.
 * @param psttIp4Header IPv4 헤더
 * @returns IP 헤더 길이를 리턴한다.
 */
int GetIpHeaderLength( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		return ( psttIp4Header->ver_ihl & 0x0F ) * 4;
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		return 40;
	}

	return 0;
}

/**
 * @ingroup PcapViewer
 * @brief network order IP 주소를 문자열 IP 주소에 저장한다.
 * @param iIp			IP 주소
 * @param pszIp		[out] IP 주소 저장 변수
 * @param iIpSize pszIp 변수의 크기
 */
void GetIpString( uint32_t iIp, char * pszIp, int iIpSize )
{
	snprintf( pszIp, iIpSize, "%d.%d.%d.%d", iIp & 0xFF, ( iIp >> 8 ) & 0xFF, ( iIp >> 16 ) & 0xFF, ( iIp >> 24 ) & 0xFF );
}

/**
 * @ingroup PcapViewer
 * @brief TCP 헤더 길이를 리턴한다.
 * @param psttTcpHeader TCP 헤더
 * @returns TCP 헤더 길이를 리턴한다.
 */
int GetTcpHeaderLength( TcpHeader * psttTcpHeader )
{
	return ( ( psttTcpHeader->hlen >> 4 ) & 0x0F ) * 4;
}
