#ifndef RTIME_H
#define RTIME_H

#include <chrono>
#include <ctime>

class RTime
{
public:
    enum TimeZone {
        UniversalTime,
        LocatlTime
    };

    using hours =  std::chrono::hours;
    using minutes =  std::chrono::minutes;
    using seconds =  std::chrono::seconds;
    using milliseconds =  std::chrono::milliseconds;//毫秒
    using microseconds =  std::chrono::microseconds;//纳秒

    //1970.1.1至今的duration
    static long long timeSinceEpoch() {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    RTime(TimeZone zone = LocatlTime): transformation(zone == LocatlTime ? std::localtime : std::gmtime) { update(); }

    void update() {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        time_ = *(transformation(&t));
    }

    int second() const { return time_.tm_sec; }
    int minute() const { return time_.tm_min; }
    int hour() const { return time_.tm_hour; }
    int day() const { return time_.tm_mday; }
    int month() const { return time_.tm_mon; }
    int year() const { return time_.tm_year + 1900; }
    int week() const { return time_.tm_wday; }
    int yearDay() const { return time_.tm_yday; }

private:
    tm* (*transformation)(const time_t *clock);
    tm time_;
};

#endif // RTIME_H
