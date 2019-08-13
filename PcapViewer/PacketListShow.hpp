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

void ShowPacketInfo( CListCtrl & clsList, int iRow, const char * pszUdpBody, int iUdpBodyLen, uint16_t sFragmentOffset )
{
	char szBuf[255];

	if( IsPrintString( pszUdpBody, iUdpBodyLen ) )
	{
		if( iUdpBodyLen > ( sizeof(szBuf) - 1 ) )
		{
			snprintf( szBuf, sizeof(szBuf)-1, ".*s", sizeof(szBuf)-1, pszUdpBody );
		}
		else
		{
			snprintf( szBuf, sizeof(szBuf), ".*s", iUdpBodyLen, pszUdpBody );
		}
	}
	else
	{
		szBuf[0] = '\0';
	}

	if( sFragmentOffset )
	{
		char szNewBuf[512];

		snprintf( szNewBuf, sizeof(szNewBuf), "IP fragment offset(%d) %s", sFragmentOffset * 8, szBuf );
		clsList.SetItemText( iRow, 6, szNewBuf );
	}
	else
	{
		clsList.SetItemText( iRow, 6, szBuf );
	}
}

void CPacketList::ShowList( CListCtrl & clsList )
{
	PACKET_LIST::iterator itPL;
	int iRow = 0;
	int iIpPos, iIpHeaderLen, iTcpHeaderLen;
	int iUdpBodyLen, iTcpBodyLen;
	char szTemp[255];
	struct tm	sttTm;
	uint16_t sFlags, sFragmentOffset;

	Ip4Header		* psttIp4Header;
	UdpHeader		* psttUdpHeader;
	TcpHeader		* psttTcpHeader;

	char				* pszUdpBody, * pszTcpBody;

	clsList.DeleteAllItems();

	for( itPL = m_clsList.begin(); itPL != m_clsList.end(); ++itPL )
	{
		snprintf( szTemp, sizeof(szTemp), "%d", (*itPL)->m_iId );
		clsList.InsertItem( iRow, szTemp );

		localtime_s( &sttTm, (time_t *)&(*itPL)->m_sttHeader.ts.tv_sec );
		snprintf( szTemp, sizeof(szTemp), "%02d:%02d:%02d.%06d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, (*itPL)->m_sttHeader.ts.tv_usec );
		clsList.SetItemText( iRow, 1, szTemp );

		iIpPos = 0;

		if( (*itPL)->m_pszPacket[12] == 0x81 )
		{
			iIpPos = 18;		// VLAN
		}
		else if( (*itPL)->m_pszPacket[12] == 0x08 )
		{
			iIpPos = 14;		// IP
		}
		
		if( iIpPos == 0 || (*itPL)->m_pszPacket[13] )
		{
			GetMacString( (*itPL)->m_pszPacket, szTemp, sizeof(szTemp) );
			clsList.SetItemText( iRow, 2, szTemp );

			GetMacString( (*itPL)->m_pszPacket + 6, szTemp, sizeof(szTemp) );
			clsList.SetItemText( iRow, 3, szTemp );

			if( (*itPL)->m_pszPacket[13] == 0x06 )
			{
				clsList.SetItemText( iRow, 4, "ARP" );
			}
		}
		else
		{
			psttIp4Header = ( Ip4Header * )( (*itPL)->m_pszPacket + iIpPos );

			GetIpString( psttIp4Header->saddr, szTemp, sizeof(szTemp) );
			clsList.SetItemText( iRow, 2, szTemp );

			GetIpString( psttIp4Header->daddr, szTemp, sizeof(szTemp) );
			clsList.SetItemText( iRow, 3, szTemp );

			sFragmentOffset = 0;
			sFlags = ntohs(psttIp4Header->flags_fo);
			if( sFlags )
			{
				sFragmentOffset = sFlags & 0x1FFF;
			}

			iIpHeaderLen = GetIpHeaderLength( psttIp4Header );

			if( sFragmentOffset )
			{
				snprintf( szTemp, sizeof(szTemp), "IP fragment offset(%d)", sFragmentOffset * 8 );

				pszUdpBody = (char *)( (*itPL)->m_pszPacket + iIpPos + iIpHeaderLen );
				iUdpBodyLen = (*itPL)->m_sttHeader.caplen - ( iIpPos + iIpHeaderLen );

				ShowPacketInfo( clsList, iRow, pszUdpBody, iUdpBodyLen, sFragmentOffset );
			}
			else if( IsUdpPacket( psttIp4Header ) )
			{
				clsList.SetItemText( iRow, 4, "UDP" );

				psttUdpHeader = (UdpHeader *)( (*itPL)->m_pszPacket + iIpPos + iIpHeaderLen );
				pszUdpBody = (char *)( (*itPL)->m_pszPacket + iIpPos + iIpHeaderLen + 8 );
				iUdpBodyLen = (*itPL)->m_sttHeader.caplen - ( iIpPos + iIpHeaderLen + 8 );

				ShowPacketInfo( clsList, iRow, pszUdpBody, iUdpBodyLen, sFragmentOffset );
			}
			else if( IsTcpPacket( psttIp4Header ) )
			{
				clsList.SetItemText( iRow, 4, "TCP" );

				psttTcpHeader = (TcpHeader *)( (*itPL)->m_pszPacket + iIpPos + iIpHeaderLen );
				iTcpHeaderLen = GetTcpHeaderLength( psttTcpHeader );
				pszTcpBody = (char *)( (*itPL)->m_pszPacket + iIpPos + iIpHeaderLen + iTcpHeaderLen );
				iTcpBodyLen = (*itPL)->m_sttHeader.caplen - ( iIpPos + iIpHeaderLen + iTcpHeaderLen );

				ShowPacketInfo( clsList, iRow, pszTcpBody, iTcpBodyLen, sFragmentOffset );
			}
		}

		snprintf( szTemp, sizeof(szTemp), "%d", (*itPL)->m_sttHeader.caplen );
		clsList.SetItemText( iRow, 5, szTemp );

		++iRow;
	}
}
