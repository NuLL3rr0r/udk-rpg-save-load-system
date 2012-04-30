#ifndef CDATE_HPP
#define CDATE_HPP


#include <string>

namespace CDate {
    class DateConv;
    class Now;
}

class CDate::DateConv
{
public:
    static std::string CalcToG(int jYear, int dayOfYear);
    static std::string CalcToJ(int gYear, int dayOfYear);
    static bool IsRangeValidG(int gYear, int gMonth, int gDay);
    static bool IsRangeValidJ(int jYear, int jMonth, int jDay);
    static int DayOfYearG(int gYear, int gMonth, int gDay);
    static int DayOfYearJ(int jYear, int jMonth, int jDay);
    static bool IsLeapYearG(int gYear);
    static bool IsLeapYearJ(int jYear);
    static std::string ToGregorian(int jYear, int jMonth, int jDay);
    static std::string ToGregorian();
    static std::string ToJalali(int gYear, int gMonth, int gDay);
    static std::string ToJalali();
    static std::string ToGregorian(const CDate::Now &now);
    static std::string ToJalali(const CDate::Now &now);
    static std::string Time(const CDate::Now &now);
    static std::string RawLocalDateTime(const CDate::Now &now);
    static std::wstring GetPersianDayOfWeek(const CDate::Now &now);
    static std::wstring FormatToPersianNums(const std::string &date);

private:
    static std::string IntToStr(int num);
};

class CDate::Now
{
public:
    Now();

	time_t RawTime;
    int Hour;
    int DaylightSavingTime;
    int DayOfMonth;
    int Minutes;
    int Month;
    int Seconds;
    int DayOfWeek;
    int DayOfYear;
    int Year;
    struct tm *TimeInfo;
};


#endif /* CDATE_HPP */


