#include "PcapSend.h"

/**
 * @ingroup PcapSend
 * @brief 두 개의 시간의 차이를 리턴한다.
 * @param psttOld 시작 시간
 * @param psttNew 종료 시간
 * @returns 두 개의 시간의 차이를 리턴한다.
 */
int DiffTimeval( struct timeval * psttOld, struct timeval * psttNew )
{
	int	iRet;

	if( psttNew->tv_usec >= psttOld->tv_usec )
	{
		iRet = ( psttNew->tv_usec - psttOld->tv_usec ) / 1000;
		iRet += ( psttNew->tv_sec - psttOld->tv_sec ) * 1000;
	}
	else
	{
		iRet = 1000 + ( psttNew->tv_usec - psttOld->tv_usec ) / 1000;
		iRet += ( psttNew->tv_sec - psttOld->tv_sec - 1 ) * 1000;
	}

	return iRet;
}
