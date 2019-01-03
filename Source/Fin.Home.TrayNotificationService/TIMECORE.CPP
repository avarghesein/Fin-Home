

#include "stdafx.h"
#include <cmath>
#include "TimeCore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTimeSpan::CTimeSpan()
	{ }

CTime::CTime()
	{ }

CTime::CTime(const CTime& timeSrc)
	{ m_time = timeSrc.m_time; }


// CTime and CTimeSpan
CTimeSpan::CTimeSpan(time_t time)
{ m_timeSpan = time; }
CTimeSpan::CTimeSpan(LONG lDays, int nHours, int nMins, int nSecs)
{ m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24* lDays)); }
CTimeSpan::CTimeSpan(const CTimeSpan& timeSpanSrc)
{ m_timeSpan = timeSpanSrc.m_timeSpan; }
const CTimeSpan& CTimeSpan::operator=(const CTimeSpan& timeSpanSrc)
{ m_timeSpan = timeSpanSrc.m_timeSpan; return *this; }
LONG CTimeSpan::GetDays() const
{ return m_timeSpan / (24*3600L); }
LONG CTimeSpan::GetTotalHours() const
{ return m_timeSpan/3600; }
int CTimeSpan::GetHours() const
{ return (int)(GetTotalHours() - GetDays()*24); }
LONG CTimeSpan::GetTotalMinutes() const
{ return m_timeSpan/60; }
int CTimeSpan::GetMinutes() const
{ return (int)(GetTotalMinutes() - GetTotalHours()*60); }
LONG CTimeSpan::GetTotalSeconds() const
{ return m_timeSpan; }
int CTimeSpan::GetSeconds() const
{ return (int)(GetTotalSeconds() - GetTotalMinutes()*60); }
CTimeSpan CTimeSpan::operator-(CTimeSpan timeSpan) const
{ return CTimeSpan(m_timeSpan - timeSpan.m_timeSpan); }
CTimeSpan CTimeSpan::operator+(CTimeSpan timeSpan) const
{ return CTimeSpan(m_timeSpan + timeSpan.m_timeSpan); }
const CTimeSpan& CTimeSpan::operator+=(CTimeSpan timeSpan)
{ m_timeSpan += timeSpan.m_timeSpan; return *this; }
const CTimeSpan& CTimeSpan::operator-=(CTimeSpan timeSpan)
{ m_timeSpan -= timeSpan.m_timeSpan; return *this; }
BOOL CTimeSpan::operator==(CTimeSpan timeSpan) const
{ return m_timeSpan == timeSpan.m_timeSpan; }
BOOL CTimeSpan::operator!=(CTimeSpan timeSpan) const
{ return m_timeSpan != timeSpan.m_timeSpan; }
BOOL CTimeSpan::operator<(CTimeSpan timeSpan) const
{ return m_timeSpan < timeSpan.m_timeSpan; }
BOOL CTimeSpan::operator>(CTimeSpan timeSpan) const
{ return m_timeSpan > timeSpan.m_timeSpan; }
BOOL CTimeSpan::operator<=(CTimeSpan timeSpan) const
{ return m_timeSpan <= timeSpan.m_timeSpan; }
BOOL CTimeSpan::operator>=(CTimeSpan timeSpan) const
{ return m_timeSpan >= timeSpan.m_timeSpan; }



CTime::CTime(time_t time)
{ m_time = time; }
const CTime& CTime::operator=(const CTime& timeSrc)
{ m_time = timeSrc.m_time; return *this; }
const CTime& CTime::operator=(time_t t)
{ m_time = t; return *this; }
time_t CTime::GetTime() const
{ return m_time; }
int CTime::GetYear() const
{ return (GetLocalTm(NULL)->tm_year) + 1900; }
int CTime::GetMonth() const
{ return GetLocalTm(NULL)->tm_mon + 1; }
int CTime::GetDay() const
{ return GetLocalTm(NULL)->tm_mday; }
int CTime::GetHour() const
{ return GetLocalTm(NULL)->tm_hour; }
int CTime::GetMinute() const
{ return GetLocalTm(NULL)->tm_min; }
int CTime::GetSecond() const
{ return GetLocalTm(NULL)->tm_sec; }
int CTime::GetDayOfWeek() const
{ return GetLocalTm(NULL)->tm_wday + 1; }
CTimeSpan CTime::operator-(CTime time) const
{ return CTimeSpan(m_time - time.m_time); }
CTime CTime::operator-(CTimeSpan timeSpan) const
{ return CTime(m_time - timeSpan.m_timeSpan); }
CTime CTime::operator+(CTimeSpan timeSpan) const
{ return CTime(m_time + timeSpan.m_timeSpan); }
const CTime& CTime::operator+=(CTimeSpan timeSpan)
{ m_time += timeSpan.m_timeSpan; return *this; }
const CTime& CTime::operator-=(CTimeSpan timeSpan)
{ m_time -= timeSpan.m_timeSpan; return *this; }
BOOL CTime::operator==(CTime time) const
{ return m_time == time.m_time; }
BOOL CTime::operator!=(CTime time) const
{ return m_time != time.m_time; }
BOOL CTime::operator<(CTime time) const
{ return m_time < time.m_time; }
BOOL CTime::operator>(CTime time) const
{ return m_time > time.m_time; }
BOOL CTime::operator<=(CTime time) const
{ return m_time <= time.m_time; }
BOOL CTime::operator>=(CTime time) const
{ return m_time >= time.m_time; }

