#include "PcapSend.h"

/**
 * @ingroup PcapSend
 * @brief �� ���� �ð��� ���̸� �����Ѵ�.
 * @param psttOld ���� �ð�
 * @param psttNew ���� �ð�
 * @returns �� ���� �ð��� ���̸� �����Ѵ�.
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
