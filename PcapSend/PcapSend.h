#ifndef _PCAP_SEND_H_
#define _PCAP_SEND_H_

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "pcap.h"

// PcapUtility.cpp
void PrintDeviceName();

// TimeUtility.cpp
int DiffTimeval( struct timeval * psttOld, struct timeval * psttNew );

#endif
