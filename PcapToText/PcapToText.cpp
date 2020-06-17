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

#include "PcapToText.h"

static bool gbUdp = true;
static int giPort = 0;
static FILE * gsttFd = NULL;

void StringToHex( const u_char * pszBody, int iBodyLen, std::string & strOutput )
{
	char szHex[3];

	strOutput.clear();

	for( int i = 0; i < iBodyLen; ++i )
	{
		snprintf( szHex, sizeof(szHex), "%02x", pszBody[i] );
		strOutput.append( szHex );
	}
}

void ParsePacket( struct pcap_pkthdr * psttHeader, const u_char * pszData )
{
	int iIpPos = 14, iBodyLen = 0;
	const u_char * pszBody;
	unsigned short sSrcPort = 0, sDstPort = 0;

	if( pszData[12] == 0x81 )
	{
		iIpPos = 18;		// VLAN
	}

	Ip4Header * psttIp4Header = ( Ip4Header * )( pszData + iIpPos );
	int iIpHeaderLen = GetIpHeaderLength( psttIp4Header );

	if( IsUdpPacket( psttIp4Header ) )
	{
		if( gbUdp )
		{
			UdpHeader * psttUdpHeader = (UdpHeader *)( pszData + iIpPos + iIpHeaderLen );
			pszBody = pszData + iIpPos + iIpHeaderLen + 8;
			iBodyLen = psttHeader->caplen - ( iIpPos + iIpHeaderLen + 8 );
			sSrcPort = ntohs( psttUdpHeader->sport );
			sDstPort = ntohs( psttUdpHeader->dport );
		}
	}
	else if( IsTcpPacket( psttIp4Header ) )
	{
		if( gbUdp == false )
		{
			TcpHeader * psttTcpHeader = (TcpHeader *)( pszData + iIpPos + iIpHeaderLen );
			int iTcpHeaderLen = GetTcpHeaderLength( psttTcpHeader );
			pszBody = pszData + iIpPos + iIpHeaderLen + iTcpHeaderLen;
			iBodyLen = psttHeader->caplen - ( iIpPos + iIpHeaderLen + iTcpHeaderLen );
			sSrcPort = ntohs( psttTcpHeader->sport );
			sDstPort = ntohs( psttTcpHeader->dport );
		}
	}

	if( iBodyLen > 0 )
	{
		struct tm	sttTm;
		char szTime[21], szSrcIp[21], szDstIp[21];
		std::string strBody;

		_localtime32_s( &sttTm, &psttHeader->ts.tv_sec );
		snprintf( szTime, sizeof(szTime), "%02d:%02d:%02d.%06d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, psttHeader->ts.tv_usec );

		GetIpString( psttIp4Header->saddr, szSrcIp, sizeof(szSrcIp) );
		GetIpString( psttIp4Header->daddr, szDstIp, sizeof(szDstIp) );
		StringToHex( pszBody, iBodyLen, strBody );

		fprintf( gsttFd, "[%s] (%s:%d=>%s:%d) [%s]\n", szTime, szSrcIp, sSrcPort, szDstIp, sDstPort, strBody.c_str() );
	}
}

int main( int argc, char * argv[] )
{
	if( argc != 5 )
	{
		printf( "[Usage] %s {pcap file} {output file} {tcp|udp} {port number}\n", argv[0] );
		return 0;
	}

	const char * pszPcapFileName = argv[1];
	const char * pszTxtFileName = argv[2];
	const char * pszProtocol = argv[3];
	giPort = atoi( argv[4] );

	if( !strcasecmp( pszProtocol, "tcp" ) )
	{
		gbUdp = false;
	}

	pcap_t * psttPcap;
	struct pcap_pkthdr * psttHeader;
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	const		u_char * pszData;

	psttPcap = pcap_open_offline( pszPcapFileName, szErrBuf );
	if( psttPcap == NULL )
	{
		printf( "pcap_open_offline(%s) error\n", pszPcapFileName );
		return 0;
	}

	gsttFd = fopen( pszTxtFileName, "wb" );
	if( gsttFd == NULL )
	{
		printf( "fopen(%s) error\n", pszTxtFileName );
	}
	else
	{
		while( pcap_next_ex( psttPcap, &psttHeader, &pszData ) > 0 )
		{
			ParsePacket( psttHeader, pszData );
		}
	}

	if( gsttFd ) fclose( gsttFd );
	pcap_close( psttPcap );

	return 0;
}
