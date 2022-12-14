#include "AccessLogStat.h"

static char * garrMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };

CAccessLogStat::CAccessLogStat()
{
}

void CAccessLogStat::Add( CAccessLog & clsLog )
{
	std::string strMonth, strYearMonth;
	ACCESS_LOG_STAT_MONTH_MAP::iterator itSMM;

	if( clsLog.m_strDate.length() == 11 )
	{
		strMonth.append( clsLog.m_strDate.c_str() + 3, 3 );

		for( int i = 0; garrMonth[i]; ++i )
		{
			if( !strcmp( strMonth.c_str(), garrMonth[i] ) )
			{
				char szMonth[3];

				_snprintf( szMonth, sizeof(szMonth), "%02d", i + 1 );
				strMonth = szMonth;
				break;
			}
		}

		strYearMonth.append( clsLog.m_strDate.c_str() + 7 );
		strYearMonth.append( "/" );
		strYearMonth.append( strMonth );

		itSMM = m_clsMonthMap.find( strYearMonth );
		if( itSMM == m_clsMonthMap.end() )
		{
			CAccessLogStatMonth clsMonth;

			clsMonth.m_iSize = clsLog.m_iSize;

			m_clsMonthMap.insert( ACCESS_LOG_STAT_MONTH_MAP::value_type( strYearMonth, clsMonth ) );
		}
		else
		{
			itSMM->second.m_iSize += clsLog.m_iSize;
		}
	}
}

void CAccessLogStat::PrintMonthStat( const char * pszOutputFolder )
{
	ACCESS_LOG_STAT_MONTH_MAP::iterator itSMM;
	char szFileName[1024];

	_snprintf( szFileName, sizeof(szFileName), "%s\\month_size.csv", pszOutputFolder );

	FILE * fd = fopen( szFileName, "wb" );
	if( fd == NULL )
	{
		printf( "fopen(%s) error\n", szFileName );
		return;
	}

	for( itSMM = m_clsMonthMap.begin(); itSMM != m_clsMonthMap.end(); ++itSMM )
	{
		fprintf( fd, "%s,%lld\n", itSMM->first.c_str(), itSMM->second.m_iSize );
	}

	fclose( fd );
}
