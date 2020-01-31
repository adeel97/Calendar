#include "stdafx.h"
#include "utilityfunctions.h"
#include "CalendarComponent.h"
#include<time.h>
using namespace std;

// this function advances a tm struct ahead by some number of days
tm addDays(tm current, int numDays) {
	// convert current tm to time_t (stores number of seconds since 1/1/1970)
	time_t t = mktime(&current);
	int secondsToAdd = numDays * 60 * 60 * 24;
	t += secondsToAdd;
	// convert back to a tm struct, stored in local time
	tm temp;
	localtime_s(&temp, &t);
	return temp;
}

string dateString(tm date) {
	string dateString;
	dateString.append(to_string(date.tm_mon + 1));
	dateString.append("/");
	dateString.append(to_string(date.tm_mday));
	dateString.append("/");
	dateString.append(to_string(date.tm_year + 1900));
	return dateString;
}

string timeString(tm date) {
	string timeString;
	if (date.tm_hour < 10) {
		timeString.append("0");
	}
	timeString.append(to_string(date.tm_hour));
	timeString.append(":");
	if (date.tm_min < 10) {
		timeString.append("0");
	}
	timeString.append(to_string(date.tm_min));
	return timeString;
}

bool isNumber(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;

	return true;
}