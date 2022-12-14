#pragma once

#include "AccessLog.h"
#include <map>

typedef __int64 int64_t;

class CAccessLogStatMonth
{
public:
	CAccessLogStatMonth() : m_iSize(0)
	{}

	int64_t m_iSize;
};

// key = month/year
typedef std::map< std::string, CAccessLogStatMonth > ACCESS_LOG_STAT_MONTH_MAP;

class CAccessLogStat
{
public:
	CAccessLogStat();

	void Add( CAccessLog & clsLog );
	void PrintMonthStat( const char * pszOutputFolder );

private:
	ACCESS_LOG_STAT_MONTH_MAP m_clsMonthMap;
};
