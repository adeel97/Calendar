#pragma once
#include "stdafx.h"
#include "CalendarComponent.h"
#include<ctime>
using namespace std;

/*
Author: Jon Shidal
Purpose: declare functions for dealing with tm structs, particularly for advancing the date/time represented by a particular number of days.
*/

tm addDays(tm current, int numDays);
string dateString(tm date);
string timeString(tm date);
bool isNumber(string s);