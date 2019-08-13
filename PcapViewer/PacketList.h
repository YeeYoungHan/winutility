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

#ifndef _PACKET_LIST_H_
#define _PACKET_LIST_H_

#include "pcap.h"
#include <list>

class CPacket
{
public:
	CPacket();
	~CPacket();

	int			m_iId;
	struct pcap_pkthdr m_sttHeader;
	u_char * m_pszPacket;
};

typedef std::list< CPacket * > PACKET_LIST;

class CPacketList
{
public:
	CPacketList();
	~CPacketList();

	bool Open( const char * pszFileName );
	bool Close();

	bool Up( int iIndex );
	bool Down( int iIndex );

	bool Save( const char * pszFileName );

	void DeleteAll();

	void ShowList( CListCtrl & clsList );

	PACKET_LIST m_clsList;

private:
	pcap_t * m_psttPcap;
};

#endif
