#include "PcapSend.h"

/**
 * @ingroup PcapSend
 * @brief 디바이스 이름을 출력한다.
 */
void PrintDeviceName()
{
	pcap_if_t * psttAllDevice = NULL;
	pcap_if_t * psttDeviceEntry;
	char		szErrBuf[PCAP_ERRBUF_SIZE];

	// Retrieve the device list
	if( pcap_findalldevs( &psttAllDevice, szErrBuf ) == -1 )
	{
		printf( "Error in pcap_findpsttAllDevice: %s\n", szErrBuf );
		return;
	}
	
	if( psttAllDevice == NULL )
	{
		printf( "No interfaces found! Make sure WinPcap is installed.\n" );
		return;
	}
	
	// Jump to the selected adapter
	for( psttDeviceEntry = psttAllDevice; psttDeviceEntry; psttDeviceEntry = psttDeviceEntry->next )
	{
		printf( "[%s] [%s]\n", psttDeviceEntry->name, psttDeviceEntry->description );
	}

	pcap_freealldevs( psttAllDevice );
}
