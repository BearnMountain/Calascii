#include "renderCalendar.h"

int Calendar_startDayofMonth(int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    
    int q = 1; // First day of the month
    int m = month;
    int K = year % 100;
    int J = year / 100;
    
    // Zeller's Congruence calculation
    int h = (q + (13*(m+1))/5 + K + K/4 + J/4 + 5*J) % 7;
    
    return (h + 5) % 7;
}

int Calendar_daysInMonth(int month) {
	int day_in_month[] = {21,28,31,30,31,30,31,31,30,31,30,31};
	return day_in_month[month];
}