/////////////////////////////////////////////////////////////////////////////
// CTime - absolute time

CTime::CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
	int nDST)
{
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	ATLASSERT(nDay >= 1 && nDay <= 31);
	atm.tm_mday = nDay;
	ATLASSERT(nMonth >= 1 && nMonth <= 12);
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	ATLASSERT(nYear >= 1900);
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
	ATLASSERT(m_time != -1);       // indicates an illegal input time
}

CTime::CTime(WORD wDosDate, WORD wDosTime, int nDST)
{
	struct tm atm;
	atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
	atm.tm_min = (wDosTime & ~0xF800) >> 5;
	atm.tm_hour = wDosTime >> 11;

	atm.tm_mday = wDosDate & ~0xFFE0;
	atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
	atm.tm_year = (wDosDate >> 9) + 80;
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
	ATLASSERT(m_time != -1);       // indicates an illegal input time
}

CTime::CTime(const SYSTEMTIME& sysTime, int nDST)
{
	if (sysTime.wYear < 1900)
	{
		time_t time0 = 0L;
		CTime timeT(time0);
		*this = timeT;
	}
	else
	{
		CTime timeT(
			(int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
			(int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
			nDST);
		*this = timeT;
	}
}

CTime::CTime(const FILETIME& fileTime, int nDST)
{
	// first convert file time (UTC time) to local time
	FILETIME localTime;
	if (!FileTimeToLocalFileTime(&fileTime, &localTime))
	{
		m_time = 0;
		return;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		m_time = 0;
		return;
	}

	// then convert the system time to a time_t (C-runtime local time)
	CTime timeT(sysTime, nDST);
	*this = timeT;
}

CTime PASCAL CTime::GetCurrentTime()
// return the current system time
{
	return CTime(::time(NULL));
}

struct tm* CTime::GetGmtTm(struct tm* ptm) const
{
	if (ptm != NULL)
	{
		*ptm = *gmtime(&m_time);
		return ptm;
	}
	else
		return gmtime(&m_time);
}

struct tm* CTime::GetLocalTm(struct tm* ptm) const
{
	if (ptm != NULL)
	{
		struct tm* ptmTemp = localtime(&m_time);
		if (ptmTemp == NULL)
			return NULL;    // indicates the m_time was not initialized!

		*ptm = *ptmTemp;
		return ptm;
	}
	else
		return localtime(&m_time);
}

BOOL CTime::GetAsSystemTime(SYSTEMTIME& timeDest) const
{
	struct tm* ptm = GetLocalTm(NULL);
	if (ptm == NULL)
		return FALSE;

	timeDest.wYear = (WORD) (1900 + ptm->tm_year);
	timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
	timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
	timeDest.wDay = (WORD) ptm->tm_mday;
	timeDest.wHour = (WORD) ptm->tm_hour;
	timeDest.wMinute = (WORD) ptm->tm_min;
	timeDest.wSecond = (WORD) ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return TRUE;
}


#define maxTimeBufferSize       128
	// Verifies will fail if the needed buffer size is too large

#ifdef _UNICODE
#endif

CString CTimeSpan::Format(LPCTSTR pFormat) const
// formatting timespans is a little trickier than formatting CTimes
//  * we are only interested in relative time formats, ie. it is illegal
//      to format anything dealing with absolute time (i.e. years, months,
//         day of week, day of year, timezones, ...)
//  * the only valid formats:
//      %D - # of days -- NEW !!!
//      %H - hour in 24 hour format
//      %M - minute (0-59)
//      %S - seconds (0-59)
//      %% - percent sign
{
	TCHAR szBuffer[maxTimeBufferSize];
	TCHAR ch;
	LPTSTR pch = szBuffer;

	while ((ch = *pFormat++) != '\0')
	{
		ATLASSERT(pch < &szBuffer[maxTimeBufferSize]);
		if (ch == '%')
		{
			switch (ch = *pFormat++)
			{
			default:
				ATLASSERT(FALSE);      // probably a bad format character
			case '%':
				*pch++ = ch;
				break;
			case 'D':
				pch += wsprintf(pch, _T("%ld"), GetDays());
				break;
			case 'H':
				pch += wsprintf(pch, _T("%02d"), GetHours());
				break;
			case 'M':
				pch += wsprintf(pch, _T("%02d"), GetMinutes());
				break;
			case 'S':
				pch += wsprintf(pch, _T("%02d"), GetSeconds());
				break;
			}
		}
		else
		{
			*pch++ = ch;
			if (_istlead(ch))
			{
				ATLASSERT(pch < &szBuffer[maxTimeBufferSize]);
				*pch++ = *pFormat++;
			}
		}
	}

	*pch = '\0';
	return szBuffer;
}

CString CTimeSpan::Format(UINT nFormatID) const
{
	CString strFormat;
	ATLASSERT(strFormat.LoadString(nFormatID) != 0);
	return Format(strFormat);
}

CString CTime::Format(LPCTSTR pFormat) const
{
	TCHAR szBuffer[maxTimeBufferSize];

	struct tm* ptmTemp = localtime(&m_time);
	if (ptmTemp == NULL ||
		!_tcsftime(szBuffer, _countof(szBuffer), pFormat, ptmTemp))
		szBuffer[0] = '\0';
	return szBuffer;
}

CString CTime::FormatGmt(LPCTSTR pFormat) const
{
	TCHAR szBuffer[maxTimeBufferSize];

	struct tm* ptmTemp = gmtime(&m_time);
	if (ptmTemp == NULL ||
		!_tcsftime(szBuffer, _countof(szBuffer), pFormat, ptmTemp))
		szBuffer[0] = '\0';
	return szBuffer;
}

CString CTime::Format(UINT nFormatID) const
{
	CString strFormat;
	ATLASSERT(strFormat.LoadString(nFormatID) != 0);
	return Format(strFormat);
}

CString CTime::FormatGmt(UINT nFormatID) const
{
	CString strFormat;
	ATLASSERT(strFormat.LoadString(nFormatID) != 0);
	return FormatGmt(strFormat);
}

#ifdef _UNICODE
// These functions are provided for compatibility with MFC 3.x
CString CTime::Format(LPCSTR pFormat) const
{
	CString strFormat(pFormat);
	return Format((LPCTSTR)strFormat);
}

CString CTime::FormatGmt(LPCSTR pFormat) const
{
	CString strFormat(pFormat);
	return FormatGmt((LPCTSTR)strFormat);
}

CString CTimeSpan::Format(LPCSTR pFormat) const
{
	CString strFormat = pFormat;
	return Format((LPCTSTR)strFormat);
}
#endif // _UNICODE

////////////////////////////////////////////////////////////////////////////
// out-of-line s for binary compatibility


// Verifies will fail if the needed buffer size is too large
#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999

// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)

// One-based array of days in year at month start
int _afxMonthDays[13] =
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/////////////////////////////////////////////////////////////////////////////
// COleDateTime class HELPERS - implementation

BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
	WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =
		_afxMonthDays[wMonth] - _afxMonthDays[wMonth-1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}

	// Cache the date in days and time in fractional days
	long nDate;
	double dblTime;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
		_afxMonthDays[wMonth-1] + wDay;

	//  If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return TRUE;
}

BOOL _AfxTmFromOleDate(DATE dtSrc, struct tm& tmDest)
{
	// The legal range does not actually span year 0 to 9999.
	if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
		n4Day > _afxMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - _afxMonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}


BOOL afxGetAsSystemTime(DATE dtVar, SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	struct tm tmTemp;
	if (_AfxTmFromOleDate(dtVar, tmTemp))
	{
		sysTime.wYear = (WORD) tmTemp.tm_year;
		sysTime.wMonth = (WORD) tmTemp.tm_mon;
		sysTime.wDayOfWeek = (WORD) (tmTemp.tm_wday - 1);
		sysTime.wDay = (WORD) tmTemp.tm_mday;
		sysTime.wHour = (WORD) tmTemp.tm_hour;
		sysTime.wMinute = (WORD) tmTemp.tm_min;
		sysTime.wSecond = (WORD) tmTemp.tm_sec;
		sysTime.wMilliseconds = 0;

		bRetVal = TRUE;
	}
	return bRetVal;
}

BOOL CTime::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID
                              lcid)
{
    USES_CONVERSION;
    
    SCODE sc;
	DATE dblTime = 0;	
    if (FAILED(sc = VarDateFromStr((LPOLESTR)T2COLE(lpszDate), lcid,
          dwFlags, &dblTime)))
    {
          if (sc == DISP_E_TYPEMISMATCH)
          {
               // Can't convert string to date, set 0 and invalidate
               m_time = 0;
               return FALSE;
          }
          else if (sc == DISP_E_OVERFLOW)
          {
               // Can't convert string to date, set -1 and invalidate
               m_time = -1;
               return FALSE;
          }
          else
          {
               m_time=0;
               return FALSE;
          }
        
    }

	SYSTEMTIME sysTime;
	memset(&sysTime,0,sizeof(SYSTEMTIME));
	if(!afxGetAsSystemTime(dblTime,sysTime))
	{
		m_time=0;
        return FALSE;
	}
	
	*this = sysTime;

    return TRUE;
}


