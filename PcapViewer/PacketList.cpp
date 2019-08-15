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
#include "PacketList.h"
#include "PacketHeader.h"

#include "PacketListShow.hpp"

CPacket::CPacket() : m_iId(0), m_pszPacket(NULL)
{
}

CPacket::~CPacket()
{
	if( m_pszPacket )
	{
		free( m_pszPacket );
		m_pszPacket = NULL;
	}
}

CPacketList::CPacketList() : m_psttPcap(NULL)
{
}

CPacketList::~CPacketList()
{
	Close();
}

/**
 * @ingroup PcapViewer
 * @brief pcap 파일을 읽어서 자료구조에 저장한다.
 * @param pszFileName pcap 파일 full path
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CPacketList::Open( const char * pszFileName )
{
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr * psttHeader;
	const		u_char * pszData;
	bool		bError = false;
	int			iId = 0;

	Close();

	m_psttPcap = pcap_open_offline( pszFileName, szErrBuf );
	if( m_psttPcap == NULL )
	{
		return false;
	}

	while( pcap_next_ex( m_psttPcap, &psttHeader, &pszData ) > 0 )
	{
		CPacket * pclsPacket = new CPacket();
		if( pclsPacket == NULL )
		{
			bError = true;
			break;
		}

		memcpy( &pclsPacket->m_sttHeader, psttHeader, sizeof(pclsPacket->m_sttHeader) );

		pclsPacket->m_pszPacket = (u_char *)malloc( psttHeader->caplen );
		if( pclsPacket->m_pszPacket == NULL )
		{
			delete pclsPacket;
			bError = true;
			break;
		}

		memcpy( pclsPacket->m_pszPacket, pszData, psttHeader->caplen );
		pclsPacket->m_iId = iId;
		++iId;

		m_clsList.push_back( pclsPacket );
	}

	if( bError )
	{
		Close();
		return false;
	}

	return true;
}

/**
 * @ingroup PcapViewer
 * @brief pcap 파일을 close 한다.
 * @returns true 를 리턴한다.
 */
bool CPacketList::Close()
{
	DeleteAll();

	if( m_psttPcap )
	{
		pcap_close( m_psttPcap );
		m_psttPcap = NULL;
	}

	return true;
}

/**
 * @ingroup PcapViewer
 * @brief 입력된 인덱스의 패킷을 그 이전 패킷과 위치를 교체한다.
 * @param iIndex 패킷 인덱스
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CPacketList::Up( int iIndex )
{
	if( m_clsList.empty() ) return false;
	if( iIndex <= 0 || iIndex >= (int)m_clsList.size() ) return false;

	PACKET_LIST::iterator itPL, itPrev;
	int iCurIndex = 0;
	bool bRes = false;

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		if( iCurIndex == ( iIndex - 1 ) )
		{
			itPrev = itPL;
		}
		else if( iCurIndex == iIndex )
		{
			CPacket * pclsPrevPacket = *itPrev;
			*itPrev = *itPL;
			*itPL = pclsPrevPacket;
			bRes = true;
			break;
		}

		++iCurIndex;
	}

	return bRes;
}

/**
 * @ingroup PcapViewer
 * @brief 입력된 인덱스의 패킷을 그 다음 패킷과 위치를 교체한다.
 * @param iIndex 패킷 인덱스
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CPacketList::Down( int iIndex )
{
	if( m_clsList.empty() ) return false;
	if( iIndex < 0 || iIndex >= ( (int)m_clsList.size() - 1 ) ) return false;

	PACKET_LIST::iterator itPL, itCur;
	int iCurIndex = 0;
	bool bRes = false;

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		if( iCurIndex == iIndex )
		{
			itCur = itPL;
		}
		else if( iCurIndex == ( iIndex + 1 ) )
		{
			CPacket * pclsCurPacket = *itCur;
			*itCur = *itPL;
			*itPL = pclsCurPacket;
			bRes = true;
			break;
		}

		++iCurIndex;
	}

	return bRes;
}

/**
 * @ingroup PcapViewer
 * @brief 자료구조에 저장된 패킷 정보들을 pcap 파일에 저장한다.
 * @param pszFileName 저장 pcap 파일 full path
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CPacketList::Save( const char * pszFileName )
{
	if( m_psttPcap == NULL || m_clsList.empty() ) return false;

	pcap_dumper_t * psttPcapDumper = pcap_dump_open( m_psttPcap, pszFileName );
	if( psttPcapDumper == NULL )
	{
		return false;
	}

	PACKET_LIST::iterator itPL;

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		pcap_dump( (u_char *)psttPcapDumper, &(*itPL)->m_sttHeader, (*itPL)->m_pszPacket );
	}

	pcap_dump_close( psttPcapDumper );

	return true;
}

/**
 * @ingroup PcapViewer
 * @brief 자료구조에 저장된 패킷 정보들을 삭제한다.
 */
void CPacketList::DeleteAll( )
{
	PACKET_LIST::iterator itPL;

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		delete (*itPL);
	}
	m_clsList.clear();
}
