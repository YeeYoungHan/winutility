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
