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
 * @brief UDP ��Ŷ���� �˻��Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns UDP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief TCP ��Ŷ���� �˻��Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns TCP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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

/** IP ��� ���̸� �����Ѵ�.
 * @ingroup PcapViewer
 * @brief IP ��� ���̸� �����Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns IP ��� ���̸� �����Ѵ�.
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
 * @brief network order IP �ּҸ� ���ڿ� IP �ּҿ� �����Ѵ�.
 * @param iIp			IP �ּ�
 * @param pszIp		[out] IP �ּ� ���� ����
 * @param iIpSize pszIp ������ ũ��
 */
void GetIpString( uint32_t iIp, char * pszIp, int iIpSize )
{
	snprintf( pszIp, iIpSize, "%d.%d.%d.%d", iIp & 0xFF, ( iIp >> 8 ) & 0xFF, ( iIp >> 16 ) & 0xFF, ( iIp >> 24 ) & 0xFF );
}

/**
 * @ingroup PcapViewer
 * @brief MAC �ּҸ� ���ڿ��� �����Ѵ�.
 * @param pszPacket MAC �ּ�
 * @param pszMac		[out] MAC �ּ� ���� ����
 * @param iMacSize	pszMac ������ ũ��
 */
void GetMacString( const u_char * pszPacket, char * pszMac, int iMacSize )
{
	snprintf( pszMac, iMacSize, "%02X:%02X:%02X:%02X:%02X:%02X", pszPacket[0], pszPacket[1], pszPacket[2], pszPacket[3], pszPacket[4], pszPacket[5] );
}

/**
 * @ingroup PcapViewer
 * @brief TCP ��� ���̸� �����Ѵ�.
 * @param psttTcpHeader TCP ���
 * @returns TCP ��� ���̸� �����Ѵ�.
 */
int GetTcpHeaderLength( TcpHeader * psttTcpHeader )
{
	return ( ( psttTcpHeader->hlen >> 4 ) & 0x0F ) * 4;
}