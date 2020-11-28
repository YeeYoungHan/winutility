#include "PcapSend.h"

int main( int argc, char * argv[] )
{
	if( argc == 2 && !strcmp( argv[1], "-l" ) )
	{
		PrintDeviceName();
		return 0;
	}

	if( argc != 3 )
	{
		printf( "[Usage] %s {pcap file} {network device}\n", argv[0] );
		return 0;
	}

	char * pszPcapFileName = argv[1];
	char * pszNetworkDevice = argv[2];

	pcap_t * psttPcapFile, * psttPcapSend;
	struct pcap_pkthdr * psttHeader;
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	const		u_char * pszData;
	struct timeval sttStartTime;
	int			iDiff, iMiliSecond;
	clock_t	iStartClock, iClock;

	memset( &sttStartTime, 0, sizeof(sttStartTime) );

	psttPcapFile = pcap_open_offline( pszPcapFileName, szErrBuf );
	if( psttPcapFile == NULL )
	{
		printf( "pcap_open_offline(%s) error\n", pszPcapFileName );
		return 0;
	}

	psttPcapSend = pcap_open_live( pszNetworkDevice, 1500, 1, 1000, szErrBuf );
	if( psttPcapSend == NULL )
	{
		printf( "pcap_open(%s) error\n", pszNetworkDevice );
	}
	else
	{
		while( pcap_next_ex( psttPcapFile, &psttHeader, &pszData ) > 0 )
		{
			if( sttStartTime.tv_sec || sttStartTime.tv_usec )
			{
				iDiff = DiffTimeval( &sttStartTime, &psttHeader->ts );
				iClock = clock();
				iMiliSecond = iDiff - ( iClock - iStartClock );

				if( iMiliSecond > 0 )
				{
					Sleep( iMiliSecond );
				}
			}

			pcap_sendpacket( psttPcapSend, pszData, psttHeader->caplen );

			if( sttStartTime.tv_sec == 0 && sttStartTime.tv_usec == 0 )
			{
				sttStartTime = psttHeader->ts;
				iStartClock = clock();
			}
		}

		pcap_close( psttPcapSend );
	}

	pcap_close( psttPcapFile );

	return 0;
}
